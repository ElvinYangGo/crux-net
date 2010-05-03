#include "Dispatcher.h"
#include "Config.h"
#include <fcntl.h>
#include "Linux/AsyncOperationQueue.h"
#include "Logger.h"
#include "HelperFunction.h"
#include <errno.h>
#include "Thread.h"

namespace Crux
{

Dispatcher::Dispatcher()
{
	m_Epoll = epoll_create( s_Config.GetClientSessionAmountMax() + s_Config.GetServerSessionAmountMax() );
	if ( -1 == m_Epoll )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to create epoll, " << FormatErrorMessage( errno ) );
		return;
	}

	//start dispatcher thread
	if( !Start( true ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to start dispatcher thread" );
		return;
	}
	LOG_INFO( logger, "start dispatcher" );
}

Dispatcher::~Dispatcher()
{
	if( -1 == close( m_Epoll ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to close epoll fd, " << FormatErrorMessage( errno ) );
	}
}

void Dispatcher::Shutdown()
{
	Thread::Shutdown();

	if( 0 != pthread_cancel( CurrentThread() ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to cancel dispatcher thread, " << FormatErrorMessage( errno ) );	
	}
}

bool Dispatcher::RegisterSocket(
	int sock, 
	AsyncCompleteOperation* pAsyncCompleteOperation, 
	unsigned int events /* = EPOLLIN | EPOLLOUT | EPOLLET  */
	)
{
	//set non-blocking for new socket
	if ( !SetNonBlocking( sock ) )
	{
		return false;
	}

	//add new socket to epoll set
	epoll_event newSockEvent;
	newSockEvent.events = events;
	newSockEvent.data.ptr = pAsyncCompleteOperation;
	if( -1 == epoll_ctl( m_Epoll, EPOLL_CTL_ADD, sock, &newSockEvent ) )
	{
		return false;
	}

	return true;
}

void Dispatcher::Run()
{
	int eventsMax = s_Config.GetClientSessionAmountMax() + s_Config.GetServerSessionAmountMax();
	epoll_event* events = new epoll_event[eventsMax];

	while( Runnable() )
	{
		int sockAmount = epoll_wait( m_Epoll, events, eventsMax, -1 );
		if( -1 == sockAmount || 0 == sockAmount )
		{
			LOG_ERROR( logger, __FUNCTION__ << " failed epoll_wait(), " << FormatErrorMessage( errno ) );
			break;
		}

		//for each file descriptors
		for ( int i = 0; i < sockAmount; ++i )
		{
			if( events[i].events & EPOLLERR )
			{
				LOG_INFO( logger,  "EPOLLERR found!" );
				if( !s_AsyncOperationQueue.InsertAsyncOperation( 
					static_cast<AsyncCompleteOperation*>( events[i].data.ptr ),
					ASYNC_OPERATION_ERROR
					)
					)
				{
					LOG_ERROR( logger, __FUNCTION__ << " failed to insert asynchronous operation, operation type is ASYNC_OPERATION_ERROR!");
				}
			}
			else if( events[i].events & EPOLLIN ) 	//sock could be read
			{
				//				std::cout << "EPOLLIN found" << std::endl;
				if( !s_AsyncOperationQueue.InsertAsyncOperation( 
					static_cast<AsyncCompleteOperation*>( events[i].data.ptr ),
					ASYNC_OPERATION_RECV
					)
					)
				{
					LOG_ERROR( logger, __FUNCTION__ << " failed to insert asynchronous operation, operation type is ASYNC_OPERATION_RECV!");
				}
			}
			else if( events[i].events & EPOLLOUT ) 	//sock could be write
			{
				//				std::cout << "EPOLLOUT found" << std::endl;
				if( !s_AsyncOperationQueue.InsertAsyncOperation( 
					static_cast<AsyncCompleteOperation*>( events[i].data.ptr ),
					ASYNC_OPERATION_SEND
					)
					)
				{
					LOG_ERROR( logger, __FUNCTION__ << " failed to insert asynchronous operation, operation type is ASYNC_OPERATION_SEND!");
				}
			}
			//为什么不管接收还是关闭，都会同时受到IN和OUT???
			/*
			else if( events[i].events & EPOLLIN && events[i].events & EPOLLOUT )
			{
				LOG_INFO( logger, " EPOLLIN and EPOLLOUT found" );
			}
			*/
		}
	}

	LOG_INFO( logger, "dispather thread exit" );
}

bool Dispatcher::SetNonBlocking(int fd )
{
	//set non-blocking for file descriptor
	int flags = fcntl( fd, F_GETFL, 0 );
	if ( -1 == flags )
	{
		return false;
	}
	flags |= O_NONBLOCK;
	if ( -1 == fcntl( fd, F_SETFL, flags ) )
	{
		return false;
	}

	return true;
}

}


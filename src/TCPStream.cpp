#include "TCPStream.h"
#include "Config.h"
#include "WorkerThread.h"
#include "Logger.h"
#include "Session.h"
#include "SessionMgr.h"
#include "MsgQueue.h"
#include "HelperFunction.h"
#include "Dispatcher.h"

#ifdef LINUX
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Linux/AsyncOperationQueue.h"
#endif

namespace Crux
{

TCPStream::TCPStream()
{
#ifdef WIN32
	//initialize Winsock
	WSADATA wsaData;
	if ( NO_ERROR != WSAStartup( MAKEWORD(2,2), &wsaData ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed WSAStartup(), " << FormatErrorMessage( WSAGetLastError() ) );
	}
#endif

	s_Dispatcher;

	//create worker thread
	for( int i = 0; i < s_Config.GetThreadAmount(); ++i )
	{
		m_Threads.push_back( new WorkerThread );
	}
	LOG_INFO( logger, "start all worker threads" );
}

TCPStream::~TCPStream()
{
	THREADS::iterator it = m_Threads.begin();
	THREADS::iterator end = m_Threads.end();
	for (; end != it; ++it )
	{
		delete (*it);
	}

#ifdef WIN32
	if( NO_ERROR != WSACleanup() )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed WSACleanup(), " << FormatErrorMessage( WSAGetLastError() ) );
	}
#endif
}

bool TCPStream::Connect( const std::string& ip, int port )
{
	SOCKET newSock = socket( AF_INET, SOCK_STREAM, 0 );

#ifdef WIN32
	if( INVALID_SOCKET == newSock )
#elif defined(LINUX)
	if ( -1 == newSock )
#endif
	{
		return false;
	}

	sockaddr_in serverAddr;
	memset( &serverAddr, 0, sizeof(serverAddr) );
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons( port );
#ifdef WIN32
	serverAddr.sin_addr.s_addr = inet_addr( ip.c_str() );
#elif defined(LINUX)
	if ( 1 != inet_pton( AF_INET, ip.c_str(), &serverAddr.sin_addr.s_addr ) )
	{
		return false;
	}
#endif

	//blocking connect to the server
#ifdef WIN32
	if( SOCKET_ERROR == connect( newSock, (sockaddr*)&serverAddr, sizeof(serverAddr) ) )
#elif defined(LINUX)
	if( -1 == connect( newSock, (sockaddr*)&serverAddr, sizeof(serverAddr) ) )
#endif
	{
		return false;
	}

	Session* pSession = s_SessionMgr.CreateSession( newSock, ip, port );
	if( NULL == pSession )
	{
		LOG_ERROR( logger, __FUNCTION__ 
				<< " failed to create session, sock:"
				<< newSock << ",ip:" << ip << ",port:" << port 
				);
	}

	//notify connect
	s_MsgQueue.InsertSimpleMsg( MSG_CONNECT, pSession->GetID(), pSession );

	//add sock to epoll set
#ifdef WIN32
	if( !s_Dispatcher.RegisterSocket( newSock, pSession ) )
#elif defined(LINUX)
	if( !s_Dispatcher.RegisterSocket( newSock, pSession->GetAsyncCompleteOperation() ) )
#endif
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to register sock, sock:" << newSock );
		return false;
	}

#ifdef WIN32
	pSession->PostRecvOperation();
#endif

	return true;
}

void TCPStream::Shutdown()
{
#ifdef LINUX
	//shutdown dispatcher thread, ie epoll thread
	s_Dispatcher.Shutdown();
#endif

	//shutdown worker thread
	THREADS::iterator it = m_Threads.begin();
	THREADS::iterator end = m_Threads.end();
	for( ; end != it; ++it )
	{
		(*it)->Shutdown();
	}

	for( size_t n = m_Threads.size(); n != 0; --n )
	{
#ifdef WIN32
		BOOL ret = PostQueuedCompletionStatus(
			s_Dispatcher.GetIOCPHandle(), 
			0, 
			0,
			0
			);
		if ( 0 == ret )
		{
			int errNum = WSAGetLastError();
			if ( WSA_IO_PENDING != errNum )
			{
				LOG_ERROR( logger, __FUNCTION__ 
					<< " failed PostQueuedCompletionStatus(), " << FormatErrorMessage( errNum )
				   	);
			}
		}
#elif defined(LINUX)
		s_AsyncOperationQueue.InsertAsyncOperation( NULL, ASYNC_OPERATION_SHUTDOWN );
#endif
	}

	//clear all sessions
	s_SessionMgr.DestroyAllSessions();
}

} //namespace Crux

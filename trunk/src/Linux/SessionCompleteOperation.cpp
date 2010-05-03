#include "Linux/SessionCompleteOperation.h"
#include "Session.h"
#include "SessionMgr.h"
#include "MutexGuard.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include "MsgQueue.h"
#include "Logger.h"
#include "Linux/AsyncOperationQueue.h"
#include "HelperFunction.h"

namespace Crux
{

void SessionCompleteOperation::Complete( AsyncOperationType asyncOperationType )
{
	switch( asyncOperationType )
	{
	case ASYNC_OPERATION_RECV:
		Recv();
		break;
	case ASYNC_OPERATION_SEND:
		Send();
		break;
	case ASYNC_OPERATION_CLOSE:
	case ASYNC_OPERATION_ERROR:
		Close();
		break;
	default:
		break;
	}
}

void SessionCompleteOperation::SetSession(Session* pSession )
{
	m_pSession = pSession;
}

void SessionCompleteOperation::Recv()
{
	MutexGuard guard( m_pSession->GetRecvLock() );

	if( !s_SessionMgr.SessionActive( m_pSession->GetID() ) )
	{
		return;
	}

	int ret = 0;
	while(true)
	{
		ret = recv( m_pSession->GetSock(), m_pSession->GetRecvBuffer()->GetEnd(), m_pSession->GetRecvBuffer()->GetFreeSpaceSize(), 0 );
		if( -1 == ret )
		{
			int errNum = errno;
			if( EWOULDBLOCK == errNum )
			{
				m_pSession->ExtractMsg();
				m_pSession->GetRecvBuffer()->MoveData();

				return;
			}
			else
			{
				LOG_ERROR( logger, __FUNCTION__ << " failed recv(), " << FormatErrorMessage( errNum ) );
				return;
			}
		}
		else if( 0 == ret )
		{
			s_SessionMgr.SetSessionActive( m_pSession->GetID(), false );

			s_AsyncOperationQueue.InsertAsyncOperation( this, ASYNC_OPERATION_CLOSE );

			return;
		}
		m_pSession->GetRecvBuffer()->IncreaseEnd( ret );

		m_pSession->IncreaseRecvDataSizeTotal( ret );

	}
}

void SessionCompleteOperation::Send()
{
	MutexGuard guard( m_pSession->GetSendLock() );

	if( !s_SessionMgr.SessionActive( m_pSession->GetID() ) )
	{
		return;
	}

	int ret = 0;
	while( true )
	{
		if( 0 == m_pSession->GetSendBuffer()->GetDataSize() )
		{
			return;
		}

		ret = send( m_pSession->GetSock(), m_pSession->GetSendBuffer()->GetBegin(), m_pSession->GetSendBuffer()->GetDataSize(), 0 );
		if( -1 == ret )
		{
			int errNum = errno;
			if( EWOULDBLOCK == errNum )
			{
				m_pSession->GetSendBuffer()->MoveData();

				//why send() is blocked?
				LOG_ERROR( logger, __FUNCTION__ << " failed send(), EWOULDBLOCK returned!" );
				return;
			}
			else if( EBADF == errNum )
			{
				//socket has already been closed
				return;
			}
			else
			{
				LOG_ERROR( logger, __FUNCTION__ << " failed send(), " << FormatErrorMessage( errNum ) );
				return;
			}
		}

		m_pSession->GetSendBuffer()->IncreaseBegin( ret );

		m_pSession->IncreaseSendDataSizeTotal( ret );
	}
}

bool SessionCompleteOperation::Close()
{
	MutexGuard recvGuard( m_pSession->GetRecvLock() );
	MutexGuard sendGuard( m_pSession->GetSendLock() );

	s_SessionMgr.SetSessionActive( m_pSession->GetID(), false );


	if( m_pSession->GetRecvDataSizeTotal() != m_pSession->GetSendDataSizeTotal() )
	{
		int recvSize = m_pSession->GetRecvDataSizeTotal();
		int sendSize = m_pSession->GetSendDataSizeTotal();
		LOG_INFO( logger, "recv data total: " << recvSize << ",\tsend data total: " << sendSize );
	}

	//close socket
	if( -1 == close( m_pSession->GetSock() ) )
	{
		LOG_INFO( logger, __FUNCTION__ << " failed to close socket, " << FormatErrorMessage( errno ) );
		return false;
	}

	//notify close
	s_MsgQueue.InsertSimpleMsg( MSG_CLOSE, m_pSession->GetID(), NULL );

	s_SessionMgr.DestroySession( m_pSession );

	return true;
}

} //namespace Crux


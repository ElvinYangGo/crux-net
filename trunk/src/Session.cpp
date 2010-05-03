#include "Session.h"
#include "MsgQueue.h"
#include "MsgPool.h"
#include "MutexGuard.h"
#include "SessionMgr.h"
#include "Logger.h"
#include "HelperFunction.h"

namespace Crux
{

#ifdef WIN32
Session::Session( int id, SOCKET sock, const std::string& ip, int port )
: m_SendDataSizeTotal(0), m_RecvDataSizeTotal(0),
m_ID(id), m_Sock(sock), m_IP(ip), m_Port(port),
m_Sending( false ), m_SendingDataSize(0),
m_RecvCompleteOperation( &m_RecvHandler, &m_CloseHandler ),
m_SendCompleteOperation( &m_SendHandler, &m_CloseHandler )
{
}

#elif defined(LINUX)
Session::Session( int id, SOCKET sock, const std::string& ip, int port )
: m_SendDataSizeTotal(0), m_RecvDataSizeTotal(0),
m_ID(id), m_Sock(sock), m_IP(ip), m_Port(port),
m_Sending( false ), m_SendingDataSize(0)
{
	m_SessionCompleteOperation.SetSession( this );
}

#endif

Session::~Session()
{
}

Mutex* Session::GetRecvLock()
{
	return &m_RecvMutex;
}

Mutex* Session::GetSendLock()
{
	return &m_SendMutex;
}

void Session::RecvLock()
{
	m_RecvMutex.Lock();
}

void Session::RecvUnLock()
{
	m_RecvMutex.UnLock();
}

void Session::SendLock()
{
	m_SendMutex.Lock();
}

void Session::SendUnLock()
{
	m_SendMutex.UnLock();
}

bool Session::Sending() const
{
	return m_Sending;
}

void Session::SetSending( bool sending )
{
	m_Sending = sending;
}

size_t Session::GetSendingDataSize() const
{
	return m_SendingDataSize;
}

Buffer* Session::GetRecvBuffer()
{
	return &m_RecvBuffer;
}

Buffer* Session::GetSendBuffer()
{
	return &m_SendBuffer;
}

 
int Session::GetID() const
{
	return m_ID;
}

 
SOCKET Session::GetSock() const
{
	return m_Sock;
}


const std::string& Session::GetIP() const
{
	return m_IP;
}


int Session::GetPort() const
{
	return m_Port;
}

size_t Session::GetSendDataSizeTotal() const
{
	return m_SendDataSizeTotal;
}


void Session::IncreaseSendDataSizeTotal( size_t sizeIncreased )
{
	m_SendDataSizeTotal += sizeIncreased;
}


size_t Session::GetRecvDataSizeTotal() const
{
	return m_RecvDataSizeTotal;
}


void Session::IncreaseRecvDataSizeTotal( size_t sizeIncreased )
{
	m_RecvDataSizeTotal += sizeIncreased;
}

#ifdef WIN32

WSABUF* Session::GetRecvWSABUF()
{
	m_RecvWSABUF.buf = GetRecvBuffer()->GetEnd();
	m_RecvWSABUF.len = static_cast<u_long>( GetRecvBuffer()->GetFreeSpaceSize() );
	return &m_RecvWSABUF;
}

WSABUF* Session::GetSendWSABUF()
{
	m_SendWSABUF.buf = GetSendBuffer()->GetBegin();
	m_SendWSABUF.len = static_cast<u_long>( GetSendBuffer()->GetDataSize() );
	return &m_SendWSABUF;
}

AsyncCompleteOperation* Session::GetRecvCompleteOperation()
{
	m_RecvCompleteOperation.hEvent = 0;
	m_RecvCompleteOperation.Internal = 0;
	m_RecvCompleteOperation.InternalHigh = 0;
	m_RecvCompleteOperation.Offset = 0;
	m_RecvCompleteOperation.OffsetHigh = 0;

	return &m_RecvCompleteOperation;
}

AsyncCompleteOperation* Session::GetSendCompleteOperation()
{
	m_SendCompleteOperation.hEvent = 0;
	m_SendCompleteOperation.Internal = 0;
	m_SendCompleteOperation.InternalHigh = 0;
	m_SendCompleteOperation.Offset = 0;
	m_SendCompleteOperation.OffsetHigh = 0;

	return &m_SendCompleteOperation;
}

bool Session::PostRecvOperation()
{
	unsigned long dwNumTransferred = 0;
	DWORD dwFlags = 0;
	int ret = WSARecv(
		GetSock(),
		GetRecvWSABUF(),
		1,
		&dwNumTransferred,
		&dwFlags,
		GetRecvCompleteOperation(),
		NULL
		);
	if ( 0 != ret )
	{
		int errNum = WSAGetLastError();
		if ( WSA_IO_PENDING != errNum && s_SessionMgr.SessionActive( GetID() ) )
		{
			LOG_ERROR( logger, __FUNCTION__ << " failed WSARecv(), " << FormatErrorMessage( errNum ) );
			return false;
		}
	}

	return true;
}

bool Session::PostSendOperation()
{
	m_SendingDataSize = static_cast<u_long>( GetSendBuffer()->GetDataSize() );

	DWORD dwNumTransferred = 0;
	int ret = WSASend(
		GetSock(), 
		GetSendWSABUF(), 
		1,
		&dwNumTransferred,
		0,
		GetSendCompleteOperation(),
		NULL
		);
	if ( SOCKET_ERROR == ret )
	{
		int errNum = WSAGetLastError();
		if ( WSA_IO_PENDING != errNum )
		{
			LOG_ERROR( logger, __FUNCTION__	<< " failed WSASend(), " << FormatErrorMessage( errNum ) );

			return false;
		}
	}

	return true;
}

#elif defined(LINUX)

AsyncCompleteOperation* Session::GetAsyncCompleteOperation()
{
	return &m_SessionCompleteOperation;
}

#endif

void Session::ExtractMsg()
{
	while(true)
	{
		if( static_cast<size_t>(m_RecvBuffer.GetEnd() - m_RecvBuffer.GetBegin()) < MSG_HEADER_SIZE )
		{
			break;
		}

		short header = *( (short*)(m_RecvBuffer.GetBegin()) );
		//check if there is a complete msg to extract
		if( m_RecvBuffer.GetEnd() - m_RecvBuffer.GetBegin() - MSG_HEADER_SIZE < static_cast<size_t>(header) )
		{
			break;
		}

		//allocate msg
		Msg* pMsg = s_MsgPool.AllocateMsg( header );
		if( NULL == pMsg )
		{
			LOG_WARN( logger, __FUNCTION__ << " failed to allocate msg from s_MsgPool, size is " << header );
			break;
		}
		pMsg->m_Type = MSG_RECV;
		pMsg->m_SessionID = GetID();
		pMsg->m_pSession = this;
		memcpy( pMsg->m_pData, m_RecvBuffer.GetBegin() + MSG_HEADER_SIZE, header );

		//set recv start pos
		m_RecvBuffer.IncreaseBegin( MSG_HEADER_SIZE + header );

		//insert msg to recv queue
		if( !s_MsgQueue.InsertMsg( pMsg ) )
		{
			LOG_ERROR( logger, __FUNCTION__ << " failed to insert msg to s_MsgQueue!" );
			break;
		}
	}
}

bool Session::SendMsg( int sessionID, void* pData, size_t size )
{
	//check if the session is active
	//must check before the mutex, because the session may have been deleted,
	// then, use session's mutex will evoke an error
	if( !s_SessionMgr.SessionActive( sessionID ) )
	{
		LOG_INFO( logger, "first check, session " << sessionID << " is not active" );
		return false;
	}

	MutexGuard guard( GetSendLock() );

	//double check
	//if other thread delete this session between first check and GetSendLock,
	// we check this status by the second check.
	if( !s_SessionMgr.SessionActive( sessionID ) )
	{
		LOG_INFO( logger, "second check, session " << sessionID << " is not active" );
		return false;
	}

	//check if free space is enough
	if( GetSendBuffer()->GetFreeSpaceSize() < size )
	{
		return false;
	}

	//copy msg
	memcpy( GetSendBuffer()->GetEnd(), &size, MSG_HEADER_SIZE );
	m_SendBuffer.IncreaseEnd( MSG_HEADER_SIZE );
	memcpy( GetSendBuffer()->GetEnd(), pData, size );
	m_SendBuffer.IncreaseEnd( size );
 

#ifdef WIN32
	if ( !Sending() )
	{
		PostSendOperation();

		SetSending( true );
	}	
	
#elif defined(LINUX)
	guard.UnLock();
	m_SessionCompleteOperation.Complete( ASYNC_OPERATION_SEND );

#endif

	return true;
}

void Session::Disconnect()
{
#ifdef WIN32
	shutdown( GetSock(), SD_BOTH );
#elif defined(LINUX)
	m_SessionCompleteOperation.Complete( ASYNC_OPERATION_CLOSE );
#endif
}

} //namespace Crux

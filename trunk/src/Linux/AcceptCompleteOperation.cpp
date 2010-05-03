#include "Linux/AcceptCompleteOperation.h"
#include "Session.h"
#include "SessionMgr.h"
#include "MsgQueue.h"
#include "Dispatcher.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#include "Logger.h"
#include "HelperFunction.h"

namespace Crux
{

AcceptCompleteOperation::AcceptCompleteOperation(AcceptType acceptType, const std::string& ip, int port, int backlog )
: m_AcceptType( acceptType ), m_IP( ip ), m_Port( port )
{
	if( !Bind() )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to bind port!" );
		return;
	}
	if( !Listen( backlog ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to listen on port!" );
		return;
	}
	if( !s_Dispatcher.RegisterSocket( m_ListenSock, this ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to register listen socket!" );
		return;
	}
	LOG_INFO( logger,  "server is listening on port: " << m_Port );
}

void AcceptCompleteOperation::Complete( AsyncOperationType )
{
	//accept connection
	while(true)
	{
		sockaddr_in addr;
		socklen_t addrLen = sizeof(addr);
		int newSock= accept( m_ListenSock, (sockaddr *)&addr, &addrLen );
		if( -1 == newSock )
		{
			int errNum = errno;
			if( EWOULDBLOCK == errNum )
			{
				return;
			}

			LOG_ERROR( logger, __FUNCTION__ << " failed to accept, " << FormatErrorMessage( errNum ) );
			return;
		}

		char ip[INET_ADDRSTRLEN];
		if( NULL == inet_ntop( AF_INET, &addr.sin_addr, ip, sizeof(ip) ) )
		{
			LOG_ERROR( logger, __FUNCTION__ << " failed to get ip in inet_ntop()!" );
		}

		//add this session
		Session* pSession = s_SessionMgr.CreateSession( newSock, ip, addr.sin_port );
		if( NULL == pSession )
		{
			LOG_ERROR( logger, __FUNCTION__ << " failed to create session!" );
		}

		//notify accept
		MsgType type;
		if ( ACCEPT_EXTERNAL == m_AcceptType )
		{
			type = MSG_ACCEPT_EXTERNAL;
		}
		else
		{
			type = MSG_ACCEPT_INTERNAL;
		}
		s_MsgQueue.InsertSimpleMsg( type, pSession->GetID(), pSession );

		//add sock to epoll set
		if( !s_Dispatcher.RegisterSocket( newSock, pSession->GetAsyncCompleteOperation() ) )
		{
			LOG_ERROR( logger, __FUNCTION__ << " failed to register sock!" );
			return;
		}
	}
}

bool AcceptCompleteOperation::Bind()
{
	m_ListenSock = socket( AF_INET, SOCK_STREAM, 0 );
	if ( -1 == m_ListenSock )
	{
		return false;
	}

	//set SO_REUSEADDR for bind
	int value = 1;
	if ( -1 == setsockopt( m_ListenSock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value) ) )
	{
		return false;
	}
	if ( -1 == setsockopt( m_ListenSock, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value) ) )
	{
		return false;
	}

	value = 4096;
	if ( -1 == setsockopt( m_ListenSock, SOL_SOCKET, SO_RCVBUF, &value, sizeof(value) ) )
	{
		return false;
	}
	if ( -1 == setsockopt( m_ListenSock, SOL_SOCKET, SO_SNDBUF, &value, sizeof(value) ) )
	{
		return false;
	}
	value = 1;
	if ( -1 == setsockopt( m_ListenSock, SOL_SOCKET, SO_KEEPALIVE, &value, sizeof(value) ) )
	{
		return false;
	}

	sockaddr_in serverAddr;
	bzero( &serverAddr, sizeof(serverAddr) );
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons( m_Port );
	serverAddr.sin_addr.s_addr = htonl( INADDR_ANY );

	if ( -1 == bind( m_ListenSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr) ) )
	{
		return false;
	}

	return true;
}

bool AcceptCompleteOperation::Listen(int backlog )
{
	if( -1 == listen( m_ListenSock, backlog ) )
	{
		return false;
	}

	return true;
}

} //namespace Crux


#include "Win/AcceptHandler.h"
#include "Dispatcher.h"
#include <Mswsock.h>
#include "MsgQueue.h"
#include "SessionMgr.h"
#include "Logger.h"
#include "HelperFunction.h"
#include "Session.h"
#include "Win/AsyncCompleteOperation.h"

namespace Crux
{

AcceptHandler::AcceptHandler(
	AcceptType acceptType, 
	const std::string& ip, 
	int port, 
	int backlog 
	)
: m_AcceptType( acceptType ),
m_IP( ip ), m_Port( port ), m_Backlog( backlog )
{
	m_pAsyncCompleteOperation = new AsyncCompleteOperation( this, this );

	if ( !Bind( ip, port ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to bind port!" );
		return;
	}

	if ( !Listen( backlog ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to listen on port!" );
		return;
	}

	//register to iocp
	if( !s_Dispatcher.RegisterSocket( m_ListenSock, 0 ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to register listen socket!" );
		return;
	}
	LOG_INFO( logger,  "server is listening on port: " << port );

	//post first accept
	if ( !Accept() )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed Accept()!" );
		return;
	}

}

AcceptHandler::~AcceptHandler()
{
	delete m_pAsyncCompleteOperation;
}

void AcceptHandler::Complete(Session*, unsigned long )
{
	//inherit listen socket's properties
 	if( SOCKET_ERROR == setsockopt(
		m_AcceptSock, 
 		SOL_SOCKET, 
 		SO_UPDATE_ACCEPT_CONTEXT, 
 		(char *)&m_ListenSock, 
 		sizeof(m_ListenSock)
		)
		)
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed setsockopt(), " << FormatErrorMessage( WSAGetLastError() ) );
		return;
	}

	sockaddr_in addr;
	int addrLen = sizeof(addr);
	if( SOCKET_ERROR == getsockname( m_AcceptSock, (sockaddr*)&addr, &addrLen ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed getsockname(), " << FormatErrorMessage( WSAGetLastError() ) );
		return;
	}

	Session* pSession = s_SessionMgr.CreateSession( m_AcceptSock, inet_ntoa(addr.sin_addr), addr.sin_port );
	if ( NULL == pSession )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to create session!" );
		return;
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

	//add to completion port
	if( false == s_Dispatcher.RegisterSocket( m_AcceptSock, pSession ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to associate socket with completion port!" );
		return;
	}

	pSession->PostRecvOperation();

	//post another accept
	if ( !Accept() )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed Accept()!" );
		return;
	}

}

bool AcceptHandler::Bind(const std::string& ip, int port )
{
	m_ListenSock = socket( AF_INET, SOCK_STREAM, 0 );
	if ( INVALID_SOCKET == m_ListenSock )
	{
		return false;
	}

	//set SO_REUSEADDR for bind
	BOOL enabled = TRUE;
	if ( SOCKET_ERROR == setsockopt( m_ListenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&enabled, sizeof(enabled) ) )
	{
		return false;
	}
	if ( SO_ERROR == setsockopt( m_ListenSock, SOL_SOCKET, SO_KEEPALIVE, (char*)&enabled, sizeof(enabled) ) )
	{
		return false;
	}
	if ( SOL_SOCKET == setsockopt( m_ListenSock, IPPROTO_TCP, TCP_NODELAY, (char*)&enabled, sizeof(enabled) ) )
	{
		return false;
	}

	int value = 4096;
	if( SOCKET_ERROR == setsockopt( m_ListenSock, SOL_SOCKET, SO_RCVBUF, (char*)&value, sizeof(value) ) )
	{
		return false;
	}
	if( setsockopt( m_ListenSock, SOL_SOCKET, SO_SNDBUF, (char*)&value, sizeof(value) ) )
	{
		return false;
	}

	sockaddr_in serverAddr;
	memset( &serverAddr, 0, sizeof(serverAddr) );
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons( port );
	serverAddr.sin_addr.s_addr = inet_addr( ip.c_str() );

	if ( SOCKET_ERROR == bind( m_ListenSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr) ) )
	{
		return false;
	}

	return true;
}

bool AcceptHandler::Listen(int backlog )
{
	if( SOCKET_ERROR == listen( m_ListenSock, backlog ) )
	{
		return false;
	}

	return true;
}

bool AcceptHandler::Accept()
{
	m_AcceptSock = socket( AF_INET, SOCK_STREAM, 0 );
	if ( INVALID_SOCKET == m_AcceptSock )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to create socket, " << FormatErrorMessage( WSAGetLastError() ) );
		return false;
	}

	DWORD bytesReceived = 0;
	BOOL ret = AcceptEx(
		m_ListenSock,
		m_AcceptSock,
		m_AcceptBuffer,
		0,
		sizeof( sockaddr_in ) + 16,
		sizeof( sockaddr_in ) + 16,
		&bytesReceived,
		m_pAsyncCompleteOperation
		);
	if ( FALSE == ret )
	{
		int errNum = WSAGetLastError();
		if ( WSA_IO_PENDING != errNum )
		{
			LOG_ERROR( logger, __FUNCTION__ << " failed AcceptEx(), " << FormatErrorMessage( errNum ) << errNum );
			return false;
		}
	}

	return true;
}

} //namespace Crux

#include "ClientMsgHandler.h"

void ClientMsgHandler::OnConnect(Crux::Session* pSession, void* pData, size_t size )
{
	LOG_INFO( logger, "connected to the server" );

	char pNewData[] = "Hello Server";
	size_t newSize = 12;

	if( !s_TCPClient.SendMsg( pSession->GetID(), pNewData, newSize ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to send first msg!" );
		return;
	}
}

void ClientMsgHandler::OnReceive(Crux::Session* pSession, void* pData, size_t size )
{
	if( 0 != memcmp( "Hello Client", pData, 12 ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed, data is not \"Hello Client\"." );
	}
	else
	{
		LOG_INFO( logger, "receive \"Hello Client\" from server." );
	}

	s_TCPClient.Disconnect();
}

void ClientMsgHandler::OnClose( int sessionID, void* pData, size_t size )
{
	LOG_INFO( logger, "close server session, id: " << sessionID );
}

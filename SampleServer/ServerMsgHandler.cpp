#include "ServerMsgHandler.h"

using namespace Crux;

void ServerMsgHandler::OnConnect(Session* pSession, void* pData, size_t size )
{

}

void ServerMsgHandler::OnAccept(Session* pSession, void* pData, size_t size )
{
	LOG_INFO( logger, "accept connection, id: " << pSession->GetID()
		<< ", sock: " << pSession->GetSock()
		<< ", ip: " << pSession->GetIP().c_str()
		<< ", port: " << pSession->GetPort()
		);
}

void ServerMsgHandler::OnReceive(Session* pSession, void* pData, size_t size )
{
	using namespace Crux;

	if( 0 != memcmp( "Hello Server", pData, 12 ) )
	{
		LOG_INFO( logger, "unknow message from client" );
		return;
	}
	else
	{
		LOG_INFO( logger, "receive \"Hello Server\" from client." );

		char pNewData[] = "Hello Client";
		size_t size = 12;

		if ( !pSession->SendMsg( pSession->GetID(), pNewData, size ) )
		{
			LOG_ERROR( logger, __FUNCTION__ << " failed to send msg!" );
		}
	}
}

void ServerMsgHandler::OnClose( int sessionID, void* pData, size_t size )
{
	LOG_INFO( logger, "close connection, id: " << sessionID );
}


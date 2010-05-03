#ifndef SERVER_MSG_HANDLER_H
#define SERVER_MSG_HANDLER_H

#include "NetServer.h"

class ServerMsgHandler : public Crux::MsgHandler
{
public:
	virtual void OnConnect( Crux::Session* pSession, void* pData, size_t size );

	virtual void OnAccept( Crux::Session* pSession, void* pData, size_t size );

	virtual void OnReceive( Crux::Session* pSession, void* pData, size_t size );

	virtual void OnClose( int sessionID, void* pData, size_t size );

};

#endif	//SERVER_MSG_HANDLER_H


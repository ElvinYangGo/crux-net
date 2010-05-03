#ifndef CLIENT_MSG_HANDLER_H
#define CLIENT_MSG_HANDLER_H

#include "NetClient.h"

class ClientMsgHandler : public Crux::MsgHandler
{
public:
	virtual void OnConnect( Crux::Session* pSession, void* pData, size_t size );

	virtual void OnReceive( Crux::Session* pSession, void* pData, size_t size );

	virtual void OnClose( int sessionID, void* pData, size_t size );

};

#endif	


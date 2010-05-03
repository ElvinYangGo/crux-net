#ifndef MSG_HANDLER_H
#define MSG_HANDLER_H

namespace Crux
{

class Session;

class MsgHandler
{
public:
	virtual ~MsgHandler() {}

public:
	virtual void OnConnect( Session* pSession, void* pData, size_t size ) {}

	virtual void OnAccept( Session* pSession, void* pData, size_t size ) {}

	virtual void OnReceive( Session* pSession, void* pData, size_t size ) {}

	//here, Session is deleted, we can't use Session any more
	virtual void OnClose( int sessionID, void* pData, size_t size ) {}
};

} //namespace Crux

#endif	//MSG_HANDLER_H

#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include "Singleton.h"
#include "Queue.h"
#include "ConstConfigData.h"
#include "Msg.h"

namespace Crux
{

class Mutex;
struct Msg;

/**
 * msg queue, to storage msg that received from client
 */
class MsgQueue
{
	friend class Singleton<MsgQueue>;
private:
	/*
	 * allocate space for received msgs
	 */
	MsgQueue();

	~MsgQueue();

public:
	bool InsertMsg( Msg* pMsg );
	Msg* RemoveMsg();

	bool InsertSimpleMsg( MsgType type, int sessionID, Session* pSession );

private:
	Mutex* m_pMutex;

private:
	Queue<Msg*> m_Msgs;

private:
	static const size_t MSG_QUEUE_SIZE;
};

#define s_MsgQueue Crux::Singleton<Crux::MsgQueue>::Instance()

} //namespace Crux

#endif //MSG_QUEUE_H

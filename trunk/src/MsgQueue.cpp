#include "MsgQueue.h"
#include "Mutex.h"
#include "Semaph.h"
#include "MutexGuard.h"
#include "MsgPool.h"
#include "Msg.h"

namespace Crux
{

const size_t MsgQueue::MSG_QUEUE_SIZE = cofig_MSG_QUEUE_SIZE;

MsgQueue::MsgQueue()
: m_pMutex( new Mutex ), m_Msgs(MSG_QUEUE_SIZE)
{}

MsgQueue::~MsgQueue()
{
	delete m_pMutex;
}

bool MsgQueue::InsertMsg( Msg* pMsg )
{
	MutexGuard guard( m_pMutex );
	
	bool ret = m_Msgs.PushBack( pMsg );

	guard.UnLock();

	g_pMsgQueueSemaph->Post();

	return ret;
}

Msg* MsgQueue::RemoveMsg()
{
	MutexGuard guard( m_pMutex );

	Msg* pMsg = NULL;
	if( false == m_Msgs.Front( pMsg ) )
	{
		return NULL;
	}

	m_Msgs.PopFront();

	return pMsg;
}

bool MsgQueue::InsertSimpleMsg(MsgType type, int sessionID, Session* pSession )
{
	//notify close
	Msg* pMsg = s_MsgPool.AllocateMsg( 1 );
	if( NULL == pMsg )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to allocate msg in s_MsgPool, size is 1" );

		return false;
	}
	pMsg->m_Type = type;
	pMsg->m_SessionID = sessionID;
	pMsg->m_pSession = pSession;
	if( false == s_MsgQueue.InsertMsg( pMsg ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to insert msg into s_MsgQueue!" );

		return false;
	}

	return true;
}

} //namespace Crux

#include "NetServer.h"

static bool running = false;

void ShutdownNetServer()
{
	running = false;
	s_MsgQueue.InsertSimpleMsg( Crux::MSG_SHUTDOWN, 0, 0 );
}

void NetLoop( INIT_NET init, Crux::MsgHandler* pMsgHandler )
{
	if( !init() )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to init net!" );
		return;
	}

	running = true;

	int recvTimes = 0;
	while( running )
	{
		Crux::g_pMsgQueueSemaph->Wait();

		//process msg
		Crux::Msg* pMsg = s_MsgQueue.RemoveMsg();

		if( NULL == pMsg )
		{
			LOG_ERROR( logger, __FUNCTION__ << " failed to get msg from s_MsgQueue!" );
			continue;
		}

		if( Crux::MSG_SHUTDOWN == pMsg->m_Type )
		{
			break;
		}

		switch( pMsg->m_Type )
		{
		case Crux::MSG_ACCEPT_EXTERNAL:
			pMsgHandler->OnAccept( pMsg->m_pSession, pMsg->m_pData, pMsg->m_Size );
			break;
		case Crux::MSG_CONNECT:
			pMsgHandler->OnConnect( pMsg->m_pSession, pMsg->m_pData, pMsg->m_Size );
			break;
		case Crux::MSG_CLOSE:
			pMsgHandler->OnClose( pMsg->m_SessionID, pMsg->m_pData, pMsg->m_Size );
			break;
		case Crux::MSG_RECV:
			++recvTimes;
			pMsgHandler->OnReceive( pMsg->m_pSession, pMsg->m_pData, pMsg->m_Size );
			break;
		default:
			LOG_ERROR( logger, __FUNCTION__ << " failed, pMsg->m_Type is invalid!" );
			break;
		}

		if ( pMsg->m_Type == Crux::MSG_RECV 
			&& 0 != recvTimes 
			&& 0 == recvTimes % (500*1000)
			)
		{
			//pMsg->m_pSession->Disconnect();
			//ShutdownNetServer();
		}

		if( 0 != recvTimes && 0 == recvTimes % (100*1000) )
		{
			LOG_INFO( logger, "recv times is " << recvTimes );
		}

		s_MsgPool.DeallocateMsg( pMsg );
	}

	s_TCPServer.Shutdown();
}

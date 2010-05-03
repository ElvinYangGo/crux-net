#include "NetClient.h"

void ShutdownNetClient()
{
	s_TCPClient.Shutdown();

	//sleep 3 seconds
#ifdef WIN32
	Sleep( 3000 );
#elif defined(LINUX)
	sleep( 3 );
#endif
}

void HandleNet( Crux::MsgHandler* pMsgHandler )
{
	while( true )
	{
#ifdef WIN32
		if( WAIT_TIMEOUT == Crux::g_pMsgQueueSemaph->TryWait() )
		{
			return;
		}
#elif defined(LINUX)
		if( -1 == Crux::g_pMsgQueueSemaph->TryWait() )
		{
			return;
		}
#endif

		//process msg
		Crux::Msg* pMsg = s_MsgQueue.RemoveMsg();
		if( NULL == pMsg )
		{
			LOG_ERROR( logger, __FUNCTION__ << " failed to get msg from s_MsgQueue!" );
			continue;
		}

		switch( pMsg->m_Type )
		{
		case Crux::MSG_CONNECT:
			s_TCPClient.SetConnected( true );
			s_TCPClient.SetSession( pMsg->m_pSession );
			pMsgHandler->OnConnect( pMsg->m_pSession, pMsg->m_pData, pMsg->m_Size );
			break;
		case Crux::MSG_CLOSE:
			s_TCPClient.SetConnected( false );
			pMsgHandler->OnClose( pMsg->m_SessionID, pMsg->m_pData, pMsg->m_Size );
			break;
		case Crux::MSG_RECV:
			pMsgHandler->OnReceive( pMsg->m_pSession, pMsg->m_pData, pMsg->m_Size );
			break;
		default:
			LOG_ERROR( logger, __FUNCTION__ << " failed, pMsg->m_Type is invalid!" );
			break;
		}

		s_MsgPool.DeallocateMsg( pMsg );
	}
}

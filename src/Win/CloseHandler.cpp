#include "Win/CloseHandler.h"
#include "MutexGuard.h"
#include "SessionMgr.h"
#include "MsgQueue.h"
#include "Session.h"
#include "Logger.h"
#include "HelperFunction.h"

namespace Crux
{

void CloseHandler::Complete( Session* pSession, unsigned long bytesTransferred )
{
	MutexGuard recvGuard( pSession->GetRecvLock() );
	MutexGuard sendGuard( pSession->GetSendLock() );

	s_SessionMgr.SetSessionActive( pSession->GetID(), false );

	if( pSession->GetRecvDataSizeTotal() != pSession->GetSendDataSizeTotal() )
	{
		size_t recvSize = pSession->GetRecvDataSizeTotal();
		size_t sendSize = pSession->GetSendDataSizeTotal();
		LOG_ERROR( logger, __FUNCTION__ << "recv data total: " << recvSize << "\tsend data total: " << sendSize );
	}

	//close socket
	if( SOCKET_ERROR == closesocket( pSession->GetSock() ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed closesocket(), " << FormatErrorMessage( WSAGetLastError() ) );
		return;
	}

	//notify close
	s_MsgQueue.InsertSimpleMsg( MSG_CLOSE, pSession->GetID(), NULL );

	s_SessionMgr.DestroySession( pSession );
}

}

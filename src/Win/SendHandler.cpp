#include "Win/SendHandler.h"
#include "MutexGuard.h"
#include "Session.h"

namespace Crux
{

void SendHandler::Complete(Session* pSession, unsigned long bytesTransferred )
{
	MutexGuard guard( pSession->GetSendLock() );

	pSession->GetSendBuffer()->IncreaseBegin( bytesTransferred );
	pSession->GetSendBuffer()->MoveData();

	pSession->IncreaseSendDataSizeTotal( bytesTransferred );

	if ( 0 != pSession->GetSendBuffer()->GetDataSize() )
	{
		pSession->PostSendOperation();
	}
	else
	{
		pSession->SetSending( false );
	}
}

}

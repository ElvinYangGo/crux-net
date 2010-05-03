#include "Win/RecvHandler.h"
#include "MutexGuard.h"
#include "Session.h"

namespace Crux
{

void RecvHandler::Complete( Session* pSession, unsigned long bytesTransferred )
{
	MutexGuard guard( pSession->GetRecvLock() );

	pSession->GetRecvBuffer()->IncreaseEnd( bytesTransferred );
	pSession->IncreaseRecvDataSizeTotal( bytesTransferred );

	pSession->ExtractMsg();
	pSession->GetRecvBuffer()->MoveData();

	pSession->PostRecvOperation();
}

}
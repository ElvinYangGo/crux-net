#include "WorkerThread.h"
#include <WinSock2.h>
#include "Session.h"
#include "Dispatcher.h"
#include "Logger.h"
#include "HelperFunction.h"

namespace Crux
{

WorkerThread::WorkerThread()
{
	if( !Start() )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to start worker thread!" );
	}
}

void WorkerThread::Run()
{
	BOOL ret = FALSE;
	DWORD bytesTransferred = 0;
	AsyncCompleteOperation* pCompleteOperation = NULL;
	void* pCompletionKey = NULL;

	while( Runnable() )
	{
		ret = GetQueuedCompletionStatus(
			s_Dispatcher.GetIOCPHandle(),
			&bytesTransferred,
			(PULONG_PTR)&pCompletionKey,
			(LPOVERLAPPED*)&pCompleteOperation,
			INFINITE
			);

		if ( FALSE == ret )
		{
			LOG_ERROR( logger, __FUNCTION__ << " failed GetQueuedCompletionStatus(), " << FormatErrorMessage( WSAGetLastError() ) );

			if ( NULL != pCompletionKey )
			{
				pCompleteOperation->Close( static_cast<Session*>( pCompletionKey ) );
			}
			continue;
		}
		
 		if ( 0 == bytesTransferred )
 		{
			//if NULL == pCompletionKey and NULL == pCompleteOperation,
			//	it means worker thread is being stopped.
			if ( !Runnable() && NULL == pCompletionKey && NULL == pCompleteOperation )
			{
				break;				
			}

			//if NULL != pCompletionKey, the pCompletionKey points to a Session
			//if NULL == pCompletionKey, the asynchronous operation is an accept complete operation, 
			//	we should use Complete operation to accept this new session, not Close
			if ( NULL != pCompletionKey )
			{
				pCompleteOperation->Close( static_cast<Session*>( pCompletionKey ) );
				continue;
			}
 		}

		pCompleteOperation->Complete( static_cast<Session*>( pCompletionKey ) );
	}

	LOG_INFO( logger, "exit thread" );
}

} //namespace Crux

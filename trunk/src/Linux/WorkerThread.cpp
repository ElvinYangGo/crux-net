#include "WorkerThread.h"
#include "Linux/AsyncOperationQueue.h"
#include "Linux/AsyncOperationType.h"
#include "Linux/AsyncCompleteOperation.h"
#include "Semaph.h"
#include "Logger.h"
#include "Thread.h"

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
	while( Runnable() )
	{
		g_pAsyncOperationQueueSemaph->Wait();

		AsyncOperationData asyncOperationData = s_AsyncOperationQueue.RemoveAsyncOperation();

		if( !Runnable() && ASYNC_OPERATION_SHUTDOWN == asyncOperationData.m_AsyncOperationType )
		{
			break;
		}

		asyncOperationData.m_pAsyncCompleteOperation->Complete( asyncOperationData.m_AsyncOperationType );
	}

	LOG_INFO( logger, "exit thread" );
}

} //namespace Crux

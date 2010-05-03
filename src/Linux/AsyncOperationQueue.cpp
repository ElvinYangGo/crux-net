#include "Linux/AsyncOperationQueue.h"
#include "Mutex.h"
#include "Semaph.h"
#include "MutexGuard.h"

namespace Crux
{

const size_t AsyncOperationQueue::ASYNC_OPERATION_QUEUE_SIZE;

AsyncOperationQueue::AsyncOperationQueue()
: m_pMutex( new Mutex ), m_AsyncOperations( ASYNC_OPERATION_QUEUE_SIZE )
{}

AsyncOperationQueue::~AsyncOperationQueue()
{
	delete m_pMutex;
}

bool AsyncOperationQueue::InsertAsyncOperation(AsyncCompleteOperation* pAsyncCompleteOperation, AsyncOperationType asyncOperationType )
{
	MutexGuard guard( m_pMutex );

	AsyncOperationData asyncOperationData( pAsyncCompleteOperation, asyncOperationType );
	bool ret = m_AsyncOperations.PushBack( asyncOperationData );
	guard.UnLock();

	g_pAsyncOperationQueueSemaph->Post();

	return ret;
}

AsyncOperationData AsyncOperationQueue::RemoveAsyncOperation()
{
	MutexGuard guard( m_pMutex );

	AsyncOperationData asyncOperationData;
	if( !m_AsyncOperations.Front( asyncOperationData ) )
	{
		return asyncOperationData;
	}

	m_AsyncOperations.PopFront();

	return asyncOperationData;
}

} //namespace Crux

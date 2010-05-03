#ifndef ASYNC_OPERATION_QUEUE_H
#define ASYNC_OPERATION_QUEUE_H

#include "Singleton.h"
#include "Queue.h"
#include "ConstConfigData.h"
#include "Linux/AsyncOperationType.h"

namespace Crux
{

class Mutex;
class AsyncCompleteOperation;

struct AsyncOperationData
{
	AsyncOperationData( AsyncCompleteOperation* pAsyncCompletionOperation, AsyncOperationType asyncOperationType )
	: m_pAsyncCompleteOperation(pAsyncCompletionOperation), m_AsyncOperationType(asyncOperationType)
	{}
	AsyncOperationData() {}

	AsyncCompleteOperation* m_pAsyncCompleteOperation;
	AsyncOperationType m_AsyncOperationType;
};

class AsyncOperationQueue
{
	friend class Singleton<AsyncOperationQueue>;
private:
	AsyncOperationQueue();
	~AsyncOperationQueue();

public:
	bool InsertAsyncOperation( AsyncCompleteOperation* pAsyncCompleteOperation, AsyncOperationType asyncOperationType );
	AsyncOperationData RemoveAsyncOperation();

private:
	Mutex* m_pMutex;

private:
	Queue<AsyncOperationData> m_AsyncOperations;

private:
	static const size_t ASYNC_OPERATION_QUEUE_SIZE = cofig_ASYNC_OPERATION_QUEUE_SIZE;
};

#define s_AsyncOperationQueue Crux::Singleton<Crux::AsyncOperationQueue>::Instance()

} //namespace Crux

#endif //ASYNC_OPERATION_QUEUE_H

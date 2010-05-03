#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include "Thread.h"

namespace Crux
{

class WorkerThread : public Thread
{
public:
	WorkerThread();

public:
	virtual void Run();
};
		
} //namespace Crux

#endif //WORKER_THREAD_H

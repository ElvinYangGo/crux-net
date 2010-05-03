#ifndef WORKER_THREAD_SELECT_H
#define WORKER_THREAD_SELECT_H

#ifdef WIN32

#include "Win/WorkerThread.h"

#elif defined(LINUX)

#include "Linux/WorkerThread.h"

#endif

#endif	//WORKER_THREAD_SELECT_H


#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

namespace Crux
{

/**
 * A simple thread wrapper
 */

class Thread
{
public:
	Thread();
	virtual ~Thread() {}

public:
	bool Start( bool cancellable = false );

	virtual void Run() = 0;
	virtual void Shutdown();

	bool Runnable() const;
	void SetRunnable( bool runnable );

	pthread_t CurrentThread() const;

private:
	static void* ThreadFunction( void* arg );

private:
	pthread_t m_ThreadID;
	bool m_Runnable;
};


} //namespace Crux

#endif //THREAD_H

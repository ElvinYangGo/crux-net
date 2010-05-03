#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

namespace Crux
{

class Mutex
{
public:
	Mutex();
	~Mutex();

private:
	Mutex( const Mutex& );
	Mutex& operator=( const Mutex& );

public:
	void Lock();
	void UnLock();

private:
	pthread_mutex_t m_ThreadMutex;
};

} //namespace Crux

#endif //MUTEX_H

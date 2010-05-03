#ifndef MUTEX_H
#define MUTEX_H

#include <Windows.h>

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
	CRITICAL_SECTION m_CriticalSection;
};

} //namespace Crux

#endif //MUTEX_H

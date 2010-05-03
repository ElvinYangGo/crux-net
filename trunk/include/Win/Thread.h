#ifndef THREAD_H
#define THREAD_H

#include <Windows.h>

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
	bool Start();

	virtual void Run() = 0;
	virtual void Shutdown();

	bool Runnable() const;
	void SetRunnable( bool runnable );

	HANDLE CurrentThread() const;

private:
	static unsigned int __stdcall ThreadFunction( void* arg );

private:
	HANDLE m_ThreadID;
	bool m_Runnable;
};


} //namespace Crux

#endif //THREAD_H

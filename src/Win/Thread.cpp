#include "Thread.h"
#include <process.h>

namespace Crux
{

Thread::Thread()
: m_Runnable( true )
{}

bool Thread::Start()
{
	HANDLE m_ThreadID = (HANDLE)_beginthreadex(
		NULL,
		0,
		ThreadFunction,
		this,
		0,
		NULL
		);
	if ( NULL != m_ThreadID )
	{
		return true;
	}

	return false;
}

void Thread::Shutdown()
{
	m_Runnable = false;
}

bool Thread::Runnable() const
{
	return m_Runnable;
}

HANDLE Thread::CurrentThread() const
{
	return m_ThreadID;
}

unsigned int Thread::ThreadFunction( void* arg )
{
	Thread* pThread = static_cast<Thread*>( arg );
	pThread->Run();

	return 0;
}

} //namespace Crux

#include "Thread.h"

namespace Crux
{

Thread::Thread()
: m_Runnable( true )
{}

bool Thread::Start( bool cancellable )
{
	int ret = pthread_create( 
			&m_ThreadID, 
			NULL,
			ThreadFunction,
			this	
			);
	if( 0 != ret )
	{
		return false;
	}

	if( cancellable )
	{
		if( 0 != pthread_setcancelstate( PTHREAD_CANCEL_ENABLE, NULL ) )
		{
			return false;
		}
		if( 0 != pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, NULL ) )
		{
			return false;
		}
	}

	return true;
}

void Thread::Shutdown()
{
	m_Runnable = false;
}

bool Thread::Runnable() const
{
	return m_Runnable;
}

pthread_t Thread::CurrentThread() const
{
	return m_ThreadID;
}

void* Thread::ThreadFunction( void* arg )
{
	int ret = pthread_detach( pthread_self() );
	if( 0 != ret )
	{
		return NULL;
	}

	Thread* pThread = static_cast<Thread*>( arg );
	pThread->Run();

	pthread_exit( NULL );

	return NULL;
}

} //namespace Crux

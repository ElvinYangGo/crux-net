#include "Linux/Mutex.h"

namespace Crux
{

Mutex::Mutex()
{
	pthread_mutex_init( &m_ThreadMutex, NULL );
}

Mutex::~Mutex()
{
	pthread_mutex_destroy( &m_ThreadMutex );
}

void Mutex::Lock()
{
	pthread_mutex_lock( &m_ThreadMutex );
}

void Mutex::UnLock()
{
	pthread_mutex_unlock( &m_ThreadMutex );
}

}

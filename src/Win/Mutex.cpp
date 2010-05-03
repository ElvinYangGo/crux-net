#include "Mutex.h"

namespace Crux
{

Mutex::Mutex()
{
	InitializeCriticalSection( &m_CriticalSection );
}

Mutex::~Mutex()
{
	DeleteCriticalSection( &m_CriticalSection );
}

void Mutex::Lock()
{
	EnterCriticalSection( &m_CriticalSection );
}

void Mutex::UnLock()
{
	LeaveCriticalSection( &m_CriticalSection );
}

}

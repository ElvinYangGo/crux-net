#ifndef MUTEX_GUARD_H
#define MUTEX_GUARD_H

#include "Mutex.h"

namespace Crux
{

class MutexGuard
{
public:
	MutexGuard( Mutex* pMutex );
	~MutexGuard();

private:
	MutexGuard( const MutexGuard& ) {}
	void operator=( const MutexGuard& ) {}

public:
	void Lock();
	void UnLock();

private:
	Mutex* m_pMutex;
	bool m_Owner;
};

inline
MutexGuard::MutexGuard( Mutex* pMutex )
: m_pMutex( pMutex ), m_Owner(false)
{
	Lock();
}

inline
MutexGuard::~MutexGuard()
{
	UnLock();
}

inline
void MutexGuard::Lock()
{
	m_pMutex->Lock();
	m_Owner = true;
}

inline
void MutexGuard::UnLock()
{
	if( m_Owner )
	{
		m_Owner = false;
		m_pMutex->UnLock();
	}
}

} //namespace Crux

#endif //MUTEX_GUARD_H

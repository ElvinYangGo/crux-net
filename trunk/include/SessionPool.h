#ifndef SESSION_POOL_H
#define SESSION_POOL_H

#include "Singleton.h"
#include "UniSizeAllocator.h"
#include "Session.h"
#include "Config.h"

namespace Crux
{

/**
 * session pool
 */
template<typename SessionAlloc=UniSizeAllocator>
class SessionPool
{
	friend class Singleton<SessionPool>;
private:
	SessionPool();
	~SessionPool();

public:
	Session* AllocateSession( int id, SOCKET sock, const std::string& ip, int port );
	void DeallocateSession( Session* pSession );

private:
	SessionAlloc m_SessionAlloc;

private:
	Mutex* m_pMutex;
};

template<typename SessionAlloc>
inline
SessionPool<SessionAlloc>::SessionPool()
: m_SessionAlloc( sizeof(Session), s_Config.GetClientSessionAmountMax() + s_Config.GetServerSessionAmountMax() )
, m_pMutex( new Mutex )
{
}

template<typename SessionAlloc>
inline
SessionPool<SessionAlloc>::~SessionPool()
{
	delete m_pMutex;
}

template<typename SessionAlloc>
inline
Session* SessionPool<SessionAlloc>::AllocateSession( int id, SOCKET sock, const std::string& ip, int port )
{
	m_pMutex->Lock();

	void* p = m_SessionAlloc.allocate();

	m_pMutex->UnLock();

	if( NULL != p )
	{
		return new (p) Session( id, sock, ip, port );
	}
	return static_cast<Session*>(NULL);
}

template<typename SessionAlloc>
inline
void SessionPool<SessionAlloc>::DeallocateSession( Session* pSession )
{
	pSession->~Session();

	m_pMutex->Lock();

	m_SessionAlloc.deallocate( pSession );

	m_pMutex->UnLock();
}

#define s_SessionPool Crux::Singleton< Crux::SessionPool<> >::Instance()

} //namespace Crux

#endif //SESSION_POOL_H

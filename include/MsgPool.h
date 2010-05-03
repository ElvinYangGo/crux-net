#ifndef MSG_POOL_H
#define MSG_POOL_H

#include "Singleton.h"
#include "Allocator.h"
#include "UniSizeAllocator.h"
#include "Msg.h"
#include <new>
#include "ConstConfigData.h"
#include "Mutex.h"
#include "Logger.h"
#include "MutexGuard.h"

namespace Crux
{

class Mutex;

/**
 * net message pool
 */

template<typename Alloc=Allocator, typename MsgAlloc=UniSizeAllocator>
class MsgPool
{
	friend class Singleton<MsgPool>;
private:
	MsgPool();
	~MsgPool();

public:
	Msg* AllocateMsg( size_t size );
	void DeallocateMsg( Msg* pMsg );

private:
	Mutex* m_pMutex;

private:
	MsgAlloc m_MsgAlloc;

private:
	static const int MSG_COUNT_MAX = config_MSG_COUNT_MAX;
};

template<typename Alloc, typename MsgAlloc>
const int MsgPool<Alloc, MsgAlloc>::MSG_COUNT_MAX;

template<typename Alloc, typename MsgAlloc>
inline
MsgPool<Alloc, MsgAlloc>::MsgPool()
: m_pMutex(new Mutex), m_MsgAlloc( sizeof(Msg), MSG_COUNT_MAX )
{
	Alloc::Initialize();
}

template<typename Alloc, typename MsgAlloc>
inline
MsgPool<Alloc, MsgAlloc>::~MsgPool()
{
	delete m_pMutex;
	Alloc::FreeAllMemory();
}

template<typename Alloc, typename MsgAlloc>
inline
Msg* MsgPool<Alloc, MsgAlloc>::AllocateMsg( size_t size )
{
	MutexGuard guard( m_pMutex );
	
	//allocate data
	void* pData = Alloc::allocate( size );
	if ( NULL != pData )
	{
		//allocate msg
		void* pMsg = m_MsgAlloc.allocate();
		if( NULL != pMsg )
		{
			return new (pMsg) Msg( size, pData );	
		}
		else
		{
			Alloc::deallocate( pData, size );
		}
	}
	else
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to allocate memory, size is " << size );
	}

	return static_cast<Msg*>(NULL);
}

template<typename Alloc, typename MsgAlloc>
inline
void MsgPool<Alloc, MsgAlloc>::DeallocateMsg( Msg* pMsg )
{
	MutexGuard guard( m_pMutex );

	//deallocate data
	Alloc::deallocate( pMsg->m_pData, pMsg->m_Size );
	//deallocate msg
	m_MsgAlloc.deallocate( pMsg );
}

#define s_MsgPool Crux::Singleton< Crux::MsgPool<> >::Instance()

} //namespace Crux

#endif //MSG_POOL_H

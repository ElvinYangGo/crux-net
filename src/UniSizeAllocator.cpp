#include "UniSizeAllocator.h"

namespace Crux
{

UniSizeAllocator::UniSizeAllocator( size_t size, int count )
{
	//allocate all needed memory
	m_pMemoryStart = malloc( size * count );

	//link the free memory
	LinkFreeElement( size, count );
}

UniSizeAllocator::~UniSizeAllocator()
{
	free( m_pMemoryStart );
	m_pMemoryStart = NULL;
}

void* UniSizeAllocator::allocate()
{
	obj* pCurrentObj = m_FreeList;
	if( NULL == pCurrentObj )
	{
		return NULL;
	}

	m_FreeList = pCurrentObj->m_pFreeListLink;
	return pCurrentObj;
}

void UniSizeAllocator::deallocate( void* p )
{
	obj* pCurrentObj = (obj*)p;
	pCurrentObj->m_pFreeListLink = m_FreeList;
	m_FreeList = pCurrentObj;
}

void UniSizeAllocator::LinkFreeElement( size_t size, int count )
{
	m_FreeList = (obj*)m_pMemoryStart;

	obj* pCurrentObj = m_FreeList;
	//last element's free link is null, so loop count is count-1
	for( int freeElementNum = 0; freeElementNum < count - 1; ++freeElementNum )
	{
		obj* pNextObj = (obj*)( (char*)pCurrentObj + size );
		pCurrentObj->m_pFreeListLink = pNextObj;
		pCurrentObj = pNextObj;
	}
	pCurrentObj->m_pFreeListLink = NULL;
}

} //namespace Crux

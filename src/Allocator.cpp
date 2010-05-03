#include "Allocator.h"
#include "SystemAllocator.h"
#include "Logger.h"

namespace Crux
{

const size_t Allocator::ALIGN = config_ALIGN;
const size_t Allocator::BYTES_MAX = config_BYTES_MAX;
const int Allocator::FREE_LIST_COUNT = config_BYTES_MAX / config_ALIGN;
const int Allocator::FREE_LIST_ELEMENT_COUNT = config_FREE_LIST_ELEMENT_COUNT;

void* Allocator::m_pMemoryStart = NULL;

Allocator::obj* volatile Allocator::m_FreeList[config_BYTES_MAX / config_ALIGN] = { };

void* Allocator::allocate( size_t size )
{
	//todo: set second level allocator when size > BYTES_MAX
	if( BYTES_MAX < size )
	{
		return SecondLevelAllocator::allocate( size );
	}
	int index = FreeListIndex(size);
	obj* result = m_FreeList[index];
	if( NULL == result )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to allocate space, NULL == m_FreeList[index], size is " << size );
		return NULL;
	}
	m_FreeList[index] = result->m_pFreeListLink;

	return result;
}

void Allocator::deallocate( void* p, size_t size )
{
	//todo: set second level allocator when size > BYTES_MAX
	if( BYTES_MAX < size )
	{
		SecondLevelAllocator::deallocate( p, size );
		return;
	}
	//insert p to the corresponding free list
	int index = FreeListIndex(size);
	obj* pCurrentObj = m_FreeList[index];
	((obj*)p)->m_pFreeListLink = pCurrentObj;
	m_FreeList[index] = (obj*)p;
}

size_t Allocator::RoundUp( size_t bytes )
{
	return ( ( bytes + ALIGN - 1 ) & ~( ALIGN - 1 ) );
}

int Allocator::FreeListIndex( size_t bytes )
{
	return static_cast<int>( ( bytes + ALIGN - 1 ) / ALIGN - 1 );
}

size_t Allocator::MemoryTotal()
{
	size_t size = 0;
	for( size_t i = 8; i <= BYTES_MAX; i += ALIGN )
	{
		size += i * FREE_LIST_ELEMENT_COUNT;
	}
	return size;
}

void Allocator::SetFreeListPtr( void* pMemory )
{	
	char* pCurrentPos = static_cast<char*>(pMemory);
	for ( int i = 0; i < FREE_LIST_COUNT; ++i )
	{
		pCurrentPos += ALIGN * i * FREE_LIST_ELEMENT_COUNT;
		m_FreeList[i] = (obj*)(pCurrentPos);
	}
}

void Allocator::LinkFreeElement()
{
	for( int freeListIndex = 0; freeListIndex < FREE_LIST_COUNT; ++freeListIndex)
	{
		obj* pCurrentObj = m_FreeList[freeListIndex];
		//SetFreeListPtr set the first free element, so the loop times is FREE_LIST_ELEMENT-1
		for( int freeElementNum = 0; freeElementNum < FREE_LIST_ELEMENT_COUNT - 1; ++freeElementNum )
		{
			obj* pNextObj = (obj*)( (char*)pCurrentObj + (freeListIndex+1)*ALIGN );
			pCurrentObj->m_pFreeListLink = pNextObj;
			pCurrentObj = pNextObj;
		}
		pCurrentObj->m_pFreeListLink = NULL;
	}
}

void Allocator::Initialize()
{
	//allocate total memory
	m_pMemoryStart = malloc( MemoryTotal() );

	//set free list's free element
	SetFreeListPtr( m_pMemoryStart );

	//link each free element list
	LinkFreeElement();
}

void Allocator::FreeAllMemory()
{
	free( m_pMemoryStart );
	m_pMemoryStart = NULL;
}

} //namespace Crux

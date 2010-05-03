#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>
#include "ConstConfigData.h"

namespace Crux
{

class SystemAllocator;

typedef SystemAllocator SecondLevelAllocator;

/**
 * memory allocator
 */

class Allocator
{
public:
	Allocator() {}

public:
	/**
	 * allocate memory of given size.
	 * At least memory of given size will be allocated from the free list
	 * the actual size of allocated memory is aligned with ALIGN
	 */
	static void* allocate( size_t size );

	/**
	 * deallocate memory.
	 * put the memory to the free list
	 * @param p memory address, cannot be NULL
	 * @param size memory's size
	 */
	static void deallocate( void* p, size_t size );

public:
	/**
	 * allocate all the memory, and initialize it.
	 */
	static void Initialize();

	/**
	 * free all the memory.
	 */
	static void FreeAllMemory();

private:
	static size_t RoundUp( size_t bytes );

	/**
	 * return the free list index of size bytes.
	 * @param bytes must > 0
	 * @return the free list index of size bytes
	 */
	static int FreeListIndex( size_t bytes );

	/**
	 * calculate total memory size that free list need.
	 * @return total memory size that free list need
	 */
	static size_t MemoryTotal();

	/**
	 * set free list, make it point to the free element.
	 */
	static void SetFreeListPtr( void* pMemory );

	/**
	 * link free elements for free list.
	 */
	static void LinkFreeElement();

private:
	union obj
	{
		union obj* m_pFreeListLink;
		char m_ClientData[1];
	};

private:
	static const size_t ALIGN;
	static const size_t BYTES_MAX;
	static const int FREE_LIST_COUNT;
	static const int FREE_LIST_ELEMENT_COUNT;

private:
	static obj* volatile m_FreeList[config_BYTES_MAX/config_ALIGN];

	static void* m_pMemoryStart;
};

}

#endif //ALLOCATOR_H

#ifndef UNI_SIZE_ALLOCATOR_H
#define UNI_SIZE_ALLOCATOR_H

#include <stdlib.h>

namespace Crux
{

/**
 * fixed size memory allocator
 */
class UniSizeAllocator
{
public:
	UniSizeAllocator( size_t size, int count );
	~UniSizeAllocator();

public:
	/**
	 * allocate fixed size memory
	 * @return memory address allocated, if no available memory, return NULL
	 */
	void* allocate();
	void deallocate( void* p );

private:
	void LinkFreeElement( size_t size, int count );

private:
	union obj
	{
		union obj* m_pFreeListLink;
		char m_ClientData[1];
	};

private:
	obj* volatile m_FreeList;

	void* m_pMemoryStart;
};

} //namespace Crux

#endif //UNI_SIZE_ALLOCATOR_H

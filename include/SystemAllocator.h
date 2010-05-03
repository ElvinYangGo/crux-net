#ifndef SYSTEM_ALLOCATOR_H
#define SYSTEM_ALLOCATOR_H

#include <stdlib.h>

namespace Crux
{

/**
 * system allocator
 * use system's api to allocate memory
 */
class SystemAllocator
{
public:
	static void* allocate( size_t size );
	static void deallocate( void* p, size_t size );
};

} //namespace Crux

#endif //SYSTEM_ALLOCATOR_H

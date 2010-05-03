#include "SystemAllocator.h"
#include "Logger.h"

namespace Crux
{

void* SystemAllocator::allocate( size_t size )
{
	LOG_WARN( logger, __FUNCTION__ << " use system allocator to allocate memory!" );

	void* p = malloc( size );
	if( NULL == p )
	{
		LOG_WARN( logger, __FUNCTION__ << " big warning!!! system allocator cannot allocate memory" );
	}

	return p;
}

void SystemAllocator::deallocate( void* p, size_t size )
{
	LOG_WARN( logger, __FUNCTION__ << " use system allocator to deallocate memory" );

	free( p );
}

} //namespace Crux

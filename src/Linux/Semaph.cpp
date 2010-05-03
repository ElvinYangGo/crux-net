#include "Linux/Semaph.h"

namespace Crux
{

Semaph* g_pMsgQueueSemaph = new Semaph;
Semaph* g_pAsyncOperationQueueSemaph = new Semaph;

Semaph::Semaph()
: m_pSem( new sem_t )
{
	//the initial value of the semaphore is 0
	sem_init( m_pSem, 0, 0 );
}

Semaph::~Semaph()
{
	sem_destroy( m_pSem );
}

void Semaph::Post()
{
	sem_post( m_pSem );
}

void Semaph::Wait()
{
	sem_wait( m_pSem );
}

int Semaph::TryWait()
{
	return sem_trywait( m_pSem );
}

}

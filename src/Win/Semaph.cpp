#include "Semaph.h"
#include <limits>
#include "Logger.h"
#include "HelperFunction.h"

namespace Crux
{

Semaph* g_pMsgQueueSemaph = new Semaph;

Semaph::Semaph()
{
	//the initial value of the semaphore is 0
	m_Sem = CreateSemaphore(
		NULL,
		0,
		(std::numeric_limits<long>::max)(),
		NULL
		);
	if ( NULL == m_Sem )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed CreateSemaphore(), " << FormatErrorMessage( GetLastError() ) );
	}
}

Semaph::~Semaph()
{
	CloseHandle( m_Sem );
}

void Semaph::Post()
{
	if( 0 == ReleaseSemaphore(
		m_Sem,
		1,
		NULL
		)
		)
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed ReleaseSemaphore(), " << FormatErrorMessage( GetLastError() ) );
	}
}

void Semaph::Wait()
{
	WaitForSingleObject( m_Sem, INFINITE );
}

int Semaph::TryWait()
{
	return WaitForSingleObject( m_Sem, 0 );
}

}

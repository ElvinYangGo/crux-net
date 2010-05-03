#include "Dispatcher.h"
#include "Config.h"
#include "Logger.h"

namespace Crux
{

Dispatcher::Dispatcher()
{
	if ( !CreateIOCP() )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to create iocp!" );
		return;
	}
	LOG_INFO( logger, "create iocp" );
}

Dispatcher::~Dispatcher()
{
	CloseHandle( m_IOCP );
}

HANDLE Dispatcher::GetIOCPHandle() const
{
	return m_IOCP;
}

bool Dispatcher::CreateIOCP()
{
	m_IOCP = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL,
		0,
		s_Config.GetThreadAmount()
		);
	if ( NULL == m_IOCP )
	{
		return false;
	}

	return true;
}

bool Dispatcher::RegisterSocket(SOCKET sock, void* pCompletionKey )
{
	//associate with completion port
	if ( NULL == CreateIoCompletionPort(
		(HANDLE)sock,
		m_IOCP,
		(ULONG_PTR)pCompletionKey,
		0
		)
		)
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to register socket!" );
		return false;
	}

	return true;
}

}

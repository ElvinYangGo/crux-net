#include "TCPServer.h"
#include "Config.h"
#include "AcceptType.h"

#ifdef WIN32
#include "Win/AcceptHandler.h"
#elif defined(LINUX)
#include "Linux/AcceptCompleteOperation.h"
#endif

namespace Crux
{

TCPServer::TCPServer()
{
#ifdef WIN32
	m_pExternalAcceptHandler = new AcceptHandler(
		ACCEPT_EXTERNAL,
		s_Config.GetIP(),
		s_Config.GetPort(),
		s_Config.GetListenBacklog()
		);
#elif defined(LINUX)
	m_pExternalAcceptCompleteOperation = new AcceptCompleteOperation(
		ACCEPT_EXTERNAL,
		s_Config.GetIP(),
		s_Config.GetPort(),
		s_Config.GetListenBacklog()
		);
#endif
}

TCPServer::~TCPServer()
{
#ifdef WIN32
	delete m_pExternalAcceptHandler;
#elif defined(LINUX)
	delete m_pExternalAcceptCompleteOperation;
#endif
}

void TCPServer::Shutdown()
{
	TCPStream::Shutdown();
}

} //namespace Crux

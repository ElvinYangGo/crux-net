#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "Singleton.h"
#include "TCPStream.h"

namespace Crux
{

#ifdef WIN32
class AcceptHandler;
#elif defined(LINUX)
class AcceptCompleteOperation;
#endif

class TCPServer : public TCPStream
{
	friend class Singleton<TCPServer>;

private:
	TCPServer();
	~TCPServer();

public:
	virtual void Shutdown();

private:
#ifdef WIN32
	AcceptHandler* m_pExternalAcceptHandler;
#elif defined(LINUX)
	AcceptCompleteOperation* m_pExternalAcceptCompleteOperation;
#endif
};

#define s_TCPServer Crux::Singleton<Crux::TCPServer>::Instance()

}	//namespace Crux

#endif	//TCP_SERVER_H

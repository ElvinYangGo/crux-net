#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "Singleton.h"
#include <Winsock2.h>

namespace Crux
{

class Session;

class Dispatcher
{
	friend class Singleton<Dispatcher>;

private:
	Dispatcher();
	~Dispatcher();

public:
	bool RegisterSocket( SOCKET sock, void* pCompletionKey );

	HANDLE GetIOCPHandle() const;

private:
	bool CreateIOCP();

private:
	HANDLE m_IOCP;
};

#define s_Dispatcher Crux::Singleton<Crux::Dispatcher>::Instance()

} //namespace Crux

#endif	//DISPATCHER_H

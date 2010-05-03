#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "Singleton.h"
#include <sys/epoll.h>
#include "Thread.h"

namespace Crux
{

class AsyncCompleteOperation;

class Dispatcher : public Thread
{
	friend class Singleton<Dispatcher>;

private:
	Dispatcher();
	~Dispatcher();

public:
	/**
	* insert socket and its asynchronous operation
	* @param sock socket
	* @param asyncCompletionOperation asynchronous operation pointer
	* @param events socket's epoll events
	* @return true,succeeded; false, failed
	*/
	bool RegisterSocket(
		int sock, 
		AsyncCompleteOperation* pAsyncCompleteOperation, 
		unsigned int events = EPOLLIN | EPOLLOUT | EPOLLET
		);

	virtual void Run();

	virtual void Shutdown();

private:

	bool SetNonBlocking( int fd );

private:
	int m_Epoll;
};

#define s_Dispatcher Crux::Singleton<Crux::Dispatcher>::Instance()

} //namespace Crux

#endif	//DISPATCHER_H


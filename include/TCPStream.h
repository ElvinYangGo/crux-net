#ifndef TCP_STREAM_H
#define TCP_STREAM_H

#include <string>
#include <vector>

namespace Crux
{

class WorkerThread;

class TCPStream
{
public:
	TCPStream();
	virtual ~TCPStream();
	
public:
	/**
	* connect to server
	* @param ip the server's ip
	* @param port the server's port
	* @return the result of connecting
	*/
	bool Connect( const std::string& ip, int port );

	virtual void Shutdown();


private:
	typedef std::vector<WorkerThread*> THREADS;
	THREADS m_Threads;

};

} //namespace Crux

#endif //TCP_STREAM_H

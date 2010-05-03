#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "Singleton.h"
#include "TCPStream.h"

namespace Crux
{

class Session;

class TCPClient : public TCPStream
{
	friend class Singleton<TCPClient>;
private:
	TCPClient();
	~TCPClient();

public:
	void Disconnect();

	/**
	 * check if the client is connected to a server
	 * @return the connect status of the client
	 */
	bool Connected() const;

	void SetConnected( bool connected );

	void SetSession( Session* pSession );

	bool SendMsg( int sessionID, void* pData, size_t size );

	size_t GetSendDataSizeTotal() const;
	size_t GetRecvDataSizeTotal() const;

private:
	bool m_Connected;
	Session* m_pSession;
};

#define s_TCPClient Crux::Singleton<Crux::TCPClient>::Instance()

} //namespace Crux

#endif //TCPCLIENT_H

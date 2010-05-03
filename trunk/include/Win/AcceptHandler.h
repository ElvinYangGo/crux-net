#ifndef ACCEPT_HANDLER_H
#define ACCEPT_HANDLER_H

#include "AcceptType.h"
#include "Win/CompleteHandler.h"
#include <string>
#include <Winsock2.h>

namespace Crux
{

class AsyncCompleteOperation;

class AcceptHandler : public CompleteHandler
{
public:
	AcceptHandler(
		AcceptType acceptType,
		const std::string& ip, 
		int port,
		int backlog
		);
	~AcceptHandler();

public:
	virtual void Complete( Session*, unsigned long );

private:
	bool Bind( const std::string& ip, int port );
	bool Listen( int backlog );

	bool Accept();

private:
	std::string m_IP;
	int m_Port;
	int m_Backlog;
	AsyncCompleteOperation* m_pAsyncCompleteOperation;
	AcceptType m_AcceptType;
	SOCKET m_ListenSock;
	SOCKET m_AcceptSock;
	char m_AcceptBuffer[2*( sizeof(sockaddr_in) + 16 )];
};

} //namespace Crux

#endif	//ACCEPT_HANDLER_H
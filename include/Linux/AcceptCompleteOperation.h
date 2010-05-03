#ifndef ACCEPT_COMPLETE_OPERATION_H
#define ACCEPT_COMPLETE_OPERATION_H

#include "Linux/AsyncCompleteOperation.h"
#include "AcceptType.h"
#include <string>

namespace Crux
{

class AcceptCompleteOperation : public AsyncCompleteOperation
{
public:
	AcceptCompleteOperation(AcceptType acceptType, const std::string& ip, int port, int backlog );

public:
	virtual void Complete( AsyncOperationType );

private:
	bool Bind();
	bool Listen( int backlog );

private:
	int m_ListenSock;

	AcceptType m_AcceptType;
	std::string m_IP;
	int m_Port;
};

} //namespace Crux

#endif //ACCEPT_COMPLETE_OPERATION_H


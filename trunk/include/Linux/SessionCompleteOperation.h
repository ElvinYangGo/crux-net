#ifndef SESSION_COMPLETE_OPERATION_H
#define SESSION_COMPLETE_OPERATION_H

#include "Linux/AsyncCompleteOperation.h"

namespace Crux
{

class Session;

class SessionCompleteOperation : public AsyncCompleteOperation
{
public:
	virtual void Complete( AsyncOperationType asyncOperationType );

	void SetSession( Session* pSession );

private:
	void Recv();
	void Send();
	bool Close();

private:
	Session* m_pSession;
};

}

#endif	//SESSION_COMPLETE_OPERATION_H


#ifndef RECV_HANDLER_H
#define RECV_HANDLER_H

#include "Win/CompleteHandler.h"

namespace Crux
{

class Session;

class RecvHandler : public CompleteHandler
{
public:
	virtual void Complete( Session* pSession, unsigned long bytesTransferred );

};

}

#endif	//RECV_HANDLER_H
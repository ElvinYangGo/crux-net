#ifndef SEND_HANDLER_H
#define SEND_HANDLER_H

#include "Win/CompleteHandler.h"

namespace Crux
{

class Session;

class SendHandler : public CompleteHandler
{
public:
	virtual void Complete( Session* pSession, unsigned long bytesTransferred );
};

}

#endif	//SEND_HANDLER_H
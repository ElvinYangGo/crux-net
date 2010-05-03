#ifndef CLOSE_HANDLER_H
#define CLOSE_HANDLER_H

#include "Win/CompleteHandler.h"

namespace Crux
{

class CloseHandler : public CompleteHandler
{
public:
	virtual void Complete( Session* pSession, unsigned long bytesTransferred );
};

} //namespace Crux

#endif	//CLOSE_HANDLER_H
#ifndef COMPLETE_HANDLER_H
#define COMPLETE_HANDLER_H

namespace Crux
{

class Session;

class CompleteHandler
{
public:
	virtual ~CompleteHandler() {}

public:
	virtual void Complete( Session*, unsigned long ) = 0;
};

} //namespace Crux

#endif	//COMPLETE_HANDLER_H
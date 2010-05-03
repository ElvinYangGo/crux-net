#ifndef ASYNC_COMPLETE_OPERATION_H
#define ASYNC_COMPLETE_OPERATION_H

#include "Linux/AsyncOperationType.h"

namespace Crux
{

class AsyncCompleteOperation
{
public:
	virtual ~AsyncCompleteOperation() {}
public:
	virtual void Complete( AsyncOperationType asyncOperationType ) = 0;
};

} //namespace Crux

#endif //ASYNC_COMPLETE_OPERATION_H


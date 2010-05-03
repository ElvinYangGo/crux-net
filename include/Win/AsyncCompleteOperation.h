#ifndef ASYNC_COMPLETE_OPERATION_H
#define ASYNC_COMPLETE_OPERATION_H

#include <Windows.h>

namespace Crux
{

class Session;
class CompleteHandler;

class AsyncCompleteOperation : public OVERLAPPED
{
public:
	AsyncCompleteOperation();
	AsyncCompleteOperation( CompleteHandler* pCompleteHandler, CompleteHandler* pCloseHandler );
	
	void Init( CompleteHandler* pCompleteHandler, CompleteHandler* pCloseHandler );

public:
	void Complete( Session* pSession );
	void Close( Session* pSession );

private:
	unsigned long GetBytesTransferred();

private:
	CompleteHandler* m_pCompleteHandler;
	CompleteHandler* m_pCloseHandler;

};

}

#endif	//ASYNC_COMPLETE_OPERATION_H
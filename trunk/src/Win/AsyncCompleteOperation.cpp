#include "Win/AsyncCompleteOperation.h"
#include "Win/CompleteHandler.h"

namespace Crux
{

AsyncCompleteOperation::AsyncCompleteOperation()
{
	hEvent = 0;
	Internal = 0;
	InternalHigh = 0;
	Offset = 0;
	OffsetHigh = 0;
}

AsyncCompleteOperation::AsyncCompleteOperation(CompleteHandler* pCompleteHandler, CompleteHandler* pCloseHandler )
: m_pCompleteHandler( pCompleteHandler ), m_pCloseHandler( pCloseHandler )
{
	hEvent = 0;
	Internal = 0;
	InternalHigh = 0;
	Offset = 0;
	OffsetHigh = 0;
}

void AsyncCompleteOperation::Init(CompleteHandler* pCompleteHandler, CompleteHandler* pCloseHandler )
{
	m_pCompleteHandler = pCompleteHandler;
	m_pCloseHandler = pCloseHandler;
}

void AsyncCompleteOperation::Complete( Session* pSession )
{
	m_pCompleteHandler->Complete( pSession, GetBytesTransferred()  );
}

void AsyncCompleteOperation::Close(Session* pSession )
{
	m_pCloseHandler->Complete( pSession, GetBytesTransferred() );
}

unsigned long AsyncCompleteOperation::GetBytesTransferred()
{
	return static_cast<unsigned long>( InternalHigh );
}

} //namespace Crux

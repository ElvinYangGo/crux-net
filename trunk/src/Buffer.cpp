#include "Buffer.h"
#include <string.h>

namespace Crux
{

const size_t Buffer::BUFFER_SIZE = config_BUFFER_SIZE;
//const size_t Buffer::BUFFER_SIZE;

Buffer::Buffer()
{
	memset( m_pBuffer, 0 ,sizeof(m_pBuffer) );
	m_pBegin = m_pEnd = m_pBuffer;
}

char* Buffer::GetBegin() const
{
	return m_pBegin;
}

char* Buffer::GetEnd() const
{
	return m_pEnd;
}

size_t Buffer::GetFreeSpaceSize() const
{
	return BUFFER_SIZE - ( m_pEnd - m_pBuffer );
}

size_t Buffer::GetDataSize() const
{
	return m_pEnd - m_pBegin;
}

bool Buffer::IncreaseBegin( size_t size )
{
	if( m_pEnd < m_pBegin + size )
	{
		return false;
	}

	m_pBegin += size;

	//only when increase send begin pos, send begin pos may be equal with send end pos
	// so, we check if they point to the same pos, if so, move them to the send buffer's beginning
	if( m_pBegin == m_pEnd )
	{
		m_pBegin = m_pEnd = m_pBuffer;
	}

	return true;
}

bool Buffer::IncreaseEnd( size_t size )
{
	if( GetFreeSpaceSize() < size )
	{
		return false;
	}

	m_pEnd += size;

	return true;
}

void Buffer::MoveData()
{
	if( (m_pBuffer == m_pBegin) || (m_pBegin == m_pEnd) )
	{
		return;
	}

	size_t dataSize = m_pEnd - m_pBegin;
	memmove( m_pBuffer, m_pBegin, dataSize );

	m_pEnd = m_pBuffer + dataSize;
	m_pBegin = m_pBuffer;
}



} //namespace Crux

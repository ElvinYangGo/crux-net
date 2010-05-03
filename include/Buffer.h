#ifndef BUFFER_H
#define BUFFER_H

#include "ConstConfigData.h"

namespace Crux
{

/**
 * buffer for receiving and sending
 */

class Buffer
{
public:
	Buffer();

public:
	char* GetBegin() const;
	char* GetEnd() const;
	size_t GetDataSize() const;
	size_t GetFreeSpaceSize() const;
	bool IncreaseBegin( size_t size );
	bool IncreaseEnd( size_t size );
	void MoveData();

private:
	//static const size_t BUFFER_SIZE = config_BUFFER_SIZE;
	static const size_t BUFFER_SIZE;

private:
	char m_pBuffer[config_BUFFER_SIZE]; //why we can't use BUFFER_SIZE in vc
//	char m_pBuffer[BUFFER_SIZE]; //why we can't use BUFFER_SIZE in vc
	char* m_pBegin;
	char* m_pEnd;
};

} //namespace Crux

#endif //BUFFER_H


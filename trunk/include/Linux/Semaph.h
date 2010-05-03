#ifndef SEMAPH_H
#define SEMAPH_H

#include <semaphore.h>

namespace Crux
{

class Semaph
{
public:
	Semaph();
	~Semaph();

private:
	Semaph( const Semaph& );
	Semaph& operator=( const Semaph& );

public:
	void Post();
	void Wait();
	int TryWait();

private:
	sem_t* m_pSem;
};

extern Semaph* g_pMsgQueueSemaph;
extern Semaph* g_pAsyncOperationQueueSemaph;

} //namespace Crux

#endif //SEMAPH_H

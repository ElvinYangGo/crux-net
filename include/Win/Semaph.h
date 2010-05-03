#ifndef SEMAPH_H
#define SEMAPH_H

#include <Windows.h>

namespace Crux
{

class Semaph
{
public:
	Semaph();
	~Semaph();

private:
	Semaph( const Semaph& ) {}
	Semaph& operator=( const Semaph& ) {}

public:
	void Post();
	void Wait();
	int TryWait();

private:
	HANDLE m_Sem;
};

extern Semaph* g_pMsgQueueSemaph;

} //namespace Crux

#endif //SEMAPH_H

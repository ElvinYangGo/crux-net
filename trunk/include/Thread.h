#ifndef THREAD_SELECT_H
#define THREAD_SELECT_H

#ifdef WIN32

#include "Win/Thread.h"

#elif defined(LINUX)

#include "Linux/Thread.h"

#endif

#endif	//THREAD_SELECT_H


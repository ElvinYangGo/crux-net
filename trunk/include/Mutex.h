#ifndef MUTEX_SELECT_H
#define MUTEX_SELECT_H

#ifdef WIN32

#include "Win/Mutex.h"

#elif defined(LINUX)

#include "Linux/Mutex.h"

#endif

#endif	//MUTEX_SELECT_H


#ifndef DISPATCHER_SELECT_H
#define DISPATCHER_SELECT_H

#ifdef WIN32
#include "Win/Dispatcher.h"
#elif defined(LINUX)
#include "Linux/Dispatcher.h"
#endif

#endif	//DISPATCHER_SELECT_H

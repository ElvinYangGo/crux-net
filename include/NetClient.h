#ifndef NET_CLIENT_H
#define NET_CLIENT_H

#include "MsgPool.h"
#include "MsgQueue.h"
#include "Logger.h"
#include "TCPClient.h"
#include "Semaph.h"
#include "Config.h"
#include "MsgHandler.h"
#include "Session.h"

#ifdef LINUX
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#endif

void ShutdownNetClient();
void HandleNet( Crux::MsgHandler* pMsgHandler );

#endif //NET_CLIENT_H

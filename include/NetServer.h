#ifndef NET_SERVER_H
#define NET_SERVER_H

#include "MsgQueue.h"
#include "Msg.h"
#include "MsgPool.h"
#include "Session.h"
#include "Semaph.h"
#include "SessionMgr.h"
#include "TCPServer.h"
#include "Logger.h"
#include "MsgHandler.h"

typedef bool (*INIT_NET)();

void ShutdownNetServer();
void NetLoop( INIT_NET init, Crux::MsgHandler* pMsgHandler );

#endif //NET_SERVER_H

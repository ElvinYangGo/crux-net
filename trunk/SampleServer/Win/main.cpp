#include "NetServer.h"
#include "../ServerMsgHandler.h"

bool InitNet()
{
	s_TCPServer;

	//here we can connect other server
	//...

	LOG_INFO( logger, "start server..." );

	return true;
}

int main()
{	
	ServerMsgHandler serverMsgHandler;
	NetLoop( InitNet, &serverMsgHandler );

	return 0;
}

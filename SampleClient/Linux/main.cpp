#include "NetClient.h"
#include "../ClientMsgHandler.h"

bool InitNet()
{
	if( !s_TCPClient.Connect( s_Config.GetIP(), s_Config.GetPort() ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to connect to server!");
		return false;
	}

	LOG_INFO( logger, "start client..." );
	
	return true;
}

int main()
{
	ClientMsgHandler clientMsgHandler;

	if( !InitNet() )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed InitNet()!");
		return 1;
	}

	//main loop
	while( true )
	{
		HandleNet( &clientMsgHandler );

		if( !s_TCPClient.Connected() )
		{
			break;
		}
	}

	ShutdownNetClient();

	return 0;
}


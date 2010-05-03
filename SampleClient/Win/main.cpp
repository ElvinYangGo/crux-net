#include "NetClient.h"
#include "../ClientMsgHandler.h"

bool InitNet()
{
	if( !s_TCPClient.Connect( s_Config.GetIP(), s_Config.GetPort() ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to connect to server!" );
		return false;
	}

	LOG_INFO( logger, "start client..." );

	return true;
}

int main()
{
	DWORD timeStart = GetTickCount();

	ClientMsgHandler clientMsgHandler;

	if( !InitNet() )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed InitNet()!" );
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

	DWORD timePassed = GetTickCount() - timeStart;
	LOG_INFO( logger, "time consumed is " << timePassed / 1000 << " ms." );

	LOG_INFO( logger, "begin shutdown..." );
	ShutdownNetClient();

	return 0;
}


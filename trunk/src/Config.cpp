#include "Config.h"
#include "ConfigReader.h"

namespace Crux
{

Config::Config()
{
	ConfigReader reader("../../configure/Config.cfg");
	reader.Init();
	
	reader.ReadString( "IP", m_IP );
	reader.ReadInteger( "Port", m_Port );
	reader.ReadInteger( "ThreadAmount", m_ThreadAmount );
	reader.ReadInteger( "ListenBacklog", m_ListenBacklog );
	reader.ReadInteger( "ClientSessionAmountMax", m_ClientSessionAmountMax );
	reader.ReadInteger( "ServerSessionAmountMax", m_ServerSessionAmountMax );
}

const std::string& Config::GetIP() const
{
	return m_IP;
}

int Config::GetPort() const
{
	return m_Port;
}

int Config::GetThreadAmount() const
{
	return m_ThreadAmount;
}

int Config::GetListenBacklog() const
{
	return m_ListenBacklog;
}

int Config::GetClientSessionAmountMax() const
{
	return m_ClientSessionAmountMax;
}

int Config::GetServerSessionAmountMax() const
{
	return m_ServerSessionAmountMax;
}

} //namespace Crux

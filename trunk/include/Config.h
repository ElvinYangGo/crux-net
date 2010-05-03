#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include "Singleton.h"

namespace Crux
{
/**
 * Get configure value from configure file
 */

class Config
{
	friend class Singleton<Config>;
private:
	Config();

public:
	const std::string& GetIP() const;
	int GetPort() const;
	/**
	 * get concurrent working threads' amount
	 * @return the amount of the concurrent working threads
	 */
	int GetThreadAmount() const;

	/**
	 * get backlog for listen
	 * @return the backlog
	 */
	int GetListenBacklog() const;

	/**
	 * get max client amount
	 * @return the max amount of client
	 */
	int GetClientSessionAmountMax() const;

	int GetServerSessionAmountMax() const;

private:
	std::string m_IP;
	int m_Port;
	int m_ThreadAmount;
	int m_ListenBacklog;
	int m_ClientSessionAmountMax;
	int m_ServerSessionAmountMax;
};

#define s_Config Crux::Singleton<Crux::Config>::Instance()

} //namespace Crux
#endif //CONFIG_H

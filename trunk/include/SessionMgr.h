#ifndef SESSION_MGR_H
#define SESSION_MGR_H

#include "Singleton.h" 
#include <map>

#ifdef WIN32
#include <WinSock2.h>

#elif defined(LINUX)

typedef int SOCKET;
#endif


namespace Crux
{

class Mutex;
class Session;
template<typename T> class Stack;

/**
 * Session Manager
 */

class SessionMgr
{
	friend class Singleton<SessionMgr>;

	typedef std::map<int, Session*> SESSION_MAP;
	typedef std::map<int, bool> SESSION_ACTIVE_STATUS_MAP;
private:
	SessionMgr();
	~SessionMgr();

public:
	Session* CreateSession( SOCKET sock, const std::string& ip, int port );
	void DestroySession( Session* pSession );

	void DestroyAllSessions();

public:
	/**
	 * Generate a unique session id
	 * @return the unique session id
	 */
	int GenerateSessionID();

	/**
	 * Insert a session to session list
	 * @param pSession the session to be inserted
	 * @return true,if succeed; false,if there is already a same session id in the list
	 */
	bool InsertSession( Session* pSession );

	/**
	 * Remove a session from session list
	 * @param sessionID the session's id
	 * @return true,if succeed; false,if this session doesn't exist
	 */
	bool RemoveSession( int sessionID );

	/**
	 * get the session by its id
	 * @param sessionID the session's id
	 * @return the session; if this session doesn't exist, return NULL
	 */
	Session* GetSession( int sessionID );

	/**
	 * check if this session is active
	 * when this session's socket is closed, but is still in using(session is not deallocated), it is unactive.
	 * @param sessionID the session's id
	 * @return true,if this session is active; false, otherwise
	 */
	bool SessionActive( int sessionID );

	/**
	 * set session's active status
	 * @param sessionID the session's id
	 * @param active the session's active status
	 */
	void SetSessionActive( int sessionID, bool active );

private:
	void SetSessionActiveImpl( int sessionID, bool active );

private:
	Mutex* m_pMutex;

	SESSION_MAP m_Sessions;
	Stack<int>* m_pFreeSessionID;
	SESSION_ACTIVE_STATUS_MAP m_SessionActiveStatus;
};

#define s_SessionMgr Crux::Singleton<Crux::SessionMgr>::Instance()

} 	//namespace Crux

#endif 	//SESSION_MGR_H

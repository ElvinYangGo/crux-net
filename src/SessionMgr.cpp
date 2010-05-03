#include "SessionMgr.h"
#include "Session.h"
#include "Stack.h"
#include "Config.h"
#include "MutexGuard.h"
#include "SessionPool.h"
#include "Logger.h"
#include "HelperFunction.h"

namespace Crux
{

SessionMgr::SessionMgr()
: m_pMutex( new Mutex ),
m_pFreeSessionID( new Stack<int>( s_Config.GetClientSessionAmountMax() + s_Config.GetServerSessionAmountMax() ) )
{
	//initialize session id
	for( int i = 0; i < s_Config.GetClientSessionAmountMax() + s_Config.GetServerSessionAmountMax(); ++i )
	{
		//the first id is 1000
		m_pFreeSessionID->Push( i + 1000 );
		m_Sessions.insert( std::make_pair( i + 1000, static_cast<Session*>(NULL) ) );

		m_SessionActiveStatus.insert( std::make_pair( i + 1000, false ) );
	}
}

SessionMgr::~SessionMgr()
{
	delete m_pMutex;
	delete m_pFreeSessionID;
}

Session* SessionMgr::CreateSession( SOCKET sock, const std::string& ip, int port )
{
	//add this session
	int sessionID = GenerateSessionID();
	if( 0 == sessionID )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to generate session id!" );
		return NULL;
	}
	Session* pSession = s_SessionPool.AllocateSession( 
		sessionID,
		sock,
		ip,
		port
		);
	if( NULL == pSession )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to allocate session from pool!" );
		return NULL;
	}
	if( !InsertSession( pSession ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to insert session into SessionMgr!" );
		return NULL;
	}

	return pSession;
}

void SessionMgr::DestroySession(Session* pSession )
{
	if( !RemoveSession( pSession->GetID() ) )
	{
		LOG_ERROR( logger, __FUNCTION__ << " failed to remove session from s_SessionMgr, id: " << pSession->GetID() );
	}
	s_SessionPool.DeallocateSession( pSession );
}

void SessionMgr::DestroyAllSessions()
{
	SESSION_MAP::iterator it = m_Sessions.begin();
	SESSION_MAP::iterator end = m_Sessions.end();
	for (; end != it; ++it )
	{
		if ( NULL != it->second )
		{
			if( SessionActive( it->second->GetID() ) )
			{
#ifdef WIN32
				if( SOCKET_ERROR == closesocket( it->second->GetSock() ) )
				{
					int errNum = WSAGetLastError();
#elif defined(LINUX)
				if( -1 == close( it->second->GetSock() ) )
				{
					int errNum = errno;
#endif
					LOG_INFO( logger, __FUNCTION__ 
							<< " failed to close socket, session id:" << it->second->GetID()
							<< ", socket: " << it->second->GetSock()
							<< ", " << FormatErrorMessage( errNum )
						   	);
				}

			}
			DestroySession( it->second );
		}
	}	
}

int SessionMgr::GenerateSessionID()
{
	MutexGuard guard( m_pMutex );

	int sessionID = 0;
	if( !m_pFreeSessionID->Top( sessionID ) )
	{
		return 0;
	}

	SetSessionActiveImpl( sessionID, true );

	m_pFreeSessionID->Pop();

	return sessionID;
}

bool SessionMgr::InsertSession( Session* pSession )
{
	MutexGuard guard( m_pMutex );

	SESSION_MAP::iterator it = m_Sessions.find( pSession->GetID() );
	if( m_Sessions.end() != it )
	{
		it->second = pSession;

		return true;
	}

	return false;
}

bool SessionMgr::RemoveSession( int sessionID )
{
	MutexGuard guard( m_pMutex );

	SetSessionActiveImpl( sessionID, false );

	SESSION_MAP::iterator it = m_Sessions.find( sessionID );
	if( m_Sessions.end() != it )
	{
		//get back the session id
		if( !m_pFreeSessionID->Push( sessionID ) )
		{
			return false;
		}

		it->second = NULL;

		return true;
	}

	return false;
}

Session* SessionMgr::GetSession( int sessionID )
{
	SESSION_MAP::iterator it = m_Sessions.find( sessionID );
	if ( m_Sessions.end() != it )
	{
		return it->second;
	}
	return NULL;
}

bool SessionMgr::SessionActive( int sessionID )
{
	SESSION_ACTIVE_STATUS_MAP::iterator it = m_SessionActiveStatus.find( sessionID );
	if( m_SessionActiveStatus.end() != it )
	{
		return it->second;
	}
	return false;
}

void SessionMgr::SetSessionActive( int sessionID, bool active )
{
	MutexGuard guard( m_pMutex );
	SetSessionActiveImpl( sessionID, active );
}

void SessionMgr::SetSessionActiveImpl( int sessionID, bool active )
{
	SESSION_ACTIVE_STATUS_MAP::iterator it = m_SessionActiveStatus.find( sessionID );
	if( m_SessionActiveStatus.end() != it )
	{
		it->second = active;
	}
}

}

#ifndef MSG_H
#define MSG_H

namespace Crux
{
/*
 * net msg
 */

class Session;

const size_t MSG_HEADER_SIZE = 2;

enum MsgType
{
	MSG_INVALID,
	MSG_ACCEPT_EXTERNAL, //accept a new external session
	MSG_ACCEPT_INTERNAL, //accept a new internal session
	MSG_CONNECT, //connect to server
	MSG_CLOSE, 	 //close a session
	MSG_RECV, 	 //recv data from a session
	MSG_SHUTDOWN, //shut down net
};

struct Msg
{
public:
	Msg( size_t size, void* p );
	MsgType m_Type;
	int m_SessionID;
	short m_Size;
	void* m_pData;	
	Session* m_pSession;
};

inline
Msg::Msg( size_t size, void* p )
: m_Type(MSG_INVALID), m_SessionID(0),
m_Size( static_cast<short>(size) ), m_pData(p),
m_pSession(NULL)
{}

} //namespace Crux

#endif //MSG_H

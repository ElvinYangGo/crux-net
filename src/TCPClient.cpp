#include "TCPClient.h"
#include "Session.h"

namespace Crux
{

TCPClient::TCPClient()
: m_Connected(false), m_pSession( NULL )
{
}

TCPClient::~TCPClient()
{
}

void TCPClient::Disconnect()
{
	m_Connected = false;
	m_pSession->Disconnect();
}

bool TCPClient::Connected() const
{
	return m_Connected;
}

void TCPClient::SetConnected( bool connected )
{
	m_Connected = connected;
}

void TCPClient::SetSession( Session* pSession )
{
	m_pSession = pSession;
}

bool TCPClient::SendMsg( int sessionID, void* pData, size_t size )
{
	return m_pSession->SendMsg( sessionID, pData, size );
}

size_t TCPClient::GetSendDataSizeTotal() const
{
	return m_pSession->GetSendDataSizeTotal();
}

size_t TCPClient::GetRecvDataSizeTotal() const
{
	return m_pSession->GetRecvDataSizeTotal();
}

} //namespace Crux

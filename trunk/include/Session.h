#ifndef SESSION_H
#define SESSION_H

#include <string>
#include "Mutex.h"
#include "Buffer.h"

#ifdef WIN32
#include <WinSock2.h>
#include "Win/AsyncCompleteOperation.h"
#include "Win/RecvHandler.h"
#include "Win/SendHandler.h"
#include "Win/CloseHandler.h"

#elif defined(LINUX)
#include "Linux/SessionCompleteOperation.h"

typedef int SOCKET;

#endif


namespace Crux
{

struct Msg;

/**
 * a session
 */

class Session
{
public:
	Session( int id, SOCKET sock, const std::string& ip, int port );
	~Session();

public:
	Mutex* GetRecvLock();
	Mutex* GetSendLock();
	void RecvLock();
	void RecvUnLock();
	void SendLock();
	void SendUnLock();

	bool Sending() const;
	void SetSending( bool sending );
	size_t GetSendingDataSize() const;

	Buffer* GetRecvBuffer();
	Buffer* GetSendBuffer();

public:
	int GetID() const;
	SOCKET GetSock() const;
	const std::string& GetIP() const;
	int GetPort() const;

#ifdef WIN32
public:
	WSABUF* GetRecvWSABUF();
	WSABUF* GetSendWSABUF();

	AsyncCompleteOperation* GetRecvCompleteOperation();
	AsyncCompleteOperation* GetSendCompleteOperation();

	bool PostRecvOperation();
	bool PostSendOperation();

private:
	WSABUF m_RecvWSABUF;
	WSABUF m_SendWSABUF;
 	OVERLAPPED m_RecvOverlapped;
 	OVERLAPPED m_SendOverlapped;
	RecvHandler m_RecvHandler;
	SendHandler m_SendHandler;
	CloseHandler m_CloseHandler;
 	AsyncCompleteOperation m_RecvCompleteOperation;
 	AsyncCompleteOperation m_SendCompleteOperation;

#elif defined(LINUX)

public:
	AsyncCompleteOperation* GetAsyncCompleteOperation();

private:
	SessionCompleteOperation m_SessionCompleteOperation;
#endif

public:
	void ExtractMsg();

public:
	bool SendMsg( int sessionID, void* pData, size_t size );
	
	void Disconnect();

public: 	//for statistics
	size_t GetSendDataSizeTotal() const;
	void IncreaseSendDataSizeTotal( size_t sizeIncreased );

	size_t GetRecvDataSizeTotal() const;
	void IncreaseRecvDataSizeTotal( size_t sizeIncreased );

private: 	//for statistics
	size_t m_SendDataSizeTotal;
	size_t m_RecvDataSizeTotal;

	//todo: calculate the average size per minute
//	size_t m_SendDataSizeAverage;
//	size_t m_RecvDataSizeAverage;

private:
	int m_ID;
	SOCKET m_Sock;
	std::string m_IP;
	int m_Port;

private:
	Buffer m_SendBuffer;
	Buffer m_RecvBuffer;

	Mutex m_RecvMutex;
	Mutex m_SendMutex;
	
	bool m_Sending;

	//the data's size in sending
	size_t m_SendingDataSize;
};

} //namespace Crux

#endif 	//SESSION_H

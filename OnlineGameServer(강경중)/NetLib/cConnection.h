#pragma once
#include "cQueue.h"
#include "cRingBuffer.h"
class NETLIB_API cConnection : public cMonitor
{
public:
	cConnection(void);
	~cConnection(void);

public:
	void 	InitializeConnection();
	bool 	CreateConnection( INITCONFIG &initConfig );
	bool 	CloseConnection ( bool bForce=FALSE );
	bool	ConnectTo( char* szIp , unsigned short usPort );
	bool 	BindIOCP( HANDLE& hIOCP );
	bool 	RecvPost( char* pNext , DWORD dwRemain );
	bool 	SendPost( int nSendSize );
	void 	SetSocket(SOCKET socket) { m_socket = socket; }
	SOCKET 	GetSocket() { return m_socket; }
	bool 	BindAcceptExSock();
	char* 	PrepareSendPacket(int slen);
	bool 	ReleaseRecvPacket();
	bool 	ReleaseSendPacket( LPOVERLAPPED_EX lpSendOverlappedEx = NULL );
	
	inline void  SetConnectionIp(char* Ip) { memcpy( m_szIp , Ip , MAX_IP_LENGTH );  }
	inline BYTE*  GetConnectionIp() { return m_szIp; }
	
	inline int  GetIndex() { return m_nIndex; }

	inline int  GetRecvBufSize() { return m_nRecvBufSize; }
    inline int  GetSendBufSize() { return m_nSendBufSize; }

	inline int  GetRecvIoRefCount() { return m_dwRecvIoRefCount; }
	inline int  GetSendIoRefCount() { return m_dwSendIoRefCount; }
	inline int  GetAcceptIoRefCount() { return m_dwAcceptIoRefCount; }

	inline int  IncrementRecvIoRefCount() 
	{ return InterlockedIncrement( (LPLONG)&m_dwRecvIoRefCount ); }
	inline int  IncrementSendIoRefCount() 
	{ return InterlockedIncrement( (LPLONG)&m_dwSendIoRefCount ); }
	inline int  IncrementAcceptIoRefCount() 
	{ return InterlockedIncrement( (LPLONG)&m_dwAcceptIoRefCount ); }
	inline int  DecrementRecvIoRefCount() 
	{ return ( m_dwRecvIoRefCount ? InterlockedDecrement( (LPLONG)&m_dwRecvIoRefCount ) : 0); }
	inline int  DecrementSendIoRefCount() 
	{ return ( m_dwSendIoRefCount ? InterlockedDecrement( (LPLONG)&m_dwSendIoRefCount ) : 0); }
	inline int  DecrementAcceptIoRefCount() 
	{ return ( m_dwAcceptIoRefCount ? InterlockedDecrement( (LPLONG)&m_dwAcceptIoRefCount ) : 0); }


public:

	//Overlapped I/O ��û�� ���� ����
	LPOVERLAPPED_EX		m_lpRecvOverlappedEx;
	LPOVERLAPPED_EX		m_lpSendOverlappedEx;
	//Ŭ���̾�Ʈ�� ������ �ۼ����� ���� �� ����
	cRingBuffer		m_ringRecvBuffer;
	cRingBuffer		m_ringSendBuffer;
	//Ŭ���̾�Ʈ �ּҸ� �ޱ����� ����
	char			m_szAddressBuf[ 1024 ];
	//Ŭ���̾�Ʈ�� ���� ���ᰡ �Ǿ����� ����
	BOOL			m_bIsClosed;
	//Ŭ���̾�Ʈ�� ������ �Ǿ��ִ��� ����
	BOOL			m_bIsConnect;
	//���� Overlapped I/O ���� �۾��� �ϰ� �ִ��� ����
	BOOL			m_bIsSend;

private:
	SOCKET		m_socket;
	//�ѹ��� ������ �� �ִ� �������� �ִ� ũ��
	int			m_nRecvBufSize;
	//�ѹ��� �۽��� �� �ִ� �������� �ִ� ũ��
	int			m_nSendBufSize;
	
	BYTE		m_szIp[MAX_IP_LENGTH];	
	int			m_nIndex;
	
	cMonitor	m_csConnection;
	
	SOCKET		m_sockListener;
	HANDLE		m_hIOCP;
	//Overlapped i/o�۾��� ��û�� ����
	DWORD		m_dwSendIoRefCount;
	DWORD		m_dwRecvIoRefCount;
	DWORD		m_dwAcceptIoRefCount;

private:
	
	cConnection(const cConnection &rhs);
    cConnection &operator=(const cConnection &rhs);
};


#pragma once

class NETLIB_API cIocpServer : public cMonitor 
{
public:
	cIocpServer(void);
	~cIocpServer(void);

	///////////////////////////////   �ʱ�ȭ �Լ�  //////////////////////////
	bool				InitializeSocket();

	//////////////////////////////  ���� �Լ� /////////////////////////////////
	// Network I/O�� ó���ϴ� Thread
	void	WorkerThread();
	// ������ �ִ� ��Ŷ�� ó���ϴ� Thread
	void	ProcessThread();
	// client�� ���� ���� ó��
	bool	CloseConnection( cConnection* lpConnection );
	
	bool	ProcessPacket( cConnection* lpConnection , char* pCurrent , DWORD dwCurrentSize );
	
	virtual bool	ServerStart( INITCONFIG &initConfig );
	virtual bool	ServerOff();
	SOCKET			GetListenSocket() { return m_ListenSock; }
	unsigned short	GetServerPort() { return m_usPort; }
	char*			GetServerIp() { return m_szIp; }
	inline HANDLE	GetWorkerIOCP() { return m_hWorkerIOCP; }
	void	DoAccept( LPOVERLAPPED_EX lpOverlappedEx );
	void	DoRecv( LPOVERLAPPED_EX lpOverlappedEx , DWORD dwIoSize );
	void	DoSend( LPOVERLAPPED_EX lpOverlappedEx , DWORD dwIoSize );
	LPPROCESSPACKET	GetProcessPacket( eOperationType operationType, LPARAM lParam, WPARAM wParam );
	void	ClearProcessPacket( LPPROCESSPACKET lpProcessPacket );
	///////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////  ���� ���� �Լ�   ///////////////////////////////
	//client�� ���ӵɶ� ȣ��Ǵ� �Լ�
	virtual	bool	OnAccept( cConnection *lpConnection ) = 0;
	//client���� packet�� �����Ͽ� ���� �� �ִ� ��Ŷ�� ó���� �� ȣ��Ǵ� �Լ�
	virtual	bool	OnRecv(cConnection* lpConnection, DWORD dwSize , char* pRecvedMsg) = 0;
	//client���� packet�� �����Ͽ� ���� �� ���� ��Ŷ�� ó���� �� ȣ��Ǵ� �Լ�
	virtual	bool	OnRecvImmediately(cConnection* lpConnection, DWORD dwSize , char* pRecvedMsg) = 0;
	//client ���� ����� ȣ��Ǵ� �Լ�
	virtual	void	OnClose(cConnection* lpConnection) = 0;
	//�������� ProcessThread�� �ƴ� �ٸ� �����忡�� �߻���Ų 
	//�޽����� ���� ���ְ� ó���Ǿ� �Ѵٸ� �� �Լ��� ���.
	virtual bool	OnSystemMsg( cConnection* lpConnection , DWORD dwMsgType , LPARAM lParam ) = 0;
	/////////////////////////////////////////////////////////////////////////////////////
	
	static cIocpServer* GetIocpServer() { return cIocpServer::m_pIocpServer; }
	static cIocpServer* m_pIocpServer;

private:

	bool				CreateProcessThreads();
	bool				CreateWorkerThreads();
	void				GetProperThreadsCount();
	bool				CreateWorkerIOCP();
	bool				CreateProcessIOCP();
	bool				CreateListenSock();
	// No copies do not implement
    cIocpServer(const cIocpServer &rhs);
    cIocpServer &operator=(const cIocpServer &rhs);

protected:
	
	SOCKET				m_ListenSock;

	HANDLE				m_hWorkerIOCP;
	HANDLE				m_hProcessIOCP;
	
	HANDLE				m_hWorkerThread[ MAX_WORKER_THREAD ];
	HANDLE				m_hProcessThread[ MAX_PROCESS_THREAD ];

	unsigned short		m_usPort;
	char				m_szIp[MAX_IP_LENGTH];

	DWORD				m_dwTimeTick;
	DWORD				m_dwWorkerThreadCount;
	DWORD				m_dwProcessThreadCount;

	bool				m_bWorkThreadFlag;
	bool				m_bProcessThreadFlag;

	LPPROCESSPACKET		m_lpProcessPacket;
	DWORD				m_dwProcessPacketCnt;
};

inline cIocpServer* IocpServer()
{
	return cIocpServer::GetIocpServer();
}


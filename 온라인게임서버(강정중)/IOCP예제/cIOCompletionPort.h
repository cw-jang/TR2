// cIOCompletionPort.h

#pragma once 

class CIOCompletionPortDlg;
#define MAX_SOCKBUF			1024	// ��Ŷũ��
#define MAX_CLIENT			100		// �� �����Ҽ� �ִ� Ŭ���̾�Ʈ ��
#define MAX_WORKERTHREAD	4		// ������ Ǯ�� ���� ������ ��

enum enumOperation { OP_RECV, OP_SEND };


// WSAOVERLAPPED ����ü�� Ȯ����Ѽ� �ʿ��� ������ �� �־���.
struct stOverlappedEx {
	WSAOVERLAPPED m_wsaOverlapped;		// Overlapped I/O����ü
	SOCKET m_socketClient;				// Ŭ���̾�Ʈ ����
	WSABUF m_wsaBuf;					// Overlapped I/O�۾� ����
	char m_szBuf[MAX_SOCKBUF];			// ������ ����
	enumOperation m_eOperation;			// �۾� ���� ����
};


// Ŭ���̾�Ʈ ������ ������� ����ü
struct stClientInfo {
	SOCKET m_socketClient;					// Client�� ����Ǵ� ����
	stOverlappedEx m_stRecvOverlappedEx;	// RECV Overlapped I/O�۾��� ���� ����
	stOverlappedEx m_stSendOverlappedEx;	// SEND Overlapped I/O�۾��� ���� ����

	stClientInfo()
	{
		ZeroMemory( &m_stRecvOverlappedEx, sizeof(m_stRecvOverlappedEx) );
		ZeroMemory( &m_stSendOverlappedEx, sizeof(m_stSendOverlappedEx) );
	}
};

class cIOCompletionPort
{
public:
	cIOCompletionPort();
	~cIOCompletionPort();

	// ���� Ŭ���̾�Ʈ �����ռ�
	// ������ �ʱ�ȭ�ϴ� �Լ�
	bool InitSocket();
	// ������ ������ �����Ų��.
	void CloseSocket( stClientInfo* pClientInfo, bool bIsForce=false );

	// ������ �Լ�
	// ������ �ּ������� ���ϰ� �����Ű�� ���� ��û�� �ޱ� ����
	// ������ ����ϴ� �Լ�
	bool BindandListen( int nBindPort );
	// ���� ��û�� �����ϰ� �޼����� �޾Ƽ� ó���ϴ� �Լ�
	bool StartServer();
	// WaitingThread Queue���� ����� ��������� ����
	bool CreateWorkerThread();
	// accept��û�� ó���ϴ� ������ ����
	bool CreateAccepterThread();
	// ������� �ʴ� Ŭ���̾�Ʈ ���� ����ü�� ��ȯ�Ѵ�.
	stClientInfo* GetEmptyClientInfo();

	// CompletionPort��ü�� ���ϰ� CompletionKey�� �����Ű�� ������ �Ѵ�.
	bool BindIOCompletionPort( stClientInfo* pClientInfo );
	// WSARecv Overlapped I/O �۾��� ��Ų��.
	bool BindRecv( stClientInfo* pClientInfo );
	// WSASend Overlapped I/O �۾��� ��Ų��
	bool SendMsg( stClientInfo* pClientInfo, char* pMsg, int nLen );
	// Overlapped I/O �۾��� ���� �Ϸ� �뺸�� �޾� �׿� �ش��ϴ� ó���� �ϴ� �Լ�
	void WorkerThread();
	// ������� ������ �޴� ������
	void AccepterThread();

	// ���� �������� �����͸� ����
	void SetMainDlg( CIOCompletionPortDlg* pMainDlg ) { m_pMainDlg = pMainDlg; }
	CIOCompletionPortDlg* GetMainDlg() { return m_pMainDlg; }

	// �����Ǿ� �ִ� �����带 �ı��Ѵ�
	void DestroyThread();

private:
	// Ŭ���̾�Ʈ ���� ���� ����ü
	stClientInfo* m_pClientInfo;
	// Ŭ���̾�Ʈ�� ������ �ޱ� ���� ���� ����
	SOCKET m_socketListen;
	// ���ӵǾ� �ִ� Ŭ���̾�Ʈ ��
	int m_nClientCnt;
	// ���� ������ ������
	CIOCompletionPortDlg* m_pMainDlg;
	// �۾� ������ �ڵ� -- WaitingThreadQueue�� �� ������
	HANDLE m_hWorkerThread[MAX_WORKERTHREAD];
	// ���� �������ڵ�
	HANDLE m_hAccepterThread;
	// CompletionPort��ü�ڵ�
	HANDLE m_hIOCP;
	// �۾������� �����÷���
	bool m_bWorkerRun;
	// ���Ӿ����� �����÷���
	bool m_bAccepterRun;
	// ���Ϲ���
	char m_szSocketBuf[1024];

};
#pragma once
class COverlappedCallbackDlg;
#define MAX_SOCKBUF 1024

//Overlapped I/O�۾� ���� ����
enum enumOperation{
	OP_RECV,
	OP_SEND
};

//WSAOVERLAPPED����ü�� Ȯ�� ���Ѽ� �ʿ��� ������ �� �־���.
struct stOverlappedEx{

	WSAOVERLAPPED m_wsaOverlapped;		//Overlapped I/O����ü
	SOCKET		m_socketClient;			//Ŭ���̾�Ʈ ����
	WSABUF		m_wsaBuf;				//Overlapped I/O�۾� ����
	char		m_szBuf[ MAX_SOCKBUF ]; //������ ����
	enumOperation m_eOperation;			//�۾� ���� ����
	void*		m_pOverlappedCallback;	//�ݹ� �Լ� ������
};
	
class cOverlappedCallback
{
public:
	cOverlappedCallback(void);
	~cOverlappedCallback(void);
	//------���� Ŭ���̾�Ʈ �����Լ�-------//
	//������ �ʱ�ȭ�ϴ� �Լ�
	bool InitSocket();
	//������ ������ ���� ��Ų��.
	void CloseSocket( SOCKET socketClose , bool bIsForce = false );
	
	//------������ �Լ�-------//
	//������ �ּ������� ���ϰ� �����Ű�� ���� ��û�� �ޱ� ���� �� ������ ����ϴ� �Լ�
	bool BindandListen( int nBindPort );
	//���� ��û�� �����ϰ� �޼����� �޾Ƽ� ó���ϴ� �Լ�
	bool StartServer();
	//accept��û�� ó���ϴ� ������ ����
	bool CreateAccepterThread();
	//WSARecv Overlapped I/O �۾��� ��Ų��.
	bool BindRecv( SOCKET socket );
	//WSASend Overlapped I/O�۾��� ��Ų��.
	bool SendMsg( SOCKET socket , char* pMsg , int nLen );
	//������� ������ �޴� ������
	void AccepterThread();
	
	void SetMainDlg( COverlappedCallbackDlg* pMainDlg ) { m_pMainDlg = pMainDlg; }
	COverlappedCallbackDlg* GetMainDlg() { return m_pMainDlg; }
	
	//�����Ǿ��ִ� �����带 �ı��Ѵ�.
	void DestroyThread();
	
private:

	//���� �Ǿ��ִ� Ŭ���̾�Ʈ ��
	int			m_nClientCnt;
	//���� ������ ������
	COverlappedCallbackDlg* m_pMainDlg;
	//���� ������ �ڵ�
	HANDLE		m_hAccepterThread;
	//���� ������ ���� �÷���
	bool		m_bAccepterRun;
	SOCKET		m_sockListen;
	//���� ����
	char		m_szSocketBuf[ 1024 ];
};

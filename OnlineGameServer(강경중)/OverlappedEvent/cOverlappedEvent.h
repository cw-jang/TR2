#pragma once
class COverlappedEventDlg;
#define MAX_SOCKBUF 1024
//Overlapped I/O�۾��� ����
enum enumOperation{
	OP_RECV,
	OP_SEND
};
//WSAOVERLAPPED����ü�� Ȯ�� ���Ѽ� �ʿ��� ������ �� �־���.
struct stOverlappedEx{
	WSAOVERLAPPED m_wsaOverlapped;      
	int m_nIdx;							//stClientInfo ����ü �迭 �ε���
	WSABUF m_wsaBuf;
	char m_szBuf[ MAX_SOCKBUF ];
	enumOperation m_eOperation;
};
	
//Ŭ���̾�Ʈ ������ ������� ����ü���� 0��° �迭���� ���̷� ������ ������ ���ο� ���ӿ� ���� 
//�̺�Ʈ������ ���� WSAWaitForMultipleEvents�� �ٽ� �ɾ��ش�.
struct stClientInfo{
	//Cliet�� ����Ǵ� ����
	SOCKET		m_socketClient[ WSA_MAXIMUM_WAIT_EVENTS ];
	//�̺�Ʈ ������ ���� �̺�Ʈ ��ü
	WSAEVENT    m_eventHandle[ WSA_MAXIMUM_WAIT_EVENTS ];
	stOverlappedEx	m_stOverlappedEx[ WSA_MAXIMUM_WAIT_EVENTS ];
};

class cOverlappedEvent
{
public:
	cOverlappedEvent(void);
	~cOverlappedEvent(void);
	
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

	//Overlapped I/O�۾��� ���� �����带 ����
	bool CreateWokerThread();
	//accept��û�� ó���ϴ� ������ ����
	bool CreateAccepterThread();

	//������ ���� index��ȯ
	int GetEmptyIndex();

	//WSARecv Overlapped I/O �۾��� ��Ų��.
	bool BindRecv( int nIdx );

	//WSASend Overlapped I/O�۾��� ��Ų��.
	bool SendMsg( int nIdx , char* pMsg , int nLen );

	//Overlapped I/O�۾��� ���� �Ϸ� �뺸�� �޾� 
	//�׿� �ش��ϴ� ó���� �ϴ� �Լ�
	void WokerThread();
	//������� ������ �޴� ������
	void AccepterThread();

	//Overlapped I/O �Ϸῡ ���� ��� ó��
	void OverlappedResult( int nIdx );

	void SetMainDlg( COverlappedEventDlg* pMainDlg ) { m_pMainDlg = pMainDlg; }

	//�����Ǿ��ִ� �����带 �ı��Ѵ�.
	void DestroyThread();

private:

	//Ŭ���̾�Ʈ ���� ���� ����ü
	stClientInfo m_stClientInfo;

	//���� �Ǿ��ִ� Ŭ���̾�Ʈ ��
	int			m_nClientCnt;
	//���� ������ ������
	COverlappedEventDlg* m_pMainDlg;

	//�۾� ������ �ڵ�
	HANDLE		m_hWorkerThread;
	//���� ������ �ڵ�
	HANDLE		m_hAccepterThread;
	//�۾� ������ ���� �÷���
	bool		m_bWorkerRun;
	//���� ������ ���� �÷���
	bool		m_bAccepterRun;
	
	//���� ����
	char		m_szSocketBuf[ 1024 ];
};

#pragma once
class CEventSelectDlg;
#define MAX_SOCKBUF 1024

	
//Ŭ���̾�Ʈ ������ ������� ����ü���� 0��° �迭���� ���̷� ������ ������ ���ο� ���ӿ� ���� 
//�̺�Ʈ������ ���� WSAWaitForMultipleEvents�� �ٽ� �ɾ��ش�.
struct stClientInfo{
	//�̺�Ʈ ������ ���� �̺�Ʈ ��ü
	WSAEVENT    m_eventHandle[ WSA_MAXIMUM_WAIT_EVENTS ];
	//Cliet�� ����Ǵ� ����
	SOCKET		m_socketClient[ WSA_MAXIMUM_WAIT_EVENTS ];
};

class cEventSelect
{
public:
	cEventSelect(void);
	~cEventSelect(void);
	
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
	//������ ���� index��ȯ
	int GetEmptyIndex();
	
	//�۾� ������, ��Ʈ��ũ �̺�Ʈ�� �����Ͽ� �ش� �۾�ó��
    void WokerThread();
	void DoAccept( DWORD dwObjIdx );
	void DoRecv( DWORD dwObjIdx );
	
	void SetMainDlg( CEventSelectDlg* pMainDlg ) { m_pMainDlg = pMainDlg; }

	//�����Ǿ��ִ� �����带 �ı��Ѵ�.
	void DestroyThread();

private:
	//Ŭ���̾�Ʈ ���� ���� ����ü
	stClientInfo m_stClientInfo;
	//���� �Ǿ��ִ� Ŭ���̾�Ʈ ��
	int			m_nClientCnt;
	//���� ������ ������
	CEventSelectDlg* m_pMainDlg;
	//�۾� ������ �ڵ�
	HANDLE		m_hWorkerThread;
	//�۾� ������ ���� �÷���
	bool		m_bWorkerRun;
	//���� ����
	char		m_szSocketBuf[ 1024 ];
};

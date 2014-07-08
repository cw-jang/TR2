#include <Windows.h>
#include "cIOCompletionPort.h"
#include "IOCompletionPortDlg.h"

// WSARecv�� WSASend�� Overlapped I/O �۾� ó���� ���� ������
unsigned int WINAPI CallWorkerThread(LPVOID p)
{
	cIOCompletionPort* pOverlappedEvent = (cIOCompletionPort*)p;
	pOverlappedEvent->WorkerThread();
	return 0;
}

// Clinet�� ������ �ޱ����� ������
unsigned int WINAPI CallAccepterThread(LPVOID p)
{
	cIOCompletionPort* pOverlappedEvent = (cIOCompletionPort*) p;
	pOverlappedEvent->AccepterThread();
	return 0;
}

cIOCompletionPort::cIOCompletionPort(void)
{
	///////////////////////////////////
	// ��� ��� ������ �ʱ�ȭ
	m_pMainDlg = NULL;
	m_bWorkerRun = true;
	m_bAccepterRun = true;
	m_nClientCnt = 0;
	m_hAccepterThread = NULL;
	m_hIOCP = NULL;
	m_socketListen = INVALID_SOCKET;
	ZeroMemory( m_szSocketBuf, 1024 );
	for(int i=0; MAX_WORKERTHREAD; i++)
		m_hWorkerThread[i] = NULL;

	// client ������ ������ ����ü�� ����
	m_pClientInfo = new stClientInfo[MAX_CLIENT];
}

cIOCompletionPort::~cIOCompletionPort(void)
{
	// ������ ����� ������.
	WSACleanup();
	
	// �� ����� ��ü�� ����
	if( m_pClientInfo )
	{
		delete [] m_pClientInfo;
		m_pClientInfo = NULL;
	}
}

// ������ �ʱ�ȭ�ϴ� �Լ�
bool cIOCompletionPort::InitSocket()
{
	WSADATA wsaData;

	// ������ ���� 2,2�� �ʱ�ȭ�Ѵ�
	int nRet = WSAStartup( MAKEWORD(2,2), &wsaData );
	if( 0 != nRet )
	{
		m_pMainDlg->OutputMsg("[����] WSAStartup() �Լ� ����: %d", WSAGetLastError() );
		return false;
	}

	// ���������� TCP, Overlapped I/O������ ����
	m_socketListen = WSASocket( AF_INET, SOCK_STREAM,
								IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED );

	if( INVALID_SOCKET == m_socketListen )
	{
		m_pMainDlg->OutputMsg("[����] socket()�Լ����� : %d", WSAGetLastError() );
		return false;
	}

	m_pMainDlg->OutputMsg("���� �ʱ�ȭ ����");
	return true;
}

// �ش� ������ �����Ų��.
void cIOCompletionPort::CloseSocket( stClientInfo* pClientInfo, bool bIsForce )
{
	struct linger stLinger = {0,0}; // SO_DONTLINGER�� ����

	// bIsForce�� true�̸� SO_LINGER, timeout=0���� �����Ͽ�
	// ���� ���� ��Ų��. ����: ������ �ս��� ���� �� ����
	if( true == bIsForce )
		stLinger.l_onoff = 1;

	// socketClose ������ ������ �ۼ����� ��� �ߴܽ�Ų��.
	shutdown( pClientInfo->m_socketClient, SD_BOTH );

	// ���� �ɼ��� �����Ѵ�.
	setsockopt( pClientInfo->m_socketClient, SOL_SOCKET,
				SO_LINGER, (char*)&stLinger, sizeof(stLinger) );

	// ���� ������ �����Ų��.
	closesocket( pClientInfo->m_socketClient );
	pClientInfo->m_socketClient = INVALID_SOCKET;
}

// ������ �ּ� ������ ���ϰ� �����Ű�� ���� ��û�� �ޱ� ���� ������ ����ϴ� �Լ�
bool cIOCompletionPort:: BindandListen( int nBindPort )
{
}
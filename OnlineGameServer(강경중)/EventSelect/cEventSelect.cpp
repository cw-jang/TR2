#include "StdAfx.h"
#include ".\ceventselect.h"
#include "EventSelectDlg.h"

//WSARecv�� WSASend�� Overlapped I/O �۾� ó��
unsigned int WINAPI CallWorkerThread(LPVOID p)
{
	cEventSelect* pEventSelect = (cEventSelect*)p;
	pEventSelect->WokerThread();
	return 0;
}

cEventSelect::cEventSelect(void)
{
	//�������� �ʱ�ȭ�Ѵ�.
	m_pMainDlg = NULL;
	m_bWorkerRun = true;
	m_nClientCnt = 0;
	m_hWorkerThread = NULL;
	ZeroMemory( m_szSocketBuf , 1024 );
    
	//Ŭ���̾�Ʈ ����ü�� �ʱ�ȭ
	for( int i = 0; i < WSA_MAXIMUM_WAIT_EVENTS; i++ )
	{
		m_stClientInfo.m_socketClient[ i ] = INVALID_SOCKET;
		m_stClientInfo.m_eventHandle[ i ] = WSACreateEvent();
	}
}

cEventSelect::~cEventSelect(void)
{
	//������ ����� ������.
	WSACleanup();
}

//������ �ʱ�ȭ�ϴ� �Լ�
bool cEventSelect::InitSocket()
{
	WSADATA wsaData;
	//������ ���� 2,2�� �ʱ�ȭ �Ѵ�.
	int nRet = WSAStartup( MAKEWORD(2,2) , &wsaData );
	if( 0 != nRet )
	{
		m_pMainDlg->OutputMsg("[����] WSAStartup()�Լ� ���� : %d", WSAGetLastError() );
		return false;
	}

	//���������� TCP , Overlapped I/O ������ ����
	m_stClientInfo.m_socketClient[ 0 ] = WSASocket( AF_INET , SOCK_STREAM 
		, IPPROTO_TCP , NULL , NULL , WSA_FLAG_OVERLAPPED );

	if( INVALID_SOCKET == m_stClientInfo.m_socketClient[ 0 ] )
	{
		m_pMainDlg->OutputMsg("[����] socket()�Լ� ���� : %d", WSAGetLastError() );
		return false;
	}
	
	//������ �ڵ� ����
	m_pMainDlg->OutputMsg("���� �ʱ�ȭ ����");
	return true;
}

//�ش� ������ ���� ��Ų��.
void cEventSelect::CloseSocket( SOCKET socketClose , bool bIsForce )
{
	struct linger stLinger = {0, 0};	// SO_DONTLINGER�� ����

	// bIsForce�� true�̸� SO_LINGER, timeout = 0���� �����Ͽ�
	// ���� ���� ��Ų��. ���� : ������ �ս��� ������ ���� 
	if( true == bIsForce )
		stLinger.l_onoff = 1; 

	//socketClose������ ������ �ۼ����� ��� �ߴ� ��Ų��.
	shutdown( socketClose, SD_BOTH );
	//���� �ɼ��� �����Ѵ�.
	setsockopt( socketClose, SOL_SOCKET, SO_LINGER, (char *)&stLinger, sizeof(stLinger) );
	//���� ������ ���� ��Ų��. 
	closesocket( socketClose );
	
	socketClose = INVALID_SOCKET;
}

//������ �ּ������� ���ϰ� �����Ű�� ���� ��û�� �ޱ� ���� �� ������ ����ϴ� �Լ�
bool cEventSelect::BindandListen( int nBindPort )
{
	SOCKADDR_IN		stServerAddr;
	stServerAddr.sin_family = AF_INET;
    //���� ��Ʈ�� �����Ѵ�.
	stServerAddr.sin_port = htons( nBindPort );
	//� �ּҿ��� ������ �����̶� �޾Ƶ��̰ڴ�.
	//���� ������� �̷��� �����Ѵ�. ���� �� �����ǿ����� ������ �ް� �ʹٸ�
	//�� �ּҸ� inet_addr�Լ��� �̿��� ������ �ȴ�.
	stServerAddr.sin_addr.s_addr = htonl( INADDR_ANY );

	//������ ������ ���� �ּ� ������ m_socketListen ������ �����Ѵ�.
	int nRet = bind( m_stClientInfo.m_socketClient[ 0 ] 
	, (SOCKADDR*) &stServerAddr , sizeof( SOCKADDR_IN ) );
	if( 0 != nRet )
	{
		m_pMainDlg->OutputMsg("[����] bind()�Լ� ���� : %d", WSAGetLastError() );
		return false;
	}

	nRet = WSAEventSelect( m_stClientInfo.m_socketClient[ 0 ] ,
		m_stClientInfo.m_eventHandle[ 0 ] ,	FD_ACCEPT | FD_CLOSE );

	if( SOCKET_ERROR == nRet )
	{
		m_pMainDlg->OutputMsg("[����] WSAEventSelect()�Լ� ���� : %d", WSAGetLastError() );
		return false;
	}

	//���� ��û�� �޾Ƶ��̱� ���� m_socketListen������ ����ϰ� ���Ӵ��ť�� 5���� ���� �Ѵ�.
	nRet = listen( m_stClientInfo.m_socketClient[ 0 ] , 5 );
	if( 0 != nRet )
	{
		m_pMainDlg->OutputMsg("[����] listen()�Լ� ���� : %d", WSAGetLastError() );
		return false;
	}
	
	m_pMainDlg->OutputMsg("���� ��� ����..");

	return true;
}

bool cEventSelect::CreateWokerThread()
{
	unsigned int uiThreadId = 0;

	m_hWorkerThread = (HANDLE)_beginthreadex(NULL, 0, &CallWorkerThread, this, CREATE_SUSPENDED , &uiThreadId);
	if(m_hWorkerThread == NULL)
	{
		m_pMainDlg->OutputMsg("WorkerThread Creation Failed: %u", GetLastError());	
		return false;
	}

	ResumeThread( m_hWorkerThread );
	m_pMainDlg->OutputMsg("WokerThread ����..");
	return true;
}

//������ ���� Index��ȯ
int cEventSelect::GetEmptyIndex()
{
	//0��° �迭�� �������ſ� �̺�Ʈ�̴�.
	for( int i = 1; i < WSA_MAXIMUM_WAIT_EVENTS ; i++ )
	{
		if( INVALID_SOCKET == m_stClientInfo.m_socketClient[ i ] )
			return  i;
	}
	return -1;
}

bool cEventSelect::StartServer()
{
	//���ӵ� Ŭ���̾�Ʈ �ּ� ������ ������ ����ü
	bool bRet = CreateWokerThread();
	if( false == bRet )
		return false;
	
	m_pMainDlg->OutputMsg("���� ����");
	return true;
}

void cEventSelect::WokerThread()
{
	WSANETWORKEVENTS wsaNetworkEvents;
	
	while( m_bWorkerRun )
	{
		//////////////////////////////////////////////////////////////////
		//��û�� Overlapped I/O�۾��� �Ϸ�Ǿ����� �̺�Ʈ�� ��ٸ���.
		DWORD dwObjIdx = WSAWaitForMultipleEvents( WSA_MAXIMUM_WAIT_EVENTS , 
			m_stClientInfo.m_eventHandle ,
			FALSE ,
			INFINITE,
			FALSE );
		//���� �߻�
		if( WSA_WAIT_FAILED == dwObjIdx )
		{
			m_pMainDlg->OutputMsg("[����] WSAWaitForMultipleEvents ���� : %d" ,	WSAGetLastError() );
			break;
		}
		WSAEnumNetworkEvents( m_stClientInfo.m_socketClient[ dwObjIdx ] ,
			m_stClientInfo.m_eventHandle[ dwObjIdx ] ,
			&wsaNetworkEvents );

		//�����尡 ���� �̺�Ʈ�� �޾Ҵٸ�
		if( false == m_bWorkerRun && 0 == dwObjIdx )
			break;
		//���� ���� �̺�Ʈ�� �߻�
		if( wsaNetworkEvents.lNetworkEvents & FD_ACCEPT )
		{
			if( 0 != wsaNetworkEvents.iErrorCode[ FD_ACCEPT_BIT ] )
			{
				m_pMainDlg->OutputMsg("[����] WSAEnumNetworkEvents ���� : %d", WSAGetLastError() );
			}
			DoAccept( dwObjIdx );
			
		}
		//�����Ͱ� �����ߴٴ� �̺�Ʈ �߻�
		else if( wsaNetworkEvents.lNetworkEvents & FD_READ )
		{
			if( 0 != wsaNetworkEvents.iErrorCode[ FD_READ_BIT ] )
			{
				m_pMainDlg->OutputMsg("[����] WSAEnumNetworkEvents ���� : %d", WSAGetLastError() );
				break;
			}
			DoRecv( dwObjIdx );
		}
		//������ ����ٴ� �̺�Ʈ �߻�
		else if( wsaNetworkEvents.lNetworkEvents & FD_CLOSE )
		{
			CloseSocket( m_stClientInfo.m_socketClient[ dwObjIdx ] );
			m_pMainDlg->OutputMsg("[����] socket [%d]Ŭ���̾�Ʈ ����", 
				m_stClientInfo.m_socketClient[ dwObjIdx ] );
			m_stClientInfo.m_socketClient[ dwObjIdx ] = INVALID_SOCKET;
		}
	}
}
void cEventSelect::DoAccept( DWORD dwObjIdx )
{
	SOCKADDR_IN		stClientAddr;
	int nAddrLen = sizeof( SOCKADDR_IN );
	//������ ���� ����ü�� �ε����� ���´�.
	int nIdx = GetEmptyIndex();
	if( -1 == nIdx )
	{
		m_pMainDlg->OutputMsg("[����] Client Full");
		return;
	}
	//Ŭ���̾�Ʈ ���� ��û�� ���� ������ ��ٸ���.
	m_stClientInfo.m_socketClient[ nIdx ] = accept( m_stClientInfo.m_socketClient[ 0 ], 
		(SOCKADDR*) &stClientAddr , &nAddrLen );

	if( INVALID_SOCKET == m_stClientInfo.m_socketClient[ nIdx ] )
		return;
	int nRet = WSAEventSelect( m_stClientInfo.m_socketClient[ nIdx ] ,
		m_stClientInfo.m_eventHandle[ nIdx ] ,
		FD_READ | FD_CLOSE );
	if( SOCKET_ERROR == nRet )
	{
		m_pMainDlg->OutputMsg("[����] WSAEventSelect()�Լ� ���� : %d", WSAGetLastError() );
		return;
	}
	//Ŭ���̾�Ʈ ���� ����
	m_nClientCnt++;
	m_pMainDlg->OutputMsg( "Ŭ���̾�Ʈ ���� : IP(%s) SOCKET(%d)" , 
		inet_ntoa( stClientAddr.sin_addr ) , 
		m_stClientInfo.m_socketClient[ nIdx ] );
}

void cEventSelect::DoRecv( DWORD dwObjIdx )
{
	int nRecvLen = recv( m_stClientInfo.m_socketClient[ dwObjIdx ] , m_szSocketBuf , MAX_SOCKBUF , 0 );
	if( 0 == nRecvLen )
	{
		m_pMainDlg->OutputMsg("Ŭ���̾�Ʈ�� ������ ���� �Ǿ����ϴ�.");
		CloseSocket( m_stClientInfo.m_socketClient[ dwObjIdx ] );
		return;
	}
	else if( -1 == nRecvLen )
	{
		m_pMainDlg->OutputMsg("[����] recv ���� : %d " , WSAGetLastError() );
		CloseSocket( m_stClientInfo.m_socketClient[ dwObjIdx ] );
	}
	m_szSocketBuf[ nRecvLen ] = NULL;
	m_pMainDlg->OutputMsg( "socket[%d] , �޼��� ���� : %d bytes , ���� : %s" 
		, m_stClientInfo.m_socketClient[ dwObjIdx ] , nRecvLen , m_szSocketBuf );

	int nSendLen = send( m_stClientInfo.m_socketClient[ dwObjIdx ] , m_szSocketBuf , nRecvLen , 0 );
	if( -1 == nSendLen )
	{
		m_pMainDlg->OutputMsg("[����] send ���� : %d " , WSAGetLastError() );
		CloseSocket( m_stClientInfo.m_socketClient[ dwObjIdx ] );
		return;
	}
	m_pMainDlg->OutputMsg( "socket[%d] , �޼��� �۽� : %d bytes , ���� : %s" 
		, m_stClientInfo.m_socketClient[ dwObjIdx ] , nSendLen , m_szSocketBuf );
}

void cEventSelect::DestroyThread()
{
	m_bWorkerRun = false;
	closesocket( m_stClientInfo.m_socketClient[ 0 ] );
	SetEvent( m_stClientInfo.m_eventHandle[ 0 ] );
	//������ ���Ḧ ��ٸ���
	WaitForSingleObject( m_hWorkerThread, INFINITE );
}
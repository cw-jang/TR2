#include "StdAfx.h"
#include ".\coverlappedevent.h"
#include "overlappedEventDlg.h"


//WSARecv�� WSASend�� Overlapped I/O �۾� ó��
unsigned int WINAPI CallWorkerThread(LPVOID p)
{
	cOverlappedEvent* pOverlappedEvent = (cOverlappedEvent*)p;
	pOverlappedEvent->WokerThread();
	return 0;
}
unsigned int WINAPI CallAccepterThread(LPVOID p)
{
	cOverlappedEvent* pOverlappedEvent = (cOverlappedEvent*)p;
	pOverlappedEvent->AccepterThread();
	return 0;
}

cOverlappedEvent::cOverlappedEvent(void)
{
	//�������� �ʱ�ȭ�Ѵ�.
	m_pMainDlg = NULL;
	m_bWorkerRun = true;
	m_bAccepterRun = true;
	m_nClientCnt = 0;
	m_hWorkerThread = NULL;
	m_hAccepterThread = NULL;
	ZeroMemory( m_szSocketBuf , 1024 );
    
	//Ŭ���̾�Ʈ ����ü�� �ʱ�ȭ
	for( int i = 0; i < WSA_MAXIMUM_WAIT_EVENTS; i++ )
	{
		m_stClientInfo.m_socketClient[ i ] = INVALID_SOCKET;
		m_stClientInfo.m_eventHandle[ i ] = WSACreateEvent();
		ZeroMemory( &m_stClientInfo.m_stOverlappedEx[ i ] , sizeof( WSAOVERLAPPED ) );
	}
}

cOverlappedEvent::~cOverlappedEvent(void)
{
	//������ ����� ������.
	WSACleanup();

	//listen������ �ݴ´�.
	closesocket( m_stClientInfo.m_socketClient[ 0 ] );
	SetEvent( m_stClientInfo.m_eventHandle[ 0 ] );
	m_bWorkerRun = false;
	m_bAccepterRun = false;
	//������ ���Ḧ ��ٸ���
	WaitForSingleObject( m_hWorkerThread, INFINITE );
	WaitForSingleObject( m_hAccepterThread, INFINITE );
	
	
}

//������ �ʱ�ȭ�ϴ� �Լ�
bool cOverlappedEvent::InitSocket()
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
void cOverlappedEvent::CloseSocket( SOCKET socketClose , bool bIsForce )
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
bool cOverlappedEvent::BindandListen( int nBindPort )
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
		m_pMainDlg->OutputMsg("[����] bind()�Լ� ���� : %d"
			, WSAGetLastError() );
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

bool cOverlappedEvent::CreateWokerThread()
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

bool cOverlappedEvent::CreateAccepterThread()
{
	unsigned int uiThreadId = 0;

	m_hAccepterThread = (HANDLE)_beginthreadex(NULL, 0, &CallAccepterThread, this, CREATE_SUSPENDED , &uiThreadId);
	if(m_hAccepterThread == NULL)
	{
		m_pMainDlg->OutputMsg("AccepterThread Creation Failed: %u", GetLastError());	
		return false;
	}

	ResumeThread( m_hAccepterThread );
	m_pMainDlg->OutputMsg("AccepterThread ����..");
	return true;
}

//������ ���� Index��ȯ
int cOverlappedEvent::GetEmptyIndex()
{
	//0��° �迭�� �������ſ� �̺�Ʈ�̴�.
	for( int i = 1; i < WSA_MAXIMUM_WAIT_EVENTS ; i++ )
	{
		if( INVALID_SOCKET == m_stClientInfo.m_socketClient[ i ] )
			return  i;
	}
	return -1;
}

bool cOverlappedEvent::StartServer()
{
	//���ӵ� Ŭ���̾�Ʈ �ּ� ������ ������ ����ü
	bool bRet = CreateWokerThread();
	if( false == bRet )
		return false;
	bRet = CreateAccepterThread();
	if( false == bRet )
		return false;
	
	//���� ���ſ� �̺�Ʈ ����
	m_stClientInfo.m_eventHandle[ 0 ] = WSACreateEvent();
	m_pMainDlg->OutputMsg("���� ����");
	return true;
}

bool cOverlappedEvent::BindRecv( int nIdx )
{
	DWORD dwFlag = 0;
	DWORD dwRecvNumBytes = 0;

	m_stClientInfo.m_eventHandle[ nIdx ] = WSACreateEvent();

	//Overlapped I/O�� ���� �� ������ ������ �ش�.
	m_stClientInfo.m_stOverlappedEx[ nIdx ].m_wsaOverlapped.hEvent = 
		m_stClientInfo.m_eventHandle[ nIdx ];
	m_stClientInfo.m_stOverlappedEx[ nIdx ].m_wsaBuf.len = MAX_SOCKBUF;
	m_stClientInfo.m_stOverlappedEx[ nIdx ].m_wsaBuf.buf =
		m_stClientInfo.m_stOverlappedEx[ nIdx ].m_szBuf;
	m_stClientInfo.m_stOverlappedEx[ nIdx ].m_nIdx = nIdx;
	m_stClientInfo.m_stOverlappedEx[ nIdx ].m_eOperation = OP_RECV;

	int nRet = WSARecv(	m_stClientInfo.m_socketClient[ nIdx ] , 	
					&(m_stClientInfo.m_stOverlappedEx[ nIdx ].m_wsaBuf),
					1, 
					&dwRecvNumBytes, 
					&dwFlag, 
					(LPWSAOVERLAPPED)&(m_stClientInfo.m_stOverlappedEx[ nIdx ]), 
					NULL);

	//socket_error�̸� client socket�� �������ɷ� ó���Ѵ�.
	if(nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		m_pMainDlg->OutputMsg("[����] WSARecv()�Լ� ���� : %d", WSAGetLastError() );
		return false;
	}
	return true;
}
bool cOverlappedEvent::SendMsg( int nIdx , char* pMsg , int nLen )
{
	DWORD dwRecvNumBytes = 0;

	//���۵� �޼����� ����
	CopyMemory( m_stClientInfo.m_stOverlappedEx[ nIdx ].m_szBuf ,
		pMsg , nLen );

	
	//Overlapped I/O�� ���� �� ������ ������ �ش�.
	m_stClientInfo.m_stOverlappedEx[ nIdx ].m_wsaOverlapped.hEvent = 
		m_stClientInfo.m_eventHandle[ nIdx ];
	m_stClientInfo.m_stOverlappedEx[ nIdx ].m_wsaBuf.len = nLen;
	m_stClientInfo.m_stOverlappedEx[ nIdx ].m_wsaBuf.buf =
		m_stClientInfo.m_stOverlappedEx[ nIdx ].m_szBuf;
	m_stClientInfo.m_stOverlappedEx[ nIdx ].m_nIdx = nIdx;
	m_stClientInfo.m_stOverlappedEx[ nIdx ].m_eOperation = OP_SEND;

	int nRet = WSASend(	m_stClientInfo.m_socketClient[ nIdx ] , 	
					&(m_stClientInfo.m_stOverlappedEx[ nIdx ].m_wsaBuf),
					1, 
					&dwRecvNumBytes, 
					0, 
					(LPWSAOVERLAPPED)&(m_stClientInfo.m_stOverlappedEx[ nIdx ]), 
					NULL);

	//socket_error�̸� client socket�� �������ɷ� ó���Ѵ�.
	if(nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		m_pMainDlg->OutputMsg("[����] WSASend()�Լ� ���� : %d", WSAGetLastError() );
		return false;
	}
	return true;
}

//������� ������ �޴� ������
void cOverlappedEvent::AccepterThread()
{
	SOCKADDR_IN		stClientAddr;
	int nAddrLen = sizeof( SOCKADDR_IN );
	while( m_bAccepterRun )
	{
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
		
		bool bRet = BindRecv( nIdx );
		if( false == bRet )
			return;

		m_pMainDlg->OutputMsg( "Ŭ���̾�Ʈ ���� : IP(%s) SOCKET(%d)" , 
			inet_ntoa( stClientAddr.sin_addr ) , 
			m_stClientInfo.m_socketClient[ nIdx ] );

		//Ŭ���̾�Ʈ ���� ����
		m_nClientCnt++;
		//Ŭ���̾�Ʈ�� ���ӵǾ����Ƿ� WokerThread�� �������� �˸�
		WSASetEvent( m_stClientInfo.m_eventHandle[ 0 ] );

	}
}
void cOverlappedEvent::WokerThread()
{
	while( m_bWorkerRun )
	{
		//////////////////////////////////////////////////////////////////
		//��û�� Overlapped I/O�۾��� �Ϸ�Ǿ����� �̺�Ʈ�� ��ٸ���.
		//
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
		//�̺�Ʈ�� ����
		WSAResetEvent( m_stClientInfo.m_eventHandle[ dwObjIdx ] );
		//������ ���Դ�.
		if( WSA_WAIT_EVENT_0 == dwObjIdx )
			continue;

		//Overlapped I/O�� ���� ��� ó��
		OverlappedResult( dwObjIdx );
	}
}

void cOverlappedEvent::OverlappedResult( int nIdx )
{
	DWORD dwTransfer = 0;
	DWORD dwFlags = 0;

	BOOL bRet = WSAGetOverlappedResult( m_stClientInfo.m_socketClient[ nIdx ] ,
		(LPWSAOVERLAPPED)&m_stClientInfo.m_stOverlappedEx[ nIdx ] ,
		&dwTransfer ,
		FALSE,
		&dwFlags );
	if( TRUE == bRet && 0 == dwTransfer )
	{
		m_pMainDlg->OutputMsg("[����] WSAGetOverlappedResult ���� : %d" ,
				WSAGetLastError() );
        return;
	}

	//������ ����
	if( 0 == dwTransfer  )
	{
		m_pMainDlg->OutputMsg("[���� ����] socket : %d" ,m_stClientInfo.m_socketClient[ nIdx ] );
		CloseSocket( m_stClientInfo.m_socketClient[ nIdx ] );
		m_nClientCnt--;
        return;
	}
	
	stOverlappedEx* pOverlappedEx = &m_stClientInfo.m_stOverlappedEx[ nIdx ];
	switch ( pOverlappedEx->m_eOperation )
	{
		//WSARecv�� Overlapped I/O�� �Ϸ�Ǿ���.
	case OP_RECV:
		{
			pOverlappedEx->m_szBuf[ dwTransfer ] = NULL;
			m_pMainDlg->OutputMsg("[����] bytes : %d , msg : %s", dwTransfer , 
				pOverlappedEx->m_szBuf );
			
			//Ŭ���̾�Ʈ�� �޼����� �����Ѵ�.
			SendMsg( nIdx , pOverlappedEx->m_szBuf , dwTransfer );
		}
		break;
		//WSASend�� Overlapped I/O�� �Ϸ�Ǿ���.
	case OP_SEND:
		{
			pOverlappedEx->m_szBuf[ dwTransfer ] = NULL;
			m_pMainDlg->OutputMsg("[�۽�] bytes : %d , msg : %s", dwTransfer , 
				pOverlappedEx->m_szBuf );
			//�ٽ� Recv Overlapped I/O�� �ɾ��ش�.
			BindRecv( nIdx );
		}
		break;
	default:
		{
			m_pMainDlg->OutputMsg("���ǵ��� ���� Operation");
		}
		break;
	}
}
void cOverlappedEvent::DestroyThread()
{
	closesocket( m_stClientInfo.m_socketClient[ 0 ] );
	SetEvent( m_stClientInfo.m_eventHandle[ 0 ] );
	m_bWorkerRun = false;
	m_bAccepterRun = false;
	//������ ���Ḧ ��ٸ���
	WaitForSingleObject( m_hWorkerThread, INFINITE );
	WaitForSingleObject( m_hAccepterThread, INFINITE );
}
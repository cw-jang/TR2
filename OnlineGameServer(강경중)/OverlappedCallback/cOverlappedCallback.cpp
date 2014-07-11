#include "StdAfx.h"
#include ".\coverlappedcallback.h"
#include "OverlappedCallbackDlg.h"


//������ �ޱ����� ����ϴ� ������
unsigned int WINAPI CallAccepterThread(LPVOID p)
{
	cOverlappedCallback* pOverlappedCallback = (cOverlappedCallback*)p;
	pOverlappedCallback->AccepterThread();
	return 0;
}

//Overlapped I/O�۾� �Ϸ� �뺸�� �޴� �ݹ� �Լ�
void CALLBACK CompletionRoutine( DWORD dwError ,
								DWORD dwTransferred,
								LPWSAOVERLAPPED Overlapped,
								DWORD dwFlags );

cOverlappedCallback::cOverlappedCallback(void)
{
	m_pMainDlg = NULL;
	m_bAccepterRun = true;
	m_nClientCnt = 0;
	m_hAccepterThread = NULL;
	m_sockListen = INVALID_SOCKET;
	ZeroMemory( m_szSocketBuf , 1024 );
}

cOverlappedCallback::~cOverlappedCallback(void)
{
	//������ ����� ������.
	WSACleanup();

	//listen������ �ݴ´�.
	closesocket( m_sockListen );
	m_bAccepterRun = false;
	//������ ���Ḧ ��ٸ���
	WaitForSingleObject( m_hAccepterThread, INFINITE );
}

//������ �ʱ�ȭ�ϴ� �Լ�
bool cOverlappedCallback::InitSocket()
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
	m_sockListen = WSASocket( AF_INET , SOCK_STREAM 
		, IPPROTO_TCP , NULL , NULL , WSA_FLAG_OVERLAPPED );

	if( INVALID_SOCKET == m_sockListen )
	{
		m_pMainDlg->OutputMsg("[����] socket()�Լ� ���� : %d", WSAGetLastError() );
		return false;
	}
	
	//������ �ڵ� ����
	m_pMainDlg->OutputMsg("���� �ʱ�ȭ ����");
	return true;
}

//�ش� ������ ���� ��Ų��.
void cOverlappedCallback::CloseSocket( SOCKET socketClose , bool bIsForce )
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
bool cOverlappedCallback::BindandListen( int nBindPort )
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
	int nRet = bind( m_sockListen , (SOCKADDR*) &stServerAddr , sizeof( SOCKADDR_IN ) );
	if( 0 != nRet )
	{
		m_pMainDlg->OutputMsg("[����] bind()�Լ� ���� : %d"
			, WSAGetLastError() );
		return false;
	}
	
	//���� ��û�� �޾Ƶ��̱� ���� m_socketListen������ ����ϰ� ���Ӵ��ť�� 5���� ���� �Ѵ�.
	nRet = listen( m_sockListen , 5 );
	if( 0 != nRet )
	{
		m_pMainDlg->OutputMsg("[����] listen()�Լ� ���� : %d"
			, WSAGetLastError() );
		return false;
	}
 
	m_pMainDlg->OutputMsg("���� ��� ����..");

	return true;
}


bool cOverlappedCallback::CreateAccepterThread()
{
	unsigned int uiThreadId = 0;

	m_hAccepterThread = (HANDLE)_beginthreadex(NULL, 0, &CallAccepterThread, this, CREATE_SUSPENDED ,
		&uiThreadId);
	if(m_hAccepterThread == NULL)
	{
		m_pMainDlg->OutputMsg("AccepterThread Creation Failed: %u", GetLastError());	
		return false;
	}

	ResumeThread( m_hAccepterThread );
	m_pMainDlg->OutputMsg("AccepterThread ����..");
	return true;
}


bool cOverlappedCallback::StartServer()
{
	//���ӵ� Ŭ���̾�Ʈ �ּ� ������ ������ ����ü
	
	bool bRet = CreateAccepterThread();
	if( false == bRet )
		return false;

	m_pMainDlg->OutputMsg("���� ����");
	return true;
}

bool cOverlappedCallback::BindRecv( SOCKET socket )
{
	DWORD dwFlag = 0;
	DWORD dwRecvNumBytes = 0;


	//Overlapped I/O�� ���� ����ü ����
	stOverlappedEx* pOverlappedEx = new stOverlappedEx;
	//����ü �ʱ�ȭ
	ZeroMemory( pOverlappedEx , sizeof( WSAOVERLAPPED ) );

	//Overlapped I/O�� ���� �� ������ ������ �ش�.
	pOverlappedEx->m_wsaBuf.len = MAX_SOCKBUF;
	pOverlappedEx->m_wsaBuf.buf = &pOverlappedEx->m_szBuf[ 0 ];
	pOverlappedEx->m_socketClient = socket;
	pOverlappedEx->m_eOperation = OP_RECV;
	pOverlappedEx->m_pOverlappedCallback = this;

	int nRet = WSARecv(	socket , 	
					&(pOverlappedEx->m_wsaBuf),
					1, 
					&dwRecvNumBytes, 
					&dwFlag, 
					(LPWSAOVERLAPPED)(pOverlappedEx), 
					CompletionRoutine);

	//socket_error�̸� client socket�� �������ɷ� ó���Ѵ�.
	if(nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		m_pMainDlg->OutputMsg("[����] WSARecv()�Լ� ���� : %d", WSAGetLastError() );
		return false;
	}
	return true;
}

bool cOverlappedCallback::SendMsg( SOCKET socket , char* pMsg , int nLen )
{
	DWORD dwRecvNumBytes = 0;

	//Overlapped I/O�� ���� ����ü ����
	stOverlappedEx* pOverlappedEx = new stOverlappedEx;
	//����ü �ʱ�ȭ
	ZeroMemory( pOverlappedEx , sizeof( WSAOVERLAPPED ) );
	//���۵� �޼����� ����
	CopyMemory( pOverlappedEx->m_szBuf , pMsg , nLen );

	//Overlapped I/O�� ���� �� ������ ������ �ش�.
	pOverlappedEx->m_wsaBuf.buf = pOverlappedEx->m_szBuf;
	pOverlappedEx->m_wsaBuf.len = nLen;
	pOverlappedEx->m_socketClient = socket;
	pOverlappedEx->m_eOperation = OP_SEND;
	pOverlappedEx->m_pOverlappedCallback = this;
	
	int nRet = WSASend(	socket , 	
					&(pOverlappedEx->m_wsaBuf),
					1, 
					&dwRecvNumBytes, 
					0, 
					(LPWSAOVERLAPPED)(pOverlappedEx), 
					CompletionRoutine);

	//socket_error�̸� client socket�� �������ɷ� ó���Ѵ�.
	if(nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		m_pMainDlg->OutputMsg("[����] WSASend()�Լ� ���� : %d", WSAGetLastError() );
		return false;
	}
	return true;
}

//������� ������ �޴� ������
void cOverlappedCallback::AccepterThread()
{
	SOCKADDR_IN		stClientAddr;
	int nAddrLen = sizeof( SOCKADDR_IN );
	while( m_bAccepterRun )
	{
		//Ŭ���̾�Ʈ ���� ��û�� ���� ������ ��ٸ���.
		SOCKET sockAccept = accept( m_sockListen ,
			(SOCKADDR*) &stClientAddr , &nAddrLen );

		if( INVALID_SOCKET == sockAccept )
		{
			continue;
		}
		bool bRet = BindRecv( sockAccept );
		if( false == bRet )
			return;

		m_pMainDlg->OutputMsg( "Ŭ���̾�Ʈ ���� : IP(%s) SOCKET(%d)" 
			, inet_ntoa( stClientAddr.sin_addr ) , sockAccept );

	}
}

void CALLBACK CompletionRoutine( DWORD dwError ,
					  			 DWORD dwTransferred,
                                 LPWSAOVERLAPPED lpOverlapped,
                                 DWORD dwFlags )
{

	stOverlappedEx* pOverlappedEx = (stOverlappedEx*)lpOverlapped;

	//class�� �����͸� �޴´�.
	cOverlappedCallback* pOverlappedCallback = 
		(cOverlappedCallback*)pOverlappedEx->m_pOverlappedCallback;

	COverlappedCallbackDlg* pMainDlg = pOverlappedCallback->GetMainDlg();

	//������ ����
	if( 0 == dwTransferred )
	{

		pMainDlg->OutputMsg("[���� ����] socket : %d" ,	pOverlappedEx->m_socketClient );
		pOverlappedCallback->CloseSocket( pOverlappedEx->m_socketClient );
        goto end;
	}

	if( 0 != dwError )
	{
		pMainDlg->OutputMsg("[����] CompletionRoutine ���� : %d" , WSAGetLastError() );
        goto end;
	}

		
	switch ( pOverlappedEx->m_eOperation )
	{
		//WSARecv�� Overlapped I/O�� �Ϸ�Ǿ���.
	case OP_RECV:
		{
			pOverlappedEx->m_szBuf[ dwTransferred ] = NULL;
			pMainDlg->OutputMsg("[����] bytes : %d , msg : %s", dwTransferred , pOverlappedEx->m_szBuf );
			
			//Ŭ���̾�Ʈ�� �޼����� �����Ѵ�.
			pOverlappedCallback->SendMsg( pOverlappedEx->m_socketClient , 
				pOverlappedEx->m_szBuf , dwTransferred );

			
		}
		break;
		//WSASend�� Overlapped I/O�� �Ϸ�Ǿ���.
	case OP_SEND:
		{
			pOverlappedEx->m_szBuf[ dwTransferred ] = NULL;
			pMainDlg->OutputMsg("[�۽�] bytes : %d , msg : %s", dwTransferred , pOverlappedEx->m_szBuf );

			//�ٽ� Recv Overlapped I/O�� �ɾ��ش�.
			pOverlappedCallback->BindRecv( pOverlappedEx->m_socketClient );
		}
		break;
	default:
		{
			pMainDlg->OutputMsg("���ǵ��� ���� Operation");
		}
		break;
	}
end:
	delete pOverlappedEx;
 }

void cOverlappedCallback::DestroyThread()
{
	closesocket( m_sockListen );
	m_bAccepterRun = false;
	//������ ���Ḧ ��ٸ���
	WaitForSingleObject( m_hAccepterThread, INFINITE );
}
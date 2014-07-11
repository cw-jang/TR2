#include "stdafx.h"
#include <iostream>

using namespace std;
#include "cAsyncSocket.h"
#include "AsyncSelectDlg.h"

cAsyncSocket::cAsyncSocket(void)
{
	m_socketListen = INVALID_SOCKET;
	m_hWnd = NULL;
	m_pMainDlg = NULL;
	ZeroMemory( m_szSocketBuf , 1024 );
}

cAsyncSocket::~cAsyncSocket(void)
{
	//������ ����� ������.
	WSACleanup();
}

//������ �ʱ�ȭ�ϴ� �Լ�
bool cAsyncSocket::InitSocket( HWND hWnd )
{
	WSADATA wsaData;
	//������ ���� 2,2�� �ʱ�ȭ �Ѵ�.
	int nRet = WSAStartup( MAKEWORD(2,2) , &wsaData );
	if( 0 != nRet )
	{
		m_pMainDlg->OutputMsg("[����] WSAStartup()�Լ� ���� : %d"
			, WSAGetLastError() );
		return false;
	}

	//���������� TCP ������ ����
	m_socketListen = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP );
	if( INVALID_SOCKET == m_socketListen )
	{
		m_pMainDlg->OutputMsg("[����] socket()�Լ� ���� : %d"
			, WSAGetLastError() );
		return false;
	}
	
	//������ �ڵ� ����
	m_hWnd = hWnd;
	m_pMainDlg->OutputMsg("���� �ʱ�ȭ ����");
	return true;
}

//�ش� ������ ���� ��Ų��.
void cAsyncSocket::CloseSocket( SOCKET socketClose , bool bIsForce )
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
bool cAsyncSocket::BindandListen( int nBindPort )
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
	int nRet = bind( m_socketListen , (SOCKADDR*) &stServerAddr , sizeof( SOCKADDR_IN ) );
	if( 0 != nRet )
	{
		m_pMainDlg->OutputMsg("[����] bind()�Լ� ���� : %d"
			, WSAGetLastError() );
		return false;
	}
	
	//���� ��û�� �޾Ƶ��̱� ���� m_socketListen������ ����ϰ� ���Ӵ��ť�� 5���� ���� �Ѵ�.
	nRet = listen( m_socketListen , 5 );
	if( 0 != nRet )
	{
		m_pMainDlg->OutputMsg("[����] listen()�Լ� ���� : %d"
			, WSAGetLastError() );
		return false;
	}
	m_pMainDlg->OutputMsg("���� ��� ����..");
	return true;
}

bool cAsyncSocket::StartServer()
{
	
	//���ϰ� ��Ʈ��ũ �̺�Ʈ�� ����Ѵ�.
	int nRet = WSAAsyncSelect( m_socketListen , m_hWnd , WM_SOCKETMSG , FD_ACCEPT | FD_CLOSE );
	if( SOCKET_ERROR == nRet )
	{
		m_pMainDlg->OutputMsg("[����] WSAAsyncSelect()�Լ� ���� : %d"
			, WSAGetLastError() );
		return false;
	}
	return true;
}

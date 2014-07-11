
//////////////////////////////////////////////////////////////////////////////////////////
/*					����		  : client.cpp											*/
/*					������Ʈ ���� : Win32 Console										*/
/*					����		  : ���� 												*/
/*					�ۼ�����	  : 2004.09.02											*/
/*					�ۼ���		  : ����												*/
///////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <iostream>

using namespace std;
#include "cAsyncSocket.h"
#include "ChatClientDlg.h"
cAsyncSocket::cAsyncSocket(void)
{
	m_socket = INVALID_SOCKET;
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
	if( INVALID_SOCKET != m_socket )
		CloseSocket( m_socket );

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
	m_socket = socket( AF_INET , SOCK_STREAM , IPPROTO_TCP );
	if( INVALID_SOCKET == m_socket )
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


bool cAsyncSocket::ConnectTo( char* szIP , int nPort )
{
	//������ ���� ������ ������ ����ü
	SOCKADDR_IN		stServerAddr;

	stServerAddr.sin_family = AF_INET;
    //������ ���� ��Ʈ�� �ִ´�.
	stServerAddr.sin_port = htons( nPort );
	//������ ���� ip�� �ִ´�.
	stServerAddr.sin_addr.s_addr = inet_addr( szIP );

	int nRet = connect( m_socket , (sockaddr*) &stServerAddr , sizeof( sockaddr ) );
	if( SOCKET_ERROR == nRet )
	{
		m_pMainDlg->OutputMsg("[ERROR] ���� ���� : connect() failed");
		return false;
	}
	WSAAsyncSelect( m_socket , m_hWnd , WM_SOCKETMSG , FD_READ | FD_CLOSE );
	m_pMainDlg->OutputMsg("[SUCCESS] ���� ����");

	return true;
}

int cAsyncSocket::SendMsg( char* pMsg , int nLen )
{
	return send( m_socket , pMsg , nLen , 0 );
}

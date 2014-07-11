
//////////////////////////////////////////////////////////////////////////////////////////
/*					����		  : cAsyncSocket.h											*/
/*					������Ʈ ���� : Win32 Console										*/
/*					����		  : ���� Ŭ����     									*/
/*					�ۼ�����	  : 2004.09.02											*/
/*					�ۼ���		  : ����												*/
///////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <winsock2.h>
#define WM_SOCKETMSG WM_USER+1
class CChatClientDlg;
class cAsyncSocket
{
public:
	cAsyncSocket(void);
	~cAsyncSocket(void);
	

	//------���� Ŭ���̾�Ʈ �����Լ�-------//
	//������ �ʱ�ȭ�ϴ� �Լ�
	bool InitSocket( HWND hWnd );
	//������ ������ ���� ��Ų��.
	void CloseSocket( SOCKET socketClose , bool bIsForce = false );
	
	//------Ŭ���̾�Ʈ�� �Լ�------//
	bool ConnectTo( char* szIP , int nPort );

	//------���� �Լ�------//
	int SendMsg( char* pMsg , int nLen );

	
	void SetMainDlg( CChatClientDlg* pMainDlg ) { m_pMainDlg = pMainDlg; }
	
private:
	
	//Ŭ���̾�Ʈ�� ���ӿ�û�� ��ٸ��� ����
	SOCKET		m_socket;


	//��Ʈ��ũ �̺�Ʈ�� �߻��Ͽ����� �޼����� ���� ������ �ڵ�
	HWND		m_hWnd;
	//���� ������ ������
	CChatClientDlg* m_pMainDlg;

	//���� ����
	char		m_szSocketBuf[ 1024 ];

	
};

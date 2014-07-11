
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
class CAsyncSelectDlg;
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
	
	//------������ �Լ�-------//
	//������ �ּ������� ���ϰ� �����Ű�� ���� ��û�� �ޱ� ���� �� ������ ����ϴ� �Լ�
	bool BindandListen( int nBindPort );
	//���� ��û�� �����ϰ� �޼����� �޾Ƽ� ó���ϴ� �Լ�
	bool StartServer();

	
	void SetMainDlg( CAsyncSelectDlg* pMainDlg ) { m_pMainDlg = pMainDlg; }
	
private:
	
	//Ŭ���̾�Ʈ�� ���ӿ�û�� ��ٸ��� ����
	SOCKET		m_socketListen;

	//��Ʈ��ũ �̺�Ʈ�� �߻��Ͽ����� �޼����� ���� ������ �ڵ�
	HWND		m_hWnd;
	//���� ������ ������
	CAsyncSelectDlg* m_pMainDlg;

	//���� ����
	char		m_szSocketBuf[ 1024 ];

	
};

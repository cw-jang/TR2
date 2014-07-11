
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
class CGameClientDlg;
class cAsyncSocket : public cSingleton
{
	DECLEAR_SINGLETON( cAsyncSocket );
public:
	cAsyncSocket(void);
	~cAsyncSocket(void);
	

	//------���� Ŭ���̾�Ʈ �����Լ�-------//
	//������ �ʱ�ȭ�ϴ� �Լ�
	bool InitSocket( HWND hWnd );
	//������ ������ ���� ��Ų��.
	void CloseSocket( bool bIsForce = false );
	SOCKET GetSocket() { return m_socket; };
	
	//------Ŭ���̾�Ʈ�� �Լ�------//
	bool ConnectTo( char* szIP , int nPort );

	//------���� �Լ�------//
	int SendMsg( char* pMsg , int nLen );

	
	void SetMainDlg( CGameClientDlg* pMainDlg ) { m_pMainDlg = pMainDlg; }

	
private:
	
	//Ŭ���̾�Ʈ�� ���ӿ�û�� ��ٸ��� ����
	SOCKET		m_socket;
	//��Ʈ��ũ �̺�Ʈ�� �߻��Ͽ����� �޼����� ���� ������ �ڵ�
	HWND		m_hWnd;
	//���� ������ ������
	CGameClientDlg* m_pMainDlg;
	//���� ����
	char		m_szSocketBuf[ 1024 ];
};
CREATE_FUNCTION( cAsyncSocket , AsyncSocket );

// AsyncSelectDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "AsyncSelect.h"
#include "AsyncSelectDlg.h"
#include ".\asyncselectdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� ������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ����

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAsyncSelectDlg ��ȭ ����



CAsyncSelectDlg::CAsyncSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsyncSelectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAsyncSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OUTPUT, m_ctOutput);
	
}

BEGIN_MESSAGE_MAP(CAsyncSelectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE( WM_SOCKETMSG , OnSocketMsg )
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_STARTSERVER, OnBnClickedStartserver)
END_MESSAGE_MAP()

// CAsyncSelectDlg �޽��� ó����
BOOL CAsyncSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

void CAsyncSelectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CAsyncSelectDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�. 
HCURSOR CAsyncSelectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CAsyncSelectDlg::OnSocketMsg( WPARAM wParam , LPARAM lParam )
{

	SOCKET sock = (SOCKET)wParam;

	//����üũ
	int nError = WSAGETSELECTERROR( lParam );
	if( 0 != nError )
	{
		OutputMsg( "[����] WSAGETSELECTERROR : %d ", nError );
		m_AsyncSocket.CloseSocket( sock );
		return false;
	}
	
	//�̺�Ʈ üũ
	int nEvent = WSAGETSELECTEVENT( lParam );
	switch( nEvent )
	{
	case FD_READ:
		{
			int nRecvLen = recv( sock , m_szSocketBuf , MAX_SOCKBUF , 0 );
			if( 0 == nRecvLen )
			{
				OutputMsg("Ŭ���̾�Ʈ�� ������ ���� �Ǿ����ϴ�.");
				m_AsyncSocket.CloseSocket( sock );
				return false;
			}
			else if( -1 == nRecvLen )
			{
				OutputMsg("[����] recv ���� : %d " , WSAGetLastError() );
				m_AsyncSocket.CloseSocket( sock );
			}
			m_szSocketBuf[ nRecvLen ] = NULL;
			OutputMsg( "socket[%d] , �޼��� ���� : %d bytes , ���� : %s" 
				, sock , nRecvLen , m_szSocketBuf );
			
			int nSendLen = send( sock , m_szSocketBuf , nRecvLen , 0 );
			if( -1 == nSendLen )
			{
				OutputMsg("[����] send ���� : %d " , WSAGetLastError() );
				m_AsyncSocket.CloseSocket( sock );
				return false;
			}
			OutputMsg( "socket[%d] , �޼��� �۽� : %d bytes , ���� : %s" 
				, sock , nSendLen , m_szSocketBuf );
			break;
		}
	case FD_ACCEPT:
		{
			//���ӵ� Ŭ���̾�Ʈ �ּ� ������ ������ ����ü
			SOCKADDR_IN		stClientAddr;
			int nAddrLen = sizeof( SOCKADDR_IN );

			SOCKET sockClient = accept( sock , (SOCKADDR*)&stClientAddr , &nAddrLen );
			if( INVALID_SOCKET == sockClient )
			{
				OutputMsg( "[����] accept()�Լ� ���� : %d "
					, WSAGetLastError() );
				return false;
			}
			//���ϰ� ��Ʈ��ũ �̺�Ʈ�� ����Ѵ�.
			int nRet = WSAAsyncSelect( sockClient , m_hWnd , WM_SOCKETMSG , FD_READ | FD_CLOSE );
			if( SOCKET_ERROR == nRet )
			{
				OutputMsg("[����] WSAAsyncSelect()�Լ� ���� : %d"
					, WSAGetLastError() );
				return false;
			}
			OutputMsg("Ŭ���̾�Ʈ ���� : IP(%s) SOCKET(%d)" 
				, inet_ntoa( stClientAddr.sin_addr ) , sockClient );

		}
		break;
	case FD_CLOSE:
		{
			OutputMsg("Ŭ���̾�Ʈ ���� ���� : SOCKET(%d)", sock );
			m_AsyncSocket.CloseSocket( sock );
		}
		break;
	}
	return true;

}
//��� �޼���
void CAsyncSelectDlg::OutputMsg( char *szOutputString , ... )
{
	char szOutStr[ 1024 ];
	va_list	argptr; 
    va_start( argptr, szOutputString );
	vsprintf( szOutStr , szOutputString, argptr );
	va_end( argptr );
	m_ctOutput.SetCurSel( m_ctOutput.AddString( szOutStr ) );
}

void CAsyncSelectDlg::OnBnClickedStartserver()
{
	//���� ���̾�α� �����͸� �Ѱ��ش�.
	//���� �޼����� �����쿡 �Ѹ��� ����
	m_AsyncSocket.SetMainDlg( this );
	//������ �ʱ�ȭ
	m_AsyncSocket.InitSocket( m_hWnd );
	//���ϰ� ���� �ּҸ� �����ϰ� ��� ��Ų��.
	m_AsyncSocket.BindandListen( 8000 );
	//��Ʈ��ũ �̺�Ʈ�� �����츦 ��� ��Ű�� ���� ����
	m_AsyncSocket.StartServer();
}

// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "GameServer.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_SERVER_START, OnServerStart)
	ON_COMMAND(ID_SERVER_STOP, OnServerStop)
	ON_COMMAND(ID_CONNECT_NPCSERVER, OnConnectNpcServer)
	ON_COMMAND(ID_SERVER_CREATETEMPPLAYER, OnCreateTempPlayer)
	ON_COMMAND(ID_SERVER_DESTROYTEMPPLAYER, OnDestroyTempPlayer)
	
	ON_MESSAGE( WM_DEBUGMSG , OnDebugMsg )
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
	IocpGameServer();
	LogView();
}

CMainFrame::~CMainFrame()
{
	cSingleton::releaseAll();
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	// Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	return TRUE;
	
}


// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame �޽��� ó����


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CVisualObject	*m_pTab1 = new CVisualObject( 1,"",pContext,RUNTIME_CLASS(CTabWnd),CVisualObject::TOS_TABBOTTOM );
	// each views
	CVisualObject	*m_pUserLogView = new CVisualObject(2,"LOG",pContext,RUNTIME_CLASS(CMsgViewFromLog));
	// add to frame
	m_frameWork.Add( m_pTab1 );
	m_frameWork.Add( m_pTab1,m_pUserLogView );
	
	delete m_pTab1->GetWnd();
	delete m_pUserLogView->GetWnd();
	return m_frameWork.Create( this );
}

void CMainFrame::OnServerStart()
{
	IocpGameServer()->ServerStart();
}

void CMainFrame::OnServerStop()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	
}

void CMainFrame::OnConnectNpcServer()
{
	IocpGameServer()->ConnectToNpcServer();
}

void CMainFrame::OnCreateTempPlayer()
{
	PlayerManager()->CreateTempPlayer( MAX_TEMPPLAYER );
}

void CMainFrame::OnDestroyTempPlayer()
{
	PlayerManager()->DestroyTempPlayer();
}

LRESULT CMainFrame::OnDebugMsg( WPARAM wParam , LPARAM lParam )
{
	char* pMsg = (char*)wParam;
	pMsg[ strlen( pMsg ) - 2 ] = NULL;		//�ǵڿ� \n ���� ���Ÿ� ����
	LogView()->OutputDebugMsg( (enumLogInfoType)lParam , (char*)wParam );
	return true;
}

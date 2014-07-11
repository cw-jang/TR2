// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "NpcServer.h"

#include "MainFrm.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_COMMAND(ID_SERVER_START, OnServerStart)
	ON_COMMAND(ID_SERVER_STOP, OnServerStop)
	ON_COMMAND(ID_SERVER_CREATENPC, OnCreateNpc)
	ON_COMMAND(ID_SERVER_DESTROYNPC, OnDestroyNpc)
	ON_MESSAGE( WM_DEBUGMSG , OnDebugMsg )
	ON_WM_CREATE()
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
}

CMainFrame::~CMainFrame()
{
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
	IocpNpcServer()->ServerStart();
}

void CMainFrame::OnServerStop()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	
}

LRESULT CMainFrame::OnDebugMsg( WPARAM wParam , LPARAM lParam )
{
	char* pMsg = (char*)wParam;
	pMsg[ strlen( pMsg ) - 2 ] = NULL;		//�ǵڿ� \n ���� ���Ÿ� ����
	LogView()->OutputDebugMsg( (enumLogInfoType)lParam , (char*)wParam );
	return true;
}

void CMainFrame::OnCreateNpc()
{
	int nDetectNpcCnt = 0;
	int nNormalNpcCnt = 0;
	if( -1 == ( nDetectNpcCnt = 
		IocpNpcServer()->GetINIInt( BASE_SERVER  , "DETECTNPC_CNT"  , INIFILE_NAME ) ) )
		return;
	if( -1 == ( nNormalNpcCnt = 
		IocpNpcServer()->GetINIInt( BASE_SERVER  , "NORMALNPC_CNT"  , INIFILE_NAME ) ) )
		return;

	NpcManager()->CreateNpc( DETECT_NPC , nDetectNpcCnt );
	NpcManager()->CreateNpc( NORMAL_NPC , nNormalNpcCnt );

	LOG( LOG_INFO_NORMAL ,
		"SYSTEM | CMainFrame::OnCreateNpc() | Detect NPC (%d)���� ����",
		nDetectNpcCnt );
	LOG( LOG_INFO_NORMAL ,
		"SYSTEM | CMainFrame::OnCreateNpc() | Normal NPC (%d)���� ����",
		nNormalNpcCnt );
}

void CMainFrame::OnDestroyNpc()
{
	NpcManager()->DestroyNpc();
}

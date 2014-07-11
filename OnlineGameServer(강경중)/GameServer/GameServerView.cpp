// GameServerView.cpp : CGameServerView Ŭ������ ����
//

#include "stdafx.h"
#include "GameServer.h"

#include "GameServerDoc.h"
#include "GameServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameServerView

IMPLEMENT_DYNCREATE(CGameServerView, CView)

BEGIN_MESSAGE_MAP(CGameServerView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CGameServerView ����/�Ҹ�

CGameServerView::CGameServerView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CGameServerView::~CGameServerView()
{
}

BOOL CGameServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	// Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CGameServerView �׸���

void CGameServerView::OnDraw(CDC* /*pDC*/)
{
	CGameServerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CGameServerView �μ�

BOOL CGameServerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CGameServerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CGameServerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CGameServerView ����

#ifdef _DEBUG
void CGameServerView::AssertValid() const
{
	CView::AssertValid();
}

void CGameServerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGameServerDoc* CGameServerView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameServerDoc)));
	return (CGameServerDoc*)m_pDocument;
}
#endif //_DEBUG


// CGameServerView �޽��� ó����

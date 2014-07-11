// NpcServerView.cpp : CNpcServerView Ŭ������ ����
//

#include "stdafx.h"
#include "NpcServer.h"

#include "NpcServerDoc.h"
#include "NpcServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNpcServerView

IMPLEMENT_DYNCREATE(CNpcServerView, CView)

BEGIN_MESSAGE_MAP(CNpcServerView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CNpcServerView ����/�Ҹ�

CNpcServerView::CNpcServerView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CNpcServerView::~CNpcServerView()
{
}

BOOL CNpcServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	// Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CNpcServerView �׸���

void CNpcServerView::OnDraw(CDC* /*pDC*/)
{
	CNpcServerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CNpcServerView �μ�

BOOL CNpcServerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CNpcServerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CNpcServerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CNpcServerView ����

#ifdef _DEBUG
void CNpcServerView::AssertValid() const
{
	CView::AssertValid();
}

void CNpcServerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNpcServerDoc* CNpcServerView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNpcServerDoc)));
	return (CNpcServerDoc*)m_pDocument;
}
#endif //_DEBUG


// CNpcServerView �޽��� ó����

// NpcServerDoc.cpp : CNpcServerDoc Ŭ������ ����
//

#include "stdafx.h"
#include "NpcServer.h"

#include "NpcServerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNpcServerDoc

IMPLEMENT_DYNCREATE(CNpcServerDoc, CDocument)

BEGIN_MESSAGE_MAP(CNpcServerDoc, CDocument)
END_MESSAGE_MAP()


// CNpcServerDoc ����/�Ҹ�

CNpcServerDoc::CNpcServerDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CNpcServerDoc::~CNpcServerDoc()
{
}

BOOL CNpcServerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ �ٽ� �ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CNpcServerDoc serialization

void CNpcServerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CNpcServerDoc ����

#ifdef _DEBUG
void CNpcServerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNpcServerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNpcServerDoc ���

// GameServerDoc.cpp : CGameServerDoc Ŭ������ ����
//

#include "stdafx.h"
#include "GameServer.h"

#include "GameServerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameServerDoc

IMPLEMENT_DYNCREATE(CGameServerDoc, CDocument)

BEGIN_MESSAGE_MAP(CGameServerDoc, CDocument)
END_MESSAGE_MAP()


// CGameServerDoc ����/�Ҹ�

CGameServerDoc::CGameServerDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CGameServerDoc::~CGameServerDoc()
{
}

BOOL CGameServerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ �ٽ� �ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CGameServerDoc serialization

void CGameServerDoc::Serialize(CArchive& ar)
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


// CGameServerDoc ����

#ifdef _DEBUG
void CGameServerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGameServerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGameServerDoc ���

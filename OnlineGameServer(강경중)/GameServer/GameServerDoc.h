// GameServerDoc.h : CGameServerDoc Ŭ������ �������̽�
//


#pragma once

class CGameServerDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CGameServerDoc();
	DECLARE_DYNCREATE(CGameServerDoc)

// Ư��
public:

// �۾�
public:

// ������
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ����
public:
	virtual ~CGameServerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};



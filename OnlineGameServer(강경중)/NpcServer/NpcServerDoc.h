// NpcServerDoc.h : CNpcServerDoc Ŭ������ �������̽�
//


#pragma once

class CNpcServerDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CNpcServerDoc();
	DECLARE_DYNCREATE(CNpcServerDoc)

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
	virtual ~CNpcServerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};



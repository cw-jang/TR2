// GameServerView.h : iCGameServerView Ŭ������ �������̽�
//


#pragma once


class CGameServerView : public CView
{
protected: // serialization������ ��������ϴ�.
	CGameServerView();
	DECLARE_DYNCREATE(CGameServerView)

// Ư��
public:
	CGameServerDoc* GetDocument() const;

// �۾�
public:

// ������
	public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ����
public:
	virtual ~CGameServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // GameServerView.cpp�� ����� ����
inline CGameServerDoc* CGameServerView::GetDocument() const
   { return reinterpret_cast<CGameServerDoc*>(m_pDocument); }
#endif


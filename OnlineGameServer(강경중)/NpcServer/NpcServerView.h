// NpcServerView.h : iCNpcServerView Ŭ������ �������̽�
//


#pragma once


class CNpcServerView : public CView
{
protected: // serialization������ ��������ϴ�.
	CNpcServerView();
	DECLARE_DYNCREATE(CNpcServerView)

// Ư��
public:
	CNpcServerDoc* GetDocument() const;

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
	virtual ~CNpcServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // NpcServerView.cpp�� ����� ����
inline CNpcServerDoc* CNpcServerView::GetDocument() const
   { return reinterpret_cast<CNpcServerDoc*>(m_pDocument); }
#endif


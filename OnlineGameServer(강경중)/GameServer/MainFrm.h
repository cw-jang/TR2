// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#include "Tab.h"
#pragma once
class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư��
public:

// �۾�
public:
	CVisualFramework	m_frameWork;
	CVisualObject	*m_pTab1;
	CVisualObject	*m_pUserLogView; 

	LRESULT OnDebugMsg( WPARAM wParam , LPARAM lParam );
// ������
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnServerStart();
	afx_msg void OnServerStop();
	afx_msg void OnCreateTempPlayer();
	afx_msg void OnDestroyTempPlayer();
	afx_msg void OnConnectNpcServer();

};


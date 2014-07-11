// EventSelectDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "cEventSelect.h"
#include "resource.h"
// CEventSelectDlg ��ȭ ����
class CEventSelectDlg : public CDialog
{
// ����
public:
	CEventSelectDlg(CWnd* pParent = NULL);	// ǥ�� ������

// ��ȭ ���� ������
	enum { IDD = IDD_EVENTSELECT_DIALOG };

	void OutputMsg( char *szOutputString , ... );

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ����
	

// ����
protected:
	HICON m_hIcon;

	// �޽��� �� �Լ��� �����߽��ϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ctOutput;
	cEventSelect m_EventSelect;
	afx_msg void OnBnClickedStartserver();
	afx_msg void OnBnClickedCancel();
};

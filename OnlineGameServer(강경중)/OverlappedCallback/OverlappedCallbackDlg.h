// OverlappedCallbackDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "resource.h"
#include "cOverlappedCallback.h"

// COverlappedCallbackDlg ��ȭ ����
class COverlappedCallbackDlg : public CDialog
{
// ����
public:
	COverlappedCallbackDlg(CWnd* pParent = NULL);	// ǥ�� ������

// ��ȭ ���� ������
	enum { IDD = IDD_OVERLAPPEDCALLBACK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ����


public:
		//��� �޼���
	void OutputMsg( char *szOutputString , ... );

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
	//overlapped i/o class
	cOverlappedCallback	m_OverlappedCallback;
	//�޼����� ����ϱ� ���� ���� ����Ʈ �ڽ� ��Ʈ��
	CListBox m_ctOutput;
	//���� ����
	char	 m_szSocketBuf[ MAX_SOCKBUF ];
	
	
	afx_msg void OnBnClickedStartserver();
	afx_msg void OnBnClickedCancel();
};

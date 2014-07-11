// OverlappedEventDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "resource.h"
#include "cOverlappedEvent.h"


// COverlappedEventDlg ��ȭ ����
class COverlappedEventDlg : public CDialog
{
// ����
public:
	COverlappedEventDlg(CWnd* pParent = NULL);	// ǥ�� ������

// ��ȭ ���� ������
	enum { IDD = IDD_OVERLAPPEDEVENT_DIALOG };
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
	cOverlappedEvent	m_OverlappedEvent;
	//�޼����� ����ϱ� ���� ���� ����Ʈ �ڽ� ��Ʈ��
	CListBox m_ctOutput;
	//���� ����
	char	 m_szSocketBuf[ MAX_SOCKBUF ];
	
	afx_msg void OnBnClickedStartserver();
	afx_msg void OnBnClickedCancel();
};

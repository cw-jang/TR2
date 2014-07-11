// IOCompletionPortDlg.h : ��� ����
//

#pragma once
#include "cIOCompletionPort.h"
#include "resource.h"

// CIOCompletionPortDlg ��ȭ ����
class CIOCompletionPortDlg : public CDialog
{
// ����
public:
	CIOCompletionPortDlg(CWnd* pParent = NULL);	// ǥ�� ������

// ��ȭ ���� ������
	enum { IDD = IDD_IOCOMPLETIONPORT_DIALOG };

	void OutputMsg( char *szOutputString , ... );


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ����

private:
	//IOCompletionPort��ü�� �����Ѵ�.
	cIOCompletionPort m_IOCompletionPort;

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
	afx_msg void OnBnClickedStartserver();

	//�޼����� ����ϱ� ���� ���� ����Ʈ �ڽ� ��Ʈ��
	CListBox m_ctOutput;

	afx_msg void OnBnClickedCancel();
};

// AsyncSelectDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "cAsyncSocket.h"

#include "resource.h"
#define MAX_SOCKBUF 1024
// CAsyncSelectDlg ��ȭ ����
class CAsyncSelectDlg : public CDialog
{
// ����
public:
	CAsyncSelectDlg(CWnd* pParent = NULL);	// ǥ�� ������

// ��ȭ ���� ������
	enum { IDD = IDD_ASYNCSELECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ����

public:
	//��Ʈ��ũ �̺�Ʈ�� �߻��Ͽ� WM_SOCKETMSG�� ������ 
	//�� �Լ��� ������ �ȴ�.wParam�� �̺�Ʈ�� �߻��� �����̰�
	//lParam�� �̺�Ʈ ������ �����ڵ带 ������ �ִ�.
	LRESULT OnSocketMsg( WPARAM wParam , LPARAM lParam );

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
	//�޼����� ����ϱ� ���� ���� ����Ʈ �ڽ� ��Ʈ��
	CListBox m_ctOutput;
	//���� ����
	char	 m_szSocketBuf[ MAX_SOCKBUF ];
	//WSAAsyncSelect���� ���� ��Ų Ŭ���� ����
	cAsyncSocket m_AsyncSocket;
	

	afx_msg void OnBnClickedStartserver();
};

// HelpDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GameClient.h"
#include "HelpDlg.h"
#include ".\helpdlg.h"


// CHelpDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CHelpDlg, CDialog)
CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpDlg::IDD, pParent)
{
}

CHelpDlg::~CHelpDlg()
{
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLMYUSER, m_ctColMyUser);
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHelpDlg �޽��� ó�����Դϴ�.

BOOL CHelpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CHelpDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialog::OnPaint()��(��) ȣ������ ���ʽÿ�.
}

// EventSelect.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CEventSelectApp:
// �� Ŭ������ ������ ���ؼ��� EventSelect.cpp�� �����Ͻʽÿ�.
//

class CEventSelectApp : public CWinApp
{
public:
	CEventSelectApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CEventSelectApp theApp;

// IOCompletionPort.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CIOCompletionPortApp:
// �� Ŭ������ ������ ���ؼ��� IOCompletionPort.cpp�� �����Ͻʽÿ�.
//

class CIOCompletionPortApp : public CWinApp
{
public:
	CIOCompletionPortApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CIOCompletionPortApp theApp;

// ChatClient.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CChatClientApp:
// �� Ŭ������ ������ ���ؼ��� ChatClient.cpp�� �����Ͻʽÿ�.
//

class CChatClientApp : public CWinApp
{
public:
	CChatClientApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CChatClientApp theApp;

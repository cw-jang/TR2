// ChatServer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CChatServerApp:
// �� Ŭ������ ������ ���ؼ��� ChatServer.cpp�� �����Ͻʽÿ�.
//

class CChatServerApp : public CWinApp
{
public:
	CChatServerApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CChatServerApp theApp;

// NpcServer.h : NpcServer ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"       // �� ��ȣ


// CNpcServerApp:
// �� Ŭ������ ������ ���ؼ��� NpcServer.cpp�� �����Ͻʽÿ�.
//

class CNpcServerApp : public CWinApp
{
public:
	CNpcServerApp();


// ������
public:
	virtual BOOL InitInstance();

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CNpcServerApp theApp;

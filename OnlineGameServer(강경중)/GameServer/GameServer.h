// GameServer.h : GameServer ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"       // �� ��ȣ


// CGameServerApp:
// �� Ŭ������ ������ ���ؼ��� GameServer.cpp�� �����Ͻʽÿ�.
//

class CGameServerApp : public CWinApp
{
public:
	CGameServerApp();


// ������
public:
	virtual BOOL InitInstance();

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGameServerApp theApp;

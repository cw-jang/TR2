// GameClient.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CGameClientApp:
// �� Ŭ������ ������ ���ؼ��� GameClient.cpp�� �����Ͻʽÿ�.
//

class CGameClientApp : public CWinApp
{
public:
	CGameClientApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CGameClientApp theApp;

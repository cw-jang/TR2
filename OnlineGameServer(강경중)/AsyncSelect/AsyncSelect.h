// AsyncSelect.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CAsyncSelectApp:
// �� Ŭ������ ������ ���ؼ��� AsyncSelect.cpp�� �����Ͻʽÿ�.
//

class CAsyncSelectApp : public CWinApp
{
public:
	CAsyncSelectApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CAsyncSelectApp theApp;

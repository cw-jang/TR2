// OverlappedEvent.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// COverlappedEventApp:
// �� Ŭ������ ������ ���ؼ��� OverlappedEvent.cpp�� �����Ͻʽÿ�.
//

class COverlappedEventApp : public CWinApp
{
public:
	COverlappedEventApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern COverlappedEventApp theApp;

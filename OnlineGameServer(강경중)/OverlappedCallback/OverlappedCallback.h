// OverlappedCallback.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// COverlappedCallbackApp:
// �� Ŭ������ ������ ���ؼ��� OverlappedCallback.cpp�� �����Ͻʽÿ�.
//

class COverlappedCallbackApp : public CWinApp
{
public:
	COverlappedCallbackApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern COverlappedCallbackApp theApp;

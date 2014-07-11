// Event.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//
//////////////////////////////////////////////////////////////////////////////////////
//
//  �� ������ �ڵ� ���� �̺�Ʈ�� �̿��Ͽ� g_nInteger�� �������� ����ϴ� �����̴�
//

#include "stdafx.h"
#include <windows.h>
#include <process.h>
using namespace std;
int g_nInteger = 0;
HANDLE g_hEvent;
unsigned int __stdcall ThreadFunc( void* lpVoid )
{

	while( true )
	{
		//g_hEvent��ü�� ��ȣ ����(Signaled)���°� �� ������ ��� ���·� ����.
		WaitForSingleObject( g_hEvent , INFINITE );
		g_nInteger++;
		cout<<"ThreadID : "<<GetCurrentThreadId()<<" : "<<g_nInteger<<endl;
		if( g_nInteger == 9 || g_nInteger == 10 )
			break;
		//g_hEvent��ü�� ���¸� ��ȣ ����(Signaled)���·� �ٲپ� �ش�.
		SetEvent( g_hEvent );
	}
	cout<<"Thread end.. ThreadID : "<<GetCurrentThreadId()<<endl;
	SetEvent( g_hEvent );
	return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int uiThreadID = 0;
    g_hEvent = CreateEvent( NULL , FALSE , FALSE , NULL );
	if( NULL == g_hEvent )
	{
		cout<<"CreateEvent() �Լ� ȣ�� ���� : "<<
			GetLastError()<<endl;
	}

	//������ ����
	HANDLE hThread1 = (HANDLE)_beginthreadex( NULL , 0 , ThreadFunc , NULL , CREATE_SUSPENDED  , &uiThreadID );
	cout<<"Create Thread ID : "<<uiThreadID<<endl;
	HANDLE hThread2 = (HANDLE)_beginthreadex( NULL , 0 , ThreadFunc , NULL , CREATE_SUSPENDED  , &uiThreadID );
	cout<<"Create Thread ID : "<<uiThreadID<<endl;
	if( NULL == hThread1 || NULL == hThread2 )
	{
		cout<<"[ERROR] Thread1 or hThread2 is NULL.."<<endl;
		return 0;
	}

	//�����带 ����
	ResumeThread( hThread1 );
	ResumeThread( hThread2 );
	//g_hEvent��ü�� ���¸� ��ȣ ����(Signaled)���·� �ٲپ� �ش�.
	SetEvent( g_hEvent );
	
	//�����尡 ����� ������ ��ٸ���.
	WaitForSingleObject( hThread1 , INFINITE );
	WaitForSingleObject( hThread2 , INFINITE );

	//g_hEvent��ü�� ����� �����Ѵ�.
	CloseHandle( g_hEvent );

	//������ �ڵ��� �ݴ´�.
	CloseHandle( hThread1 );
	CloseHandle( hThread2 );
	return 0;
}


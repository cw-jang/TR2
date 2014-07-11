// Mutex.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//
//////////////////////////////////////////////////////////////////////////////////////
//
//  �� ������ ���ؽ��� �̿��Ͽ� g_nInteger�� �������� ����ϴ� �����̴�
//

#include "stdafx.h"
#include <windows.h>
#include <process.h>
using namespace std;
int g_nInteger = 0;
HANDLE g_hMutex;
unsigned int __stdcall ThreadFunc( void* lpVoid )
{

	while( true )
	{
		//g_hEvent��ü�� ��ȣ ����(Signaled)���°� 
		//�� ������ ��� ���·� ����.
		WaitForSingleObject( g_hMutex , INFINITE );
		g_nInteger++;
		cout<<"ThreadID : "<<GetCurrentThreadId()<<" : "
			<<g_nInteger<<endl;
		if( g_nInteger == 9 || g_nInteger == 10 )
			break;
		//g_hMutex��ü�� �������� ���� �����ν� 
		//��ü�� ���¸� ��ȣ ����(Signaled)���·� �ٲپ� �ش�.
		ReleaseMutex( g_hMutex );
	}
	cout<<"Thread end.. ThreadID : "
		<<GetCurrentThreadId()<<endl;
	ReleaseMutex( g_hMutex );
	return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int uiThreadID = 0;
    g_hMutex = CreateMutex( NULL , FALSE , NULL );
	if( NULL == g_hMutex )
	{
		cout<<"CreateMutex() �Լ� ȣ�� ���� : "<<
			GetLastError()<<endl;
	}

	//������ ����
	HANDLE hThread1 = (HANDLE)_beginthreadex( NULL , 0 
		,ThreadFunc,NULL,CREATE_SUSPENDED,&uiThreadID );
	cout<<"Create Thread ID : "<<uiThreadID<<endl;

	HANDLE hThread2 = (HANDLE)_beginthreadex( NULL , 0 
		,ThreadFunc,NULL,CREATE_SUSPENDED,&uiThreadID );
	cout<<"Create Thread ID : "<<uiThreadID<<endl;
	if( NULL == hThread1 || NULL == hThread2 )
	{
		cout<<"[ERROR] Thread1 or hThread2 is NULL.."<<endl;
		return 0;
	}

	//�����带 ����
	ResumeThread( hThread1 );
	ResumeThread( hThread2 );
	//g_hMutex��ü�� �������� ���� �����ν� 
	//��ü�� ���¸� ��ȣ ����(Signaled)���·� �ٲپ� �ش�.
	ReleaseMutex( g_hMutex );
	
	//�����尡 ����� ������ ��ٸ���.
	WaitForSingleObject( hThread1 , INFINITE );
	WaitForSingleObject( hThread2 , INFINITE );

    //g_hMutex��ü�� �ڵ��� �ݴ´�.
	CloseHandle( g_hMutex );
	//������ �ڵ��� �ݴ´�.
	CloseHandle( hThread1 );
	CloseHandle( hThread2 );
	return 0;
}


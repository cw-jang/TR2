// CriticalSection.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//
/////////////////////////////////////////////////////////////////////////////////////
//
//  �� ������ ũ��Ƽ�� ���� ��ü�� ����Ͽ� ����ȭ�� �����̴�.
//
#include "stdafx.h"
#include <windows.h>
#include <process.h>
using namespace std;

int g_nInteger = 0;
CRITICAL_SECTION csInteger;
	
unsigned int __stdcall ThreadFunc( void* lpVoid )
{

	while( true )
	{
		//csInteger��ü ������ ȹ��
		EnterCriticalSection( &csInteger );
		g_nInteger++;
		cout<<"ThreadID : "<<GetCurrentThreadId()<<" : "<<g_nInteger<<endl;
		if( g_nInteger == 9 || g_nInteger == 10 )
			break;
		//csInteger��ü ������ ����
		LeaveCriticalSection( &csInteger );
	}
	cout<<"Thread end.. ThreadID : "<<GetCurrentThreadId()<<endl;
	LeaveCriticalSection( &csInteger );
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{

	unsigned int uiThreadID = 0;
    //ũ��Ƽ�� ���� ��ü �ʱ�ȭ
	InitializeCriticalSection( &csInteger );

	//������ ����
	HANDLE hThread1 = (HANDLE)_beginthreadex( NULL , 0 , ThreadFunc , NULL , CREATE_SUSPENDED  , &uiThreadID );
	cout<<"Create Thread ID : "<<uiThreadID<<endl;
	HANDLE hThread2 = (HANDLE)_beginthreadex( NULL , 0 , ThreadFunc , NULL , CREATE_SUSPENDED  , &uiThreadID );
	cout<<"Create Thread ID : "<<uiThreadID<<endl;
	if( NULL == hThread1 || NULL == hThread2 )
	{
		cout<<"[ERROR] Thread1 or Thread2 is NULL.."<<endl;
		return 0;
	}
	ResumeThread( hThread1 );
	ResumeThread( hThread2 );
	//�����尡 ����� ������ ��ٸ���.
	WaitForSingleObject( hThread1 , INFINITE );
	WaitForSingleObject( hThread2 , INFINITE );

	//������ �ڵ��� �ݴ´�.
	CloseHandle( hThread1 );
	CloseHandle( hThread2 );
	return 0;
}



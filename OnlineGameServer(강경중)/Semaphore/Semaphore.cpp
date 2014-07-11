// Semaphore.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
HANDLE g_hSemaphore;
unsigned int __stdcall ThreadFunc( void* lpVoid )
{
	LONG lPre = 0;
	while( true )
	{
		//g_hEvent��ü�� ��ȣ ����(Signaled)���°� 
		//�� ������ ��� ���·� ����.
		WaitForSingleObject( g_hSemaphore , INFINITE );
		g_nInteger++;
		cout<<"ThreadID : "<<GetCurrentThreadId()<<" : "
			<<g_nInteger<<endl;
		if( g_nInteger >= 8 )
			break;
		//g_hSemaphore��ü�� �������� ���� �����ν� 
		//��ü�� ���¸� ��ȣ ����(Signaled)���·� �ٲپ� �ش�.
		
		ReleaseSemaphore( g_hSemaphore , 1 , &lPre );
	}
	cout<<"Thread end.. ThreadID : "
		<<GetCurrentThreadId()<<endl;
	ReleaseSemaphore( g_hSemaphore , 1 , &lPre );
	return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int uiThreadID = 0;
    g_hSemaphore = CreateSemaphore( NULL , 0 , 1 , NULL );
	if( NULL == g_hSemaphore )
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

	HANDLE hThread3 = (HANDLE)_beginthreadex( NULL , 0 
		,ThreadFunc,NULL,CREATE_SUSPENDED,&uiThreadID );
	cout<<"Create Thread ID : "<<uiThreadID<<endl;

	if( NULL == hThread1 || NULL == hThread2 | NULL == hThread3 )
	{
		cout<<"[ERROR] Thread1 or hThread2 is NULL.."<<endl;
		return 0;
	}

	//�����带 ����
	ResumeThread( hThread1 );
	ResumeThread( hThread2 );
	ResumeThread( hThread3 );
	//g_hMutex��ü�� �������� ���� �����ν� 
	//��ü�� ���¸� ��ȣ ����(Signaled)���·� �ٲپ� �ش�.
	LONG lPre = 0;
	ReleaseSemaphore( g_hSemaphore , 1 , &lPre );
	
	//�����尡 ����� ������ ��ٸ���.
	WaitForSingleObject( hThread1 , INFINITE );
	WaitForSingleObject( hThread2 , INFINITE );

    //g_hMutex��ü�� �ڵ��� �ݴ´�.
	CloseHandle( g_hSemaphore );
	//������ �ڵ��� �ݴ´�.
	CloseHandle( hThread1 );
	CloseHandle( hThread2 );
	return 0;
}
SOCKET socket = WSASocket( 
{
	///////////////////////////////////////////////////////////////////////////////
	//CompletionPort��ü�� �����ϴ� ��
	HANDLE hIOCPHandle;
	//Completion Port��ü�� �����Ѵ�.	
	hIOCPHandle = CreateIoCompletionPort( NULL, hIOCP, NULL , 0);
	//87�� ������ �߸��� �μ� �����ε�. �̰��� ���� ������ �ϸ� ���´�.
	if( NULL == hIOCPHandle )
	{
		cout<<"CreateIoCompletionPort()�Լ� ����"<<endl;
		return false;
	}

	///////////////////////////////////////////////////////////////////////////////
	//CompletionPort��ü�� ������ �����ϴ� ��
	socket = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_IP
		, NULL, 0, WSA_FLAG_OVERLAPPED);
	HANDLE hIOCP = CreateIoCompletionPort( (HANDLE)socket, hIOCPHandle, 1 , 0);
	//87�� ������ �߸��� �μ� �����ε�. �̰��� ���� ������ �ϸ� ���´�.
	if( NULL == hIOCP )
	{
		cout<<"CreateIoCompletionPort()�Լ� ���� : "
			<<GetLastError()<<endl;
		return false;
	}


	m_hIOCP = hIOCP;


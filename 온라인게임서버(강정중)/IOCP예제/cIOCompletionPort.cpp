#include <Windows.h>
#include "cIOCompletionPort.h"
#include "IOCompletionPortDlg.h"

// WSARecv와 WSASend의 Overlapped I/O 작업 처리를 위한 쓰레드
unsigned int WINAPI CallWorkerThread(LPVOID p)
{
	cIOCompletionPort* pOverlappedEvent = (cIOCompletionPort*)p;
	pOverlappedEvent->WorkerThread();
	return 0;
}

// Clinet의 접속을 받기위한 쓰레드
unsigned int WINAPI CallAccepterThread(LPVOID p)
{
	cIOCompletionPort* pOverlappedEvent = (cIOCompletionPort*) p;
	pOverlappedEvent->AccepterThread();
	return 0;
}

cIOCompletionPort::cIOCompletionPort(void)
{
	///////////////////////////////////
	// 모든 멤버 변수들 초기화
	m_pMainDlg = NULL;
	m_bWorkerRun = true;
	m_bAccepterRun = true;
	m_nClientCnt = 0;
	m_hAccepterThread = NULL;
	m_hIOCP = NULL;
	m_socketListen = INVALID_SOCKET;
	ZeroMemory( m_szSocketBuf, 1024 );
	for(int i=0; MAX_WORKERTHREAD; i++)
		m_hWorkerThread[i] = NULL;

	// client 정보를 저장한 구조체를 생성
	m_pClientInfo = new stClientInfo[MAX_CLIENT];
}

cIOCompletionPort::~cIOCompletionPort(void)
{
	// 윈속의 사용을 끝낸다.
	WSACleanup();
	
	// 다 사용한 객체를 삭제
	if( m_pClientInfo )
	{
		delete [] m_pClientInfo;
		m_pClientInfo = NULL;
	}
}

// 소켓을 초기화하는 함수
bool cIOCompletionPort::InitSocket()
{
	WSADATA wsaData;

	// 윈속을 버전 2,2로 초기화한다
	int nRet = WSAStartup( MAKEWORD(2,2), &wsaData );
	if( 0 != nRet )
	{
		m_pMainDlg->OutputMsg("[에러] WSAStartup() 함수 실패: %d", WSAGetLastError() );
		return false;
	}

	// 연결지향형 TCP, Overlapped I/O소켓을 생성
	m_socketListen = WSASocket( AF_INET, SOCK_STREAM,
								IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED );

	if( INVALID_SOCKET == m_socketListen )
	{
		m_pMainDlg->OutputMsg("[에러] socket()함수실패 : %d", WSAGetLastError() );
		return false;
	}

	m_pMainDlg->OutputMsg("소켓 초기화 성공");
	return true;
}

// 해당 소켓을 종료시킨다.
void cIOCompletionPort::CloseSocket( stClientInfo* pClientInfo, bool bIsForce )
{
	struct linger stLinger = {0,0}; // SO_DONTLINGER로 설정

	// bIsForce가 true이면 SO_LINGER, timeout=0으로 설정하여
	// 강제 종료 시킨다. 주의: 데이터 손실이 있을 수 있음
	if( true == bIsForce )
		stLinger.l_onoff = 1;

	// socketClose 소켓의 데이터 송수신을 모두 중단시킨다.
	shutdown( pClientInfo->m_socketClient, SD_BOTH );

	// 소켓 옵션을 설정한다.
	setsockopt( pClientInfo->m_socketClient, SOL_SOCKET,
				SO_LINGER, (char*)&stLinger, sizeof(stLinger) );

	// 소켓 연결을 종료시킨다.
	closesocket( pClientInfo->m_socketClient );
	pClientInfo->m_socketClient = INVALID_SOCKET;
}

// 서버의 주소 정보를 소켓과 연결시키고 접속 요청을 받기 위해 소켓을 등록하는 함수
bool cIOCompletionPort:: BindandListen( int nBindPort )
{
}
#pragma once

#define MAX_RECVBUFCNT				100				//iocp recv packet pool ���� ���۰���,,
#define MAX_SENDBUFCNT				100				//iocp send packet pool ���� ���۰���,,
#define MAX_QUEUESIZE				10000           //default queue size in cQueue class
#define MAX_QUEUESENDSIZE			1000
#define MAX_PBUFSIZE				4096			//PacketPool���� ���� �Ѱ��� size
#define MAX_VPBUFSIZE				1024 * 40		//���� ���� size
#define MAX_RINGBUFSIZE				1024 * 100		//Ringbuffer size
#define DEFUALT_BUFCOUNT			10				//���� ���� ����.
#define PACKET_SIZE_LENGTH			4
#define PACKET_TYPE_LENGTH			2
#define MAX_IP_LENGTH				20
#define MAX_PROCESS_THREAD			1
#define MAX_WORKER_THREAD			10


#ifdef NETLIB_EXPORTS
#define NETLIB_API __declspec(dllexport)
#else
#define NETLIB_API __declspec(dllimport)
#endif


enum eOperationType
{ 
	//Work IOCP operation
	OP_SEND , 
	OP_RECV , 
	OP_ACCEPT ,
	//Process IOCP operation
	OP_CLOSE,		//���� ����ó��
	OP_RECVPACKET,	//���� �� ��Ŷ ó��
	OP_SYSTEM		//�ý��� �޽��� ó��
};

/////////////////////////////////////////////////
//Connection������ ���� config����ü
typedef struct _INITCONFIG
{
	int				nIndex;				//Connection index
	SOCKET			sockListener;		//Listen Socket
	//recv Ringbuffer size  = nRecvBufCnt * nRecvBufSize
	int				nRecvBufCnt;		
	//send Ringbuffer size  = nSendBufCnt * nSendBufSize
	int				nSendBufCnt;		
	int				nRecvBufSize;		
	int				nSendBufSize;		
	int				nProcessPacketCnt;	//�ִ� ó�� ��Ŷ�� ����
	int				nServerPort;		
	int				nWorkerThreadCnt;	//i/o ó���� ���� thread ����
	int				nProcessThreadCnt;	//��Ŷó���� ���� thread ����

	_INITCONFIG()
	{
		ZeroMemory( this , sizeof( INITCONFIG ) );
	}
}INITCONFIG;


typedef struct _OVERLAPPED_EX		
{
	WSAOVERLAPPED			s_Overlapped;
	WSABUF					s_WsaBuf;
	int						s_nTotalBytes;	
	DWORD					s_dwRemain;
	char*					s_lpSocketMsg;	// Send/Recv Buffer.
	eOperationType			s_eOperation;
	void*					s_lpConnection;
	_OVERLAPPED_EX( void* lpConnection )
	{
		ZeroMemory( this , sizeof( OVERLAPPED_EX ) );
		s_lpConnection = lpConnection;
	}
} OVERLAPPED_EX, *LPOVERLAPPED_EX;


typedef struct _PROCESSPACKET
{
	eOperationType	s_eOperationType;
	WPARAM			s_wParam;
	LPARAM			s_lParam;
	_PROCESSPACKET()
	{
		Init();
	}
	void Init()
	{
		ZeroMemory( this, sizeof( PROCESSPACKET ) );
	}

} PROCESSPACKET, *LPPROCESSPACKET;


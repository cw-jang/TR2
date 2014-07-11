#pragma once
#include "cTickThread.h"
#include "cPlayer.h"

enum eGameServerSystemMsg{
	SYSTEM_TEMPMSG,
	SYSTEM_UPDATE_TEMPPLAYERPOS,
};

class cIocpGameServer : public cIocpServer , cSingleton
{
	DECLEAR_SINGLETON( cIocpGameServer );

public:
	cIocpGameServer(void);
	~cIocpGameServer(void);

	union FuncProcess
	{
		void (*funcProcessPacket)( cPlayer* pPlayer,  DWORD dwSize , char* pRecvedMsg );

		FuncProcess()
		{
			funcProcessPacket = NULL;
		}
	};

	//client�� ���� ������ �Ǿ��� �� ȣ��Ǵ� �Լ�
	virtual	bool	OnAccept( cConnection *lpConnection );
	//client���� packet�� �������� �� ���� �� �ְ� ó���Ǿ����� ��Ŷó��
	virtual	bool	OnRecv(cConnection* lpConnection,  DWORD dwSize , char* pRecvedMsg);
	//client���� packet�� �������� �� ���� �� ���� ��ٷ� ó�� �Ǵ� ��Ŷó��
	virtual	bool	OnRecvImmediately(cConnection* lpConnection,  DWORD dwSize , char* pRecvedMsg);
	//client�� ������ ����Ǿ��� �� ȣ��Ǵ� �Լ�
	virtual	void	OnClose(cConnection* lpConnection);
	
	virtual bool	OnSystemMsg( cConnection* lpConnection , DWORD dwMsgType , LPARAM lParam );

	//���� ���� �Լ�
	virtual bool	ServerStart();

	bool			ConnectToNpcServer();

	//.iniȭ�Ϸ� ���� ���ڿ��� �о�´�.
	int				GetINIString( char* szOutStr , char* szAppName , char* szKey , int nSize , char* szFileName );
	//.iniȭ�Ϸ� ���� ���ڸ� �о�´�.
	int				GetINIInt( char* szAppName , char* szKey , char* szFileName );

	//ƽ������ ������ ��ȯ
	inline cTickThread*     GetTickThread() { return m_pTickThread; }
	inline DWORD			GetServerTick() { return m_pTickThread->GetTickCount(); }
	//��Ŷó�� �Լ� ����
	void			InitProcessFunc();

	void			ProcessSystemMsg( cPlayer* pPlayer , DWORD dwMsgType , LPARAM lParam );

	//������ Ű�� ����.
	DWORD			GeneratePrivateKey() { return ++m_dwPrivateKey; }

	inline cConnection* GetNpcServerConn() { return m_pNpcServerConn; }

private:
	char m_szLogFileName[ MAX_LOGFILENAME_LENGTH ];
	//ƽ ������
	cTickThread*		m_pTickThread;
	//��Ŷ ó���Լ� ������
	FuncProcess			m_FuncProcess[ MAX_PROCESSFUNC ];
	DWORD				m_dwPrivateKey;
	cConnection*		m_pNpcServerConn;
};

CREATE_FUNCTION( cIocpGameServer , IocpGameServer );

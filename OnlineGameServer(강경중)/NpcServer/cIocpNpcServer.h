#pragma once
#include "cTickThread.h"

enum eNpcServerSystemMsg{
	SYSTEM_UPDATE_NPCPOS,
};

class cIocpNpcServer : public cIocpServer , cSingleton
{
	DECLEAR_SINGLETON( cIocpNpcServer );

public:
	cIocpNpcServer(void);
	~cIocpNpcServer(void);

	struct FuncProcess
	{
		void (*funcProcessPacket)( cConnection* pConnection,  DWORD dwSize , char* pRecvedMsg );

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

	inline cConnection* GetGameServerConn() { return m_pGameServerConn; }
	//.iniȭ�Ϸ� ���� ���ڿ��� �о�´�.
	int				GetINIString( char* szOutStr , char* szAppName , char* szKey , int nSize , char* szFileName );
	//.iniȭ�Ϸ� ���� ���ڸ� �о�´�.
	int				GetINIInt( char* szAppName , char* szKey , char* szFileName );

	//ƽ������ ������ ��ȯ
	inline cTickThread*     GetTickThread() { return m_pTickThread; }
	inline DWORD	GetServerTick() { return m_pTickThread->GetTickCount(); }

	//��Ŷó�� �Լ� ����
	void			InitProcessFunc();

	void			ProcessSystemMsg( cConnection* pConnection, DWORD dwMsgType , LPARAM lParam );
private:
	char m_szLogFileName[ MAX_LOGFILENAME_LENGTH ];
	//ƽ ������
	cTickThread*		m_pTickThread;
	//��Ŷ ó���Լ� ������
	FuncProcess			m_FuncProcess[ MAX_PROCESSFUNC ];
	cConnection*		m_pGameServerConn;
	
};

CREATE_FUNCTION( cIocpNpcServer , IocpNpcServer );
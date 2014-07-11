#pragma once
#include "cPlayer.h"

class cPlayerManager : public cSingleton , cMonitor
{
	DECLEAR_SINGLETON( cPlayerManager );
public:
	cPlayerManager(void);
	~cPlayerManager(void);

	typedef pair< DWORD , cPlayer* >  PLAYER_PAIR;
	typedef map< DWORD , cPlayer* >   PLAYER_MAP;
	typedef PLAYER_MAP::iterator	  PLAYER_IT;	

	bool CreatePlayer( INITCONFIG &initConfig , DWORD dwMaxPlayer );
	bool AddPlayer( cPlayer* pPlayer );
	bool RemovePlayer( cPlayer* pPlayer );
	cPlayer* FindPlayer( DWORD dwPkey );
	inline int GetPlayerCnt() { return (int)m_mapPlayer.size(); }

	void CreateTempPlayer( int nTempPlayerCnt );
	void DestroyTempPlayer();
	void UpdateTempPlayerPos();

	void CheckKeepAliveTick( DWORD dwServerTick );
	
	////////////////////////////////////////////////////////////////
	//���� ���� �Լ���..

	//���� ���忡 ���ӵǾ��ִ� �÷��̾�� ������ ������ Ŭ���̾�Ʈ�� ����
	//����� ����ڸ� �������� �� �� �� �����Ƿ� �������� ��Ŷ���� ����
	void Send_WorldPlayerInfosToConnectPlayer( cPlayer* pPlayer );
	//�ٸ� �÷��̾�鿡�� �÷��̾ ���� �Ͽ��ٴ� ���� �˸�
	void Send_LoginPlayer( cPlayer* pPlayer );
	//�ٸ� �÷��̾�鿡�� �÷��̾ ������ �����Ͽ��ٴ� ���� �˸�
	void Send_LogoutPlayer( cPlayer* pPlayer );
	//�÷��̾ �̵��Ͽ��ٴ� ���� ���忡 �ִ� ��� �ٸ� �÷��̾�鿡�� �˸�
	void Send_MovePlayer( cPlayer* pPlayer );
	//�÷��̾�� NPC�������� ���� ���۸� �״�� ������.
	void Send_RecvBufferFromNpcServer( char* pNpcInfo , DWORD dwSize );
protected:
	PLAYER_MAP		m_mapPlayer;
	PLAYER_MAP		m_mapTempPlayer;

	cPlayer*		m_pPlayer;
	cPlayer*		m_pTempPlayer;
	cMonitor		m_csPlayer;

};

CREATE_FUNCTION( cPlayerManager , PlayerManager );


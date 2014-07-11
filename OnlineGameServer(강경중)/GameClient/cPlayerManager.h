#pragma once
#include "cPlayer.h"

class cPlayerManager : public cSingleton
{
	DECLEAR_SINGLETON( cPlayerManager );
public:
	cPlayerManager(void);
	~cPlayerManager(void);

	typedef pair< DWORD , cPlayer* >  PLAYER_PAIR;
	typedef map< DWORD , cPlayer* >   PLAYER_MAP;
	typedef PLAYER_MAP::iterator	  PLAYER_IT;	

	//�÷��̾ ����
	bool CreatePlayer( DWORD dwMaxPlayer );
	//�÷��̾ �߰�
	bool AddPlayer( cPlayer* pPlayer );
	//�÷��̾ ����
	bool RemovePlayer( DWORD dwPKey );
	//���� Ű�� �ش��ϴ� �÷��̾ ��ȯ
	cPlayer* FindPlayer( DWORD dwPKey );
	//�� �÷��̾� �����͸� ��ȯ�Ѵ�.
	cPlayer* GetEmptyPlayer();
	//�� �÷��̾� ������ ��ȯ�Ѵ�.
	cPlayer* GetMyPlayer() { return m_pMyPlayer; }
	//��ǥ�� �ִ� �÷��̾� ������ ��ȯ
	cPlayer* GetPlayerByPos( DWORD dwTagetPos );
	void SetMyPlayer( cPlayer* pPlayer ) { m_pMyPlayer = pPlayer; }
	//�÷��̾ ��´�.
	void DrawPlayer( CDC* pDC );
	inline int __fastcall	GetPlayerCnt() { return (int)m_mapPlayer.size(); }

	bool UpdatePlayersPos();
	

protected:
	PLAYER_MAP		m_mapPlayer;
	cPlayer*		m_pPlayer;

	cPlayer*		m_pMyPlayer;

	DWORD			m_dwMaxPlayerCnt;
};

CREATE_FUNCTION( cPlayerManager , PlayerManager );

#pragma once

class cAreaManager : public cSingleton , cMonitor
{
	DECLEAR_SINGLETON( cAreaManager );
public:
	cAreaManager(void);
	~cAreaManager(void);
	typedef pair< cPlayer* , BYTE >  AREA_PAIR;
	typedef map< cPlayer* , BYTE >   AREA_MAP;
	typedef AREA_MAP::iterator		  AREA_IT;	

	bool AddPlayerToArea( cPlayer* pPlayer , BYTE byArea );
	//�������� �÷��̾ �����Ѵ�.
	bool RemovePlayerFromArea( cPlayer* pPlayer , BYTE byArea );
	//�÷��̾ �̵��� �Ͽ� ������ �ٲ�
	bool TransAreaPlayer( cPlayer* pPlayer );
	//�÷��̾ ������ �ִ� ������ ���Ѵ�.
	void UpdateActiveAreas( cPlayer* pPlayer );
	//���� ��ǥ�� �ش��ϴ� ������ ��ȯ
	BYTE GetPosToArea( DWORD dwPos );

	//�÷��̾ �̵��Ͽ��ٴ� ���� Ȱ�� ������ �ִ� �ٸ� �÷��̾�鿡�� �˸�
	void Send_MovePlayerToActiveAreas( cPlayer* pPlayer );
	//�÷��̾ �̵��Ͽ��ٴ� ���� ��Ȱ�� ������ �ִ� �ٸ� �÷��̾�鿡�� �˸�
	void Send_MovePlayerToInActiveAreas( cPlayer* pPlayer );

private:
	AREA_MAP		m_mapArea[ MAX_AREA ];
	cMonitor		m_csArea;
};

CREATE_FUNCTION( cAreaManager , AreaManager );

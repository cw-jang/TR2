#include "StdAfx.h"
#include ".\cplayermanager.h"

IMPLEMENT_SINGLETON( cPlayerManager );

cPlayerManager::cPlayerManager(void)
{
	m_pPlayer = NULL;
	m_dwMaxPlayer = 0;
}

cPlayerManager::~cPlayerManager(void)
{
	if( NULL == m_pPlayer )
		delete [] m_pPlayer;
}

bool cPlayerManager::CreatePlayer( DWORD dwMaxPlayer )
{
	m_pPlayer = new cPlayer[ dwMaxPlayer ];
	m_dwMaxPlayer = dwMaxPlayer;
	return true;
}

bool cPlayerManager::AddPlayer( cPlayer* pPlayer )
{
	cMonitor::Owner lock( m_csPlayer );
	PLAYER_IT player_it;
	player_it = m_mapPlayer.find( pPlayer->GetPKey() );
	//�̹� ���ӵǾ� �ִ� �÷��̾���
	if( player_it != m_mapPlayer.end() )
	{
		LOG( LOG_ERROR_NORMAL , 
			"SYSTEM | cPlayerManager::AddPlayer() | PKey[%d]�� �̹� m_mapPlayer�� �ֽ��ϴ�.",
			pPlayer->GetPKey() );
		return false;
	}
	
	m_mapPlayer.insert( PLAYER_PAIR( pPlayer->GetPKey() , pPlayer ) );
	return true;
}

bool cPlayerManager::RemovePlayer( DWORD dwPKey )
{
	cMonitor::Owner lock( m_csPlayer );
	PLAYER_IT player_it;
	player_it = m_mapPlayer.find( dwPKey );
	//�ش� �÷��̾ ���� ���
	if( player_it == m_mapPlayer.end() )
	{
		LOG( LOG_ERROR_NORMAL , 
			"SYSTEM | cPlayerManager::RemovePlayer() | PKey[%d]�� m_mapPlayer�� �����ϴ�.",
			dwPKey );
		return false;
	}
	//�÷��̾� �ʱ�ȭ
	cPlayer* pPlayer = (cPlayer*)player_it->second;
	pPlayer->Init();

    m_mapPlayer.erase( dwPKey );
	return true;
}
cPlayer* cPlayerManager::GetEmptyPlayer()
{
	for( int i = 0 ; i < (int)m_dwMaxPlayer; i++ )
	{
		if( m_pPlayer[ i ].GetPKey() == 0 )
			return &m_pPlayer[ i ];
	}
	return NULL;
}

cPlayer* cPlayerManager::FindPlayer( DWORD dwPKey )
{
	cMonitor::Owner lock( m_csPlayer );
	PLAYER_IT player_it;
	player_it = m_mapPlayer.find( dwPKey );
	//�ش� �÷��̾ ���� ���
	if( player_it == m_mapPlayer.end() )
	{
		LOG( LOG_ERROR_NORMAL , 
			"SYSTEM | cPlayerManager::FindPlayer() | PKey[%d]�� m_mapPlayer�� �����ϴ�.",
			dwPKey );
		return NULL;
	}
	return (cPlayer*)player_it->second;
}
void cPlayerManager::DetectPlayerFromNpc( cDetectNpc* pDetectNpc )
{
	if( NULL == pDetectNpc )
		return;

	DWORD dwNpcPos = pDetectNpc->GetPos();
	cMonitor::Owner lock( m_csPlayer );
	PLAYER_IT player_it;
	for( player_it = m_mapPlayer.begin() ; player_it != m_mapPlayer.end() ; player_it++ )
	{
		cPlayer* pPlayer = (cPlayer*)player_it->second;
		if( pPlayer->GetIsDead() == true )
			continue;
		DWORD dwPlayerPos = pPlayer->GetPos();
		int nDiff1 = ( dwPlayerPos % COL_LINE ) - ( dwNpcPos % COL_LINE );
		int nDiff2 = ( dwPlayerPos / COL_LINE ) - ( dwNpcPos / COL_LINE );
		if( abs( nDiff1 ) > 1 || abs( nDiff2 ) > 1 )
			continue;
		pDetectNpc->SetTagetPlayerPKey( pPlayer->GetPKey() );
		pDetectNpc->SetEvent( EVENT_PLAYER_APPEAR );
	}
}


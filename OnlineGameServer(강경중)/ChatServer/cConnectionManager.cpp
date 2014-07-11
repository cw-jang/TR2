#include "StdAfx.h"
#include ".\cconnectionmanager.h"

IMPLEMENT_SINGLETON( cConnectionManager )

cConnectionManager::cConnectionManager(void)
{
	m_pConnection = NULL;
}

cConnectionManager::~cConnectionManager(void)
{
	if( NULL == m_pConnection )
		delete [] m_pConnection;
}

bool cConnectionManager::CreateConnection( INITCONFIG &initConfig , DWORD dwMaxConnection )
{
	cMonitor::Owner lock( m_csConnection );

	m_pConnection = new cConnection[ dwMaxConnection ];
	for( int i=0 ; i < (int)dwMaxConnection ; i++ )
	{
		initConfig.nIndex = i ;
		if( m_pConnection[i].CreateConnection( initConfig ) ==	false )
			return false;

	}
	return true;
}

bool cConnectionManager::AddConnection( cConnection* pConnection )
{
	cMonitor::Owner lock( m_csConnection );
	CONN_IT conn_it;
	conn_it = m_mapConnection.find( pConnection );
	//�̹� ���ӵǾ� �ִ� �����̶��
	if( conn_it != m_mapConnection.end() )
	{
		LOG( LOG_INFO_NORMAL , 
			"SYSTEM | cConnectionManager::AddConnection() | index[%d]�� �̹� m_mapConnection�� �ֽ��ϴ�.",
			pConnection->GetIndex() );
		return false;
	}
	m_mapConnection.insert( CONN_PAIR( pConnection , GetTickCount() ) );
	return true;
}

bool cConnectionManager::RemoveConnection( cConnection* pConnection )
{
	cMonitor::Owner lock( m_csConnection );
	CONN_IT conn_it;
	conn_it = m_mapConnection.find( pConnection );
	//���ӵǾ� �ִ� ������ ���°��
	if( conn_it == m_mapConnection.end() )
	{
		LOG( LOG_INFO_NORMAL , 
			"SYSTEM | cConnectionManager::RemoveConnection() | index[%d]�� m_mapConnection�� �����ϴ�.",
			pConnection->GetIndex() );
		return false;
	}
	m_mapConnection.erase( pConnection );
	return true;
}

void cConnectionManager::BroadCast_Chat( char* szIP , char* szChatMsg )
{
	cMonitor::Owner lock( m_csConnection );
	CONN_IT conn_it;
	for( conn_it = m_mapConnection.begin() ; conn_it != m_mapConnection.end() ; conn_it++ )
	{
		cConnection* pConnection = (cConnection*)conn_it->first;
		Packet_Chat* pChat = (Packet_Chat*)pConnection->PrepareSendPacket( sizeof( Packet_Chat ) );
		if( NULL == pChat )
			continue;
		pChat->s_sType = PACKET_CHAT;
		strncpy( pChat->s_szChatMsg , szChatMsg , MAX_CHATMSG );
		strncpy( pChat->s_szIP , szIP , MAX_IP );
		pConnection->SendPost();
	}
}
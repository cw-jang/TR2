#include "StdAfx.h"
#include ".\cplayer.h"

cPlayer::cPlayer(void)
{
	srand( GetTickCount() );
	Init();
}

cPlayer::~cPlayer(void)
{
}

//������ �ʱ�ȭ ��Ų��.
void cPlayer::Init()
{
	m_dwPKey = 0;
	ZeroMemory( m_szId , MAX_ID_LENGTH );
	ZeroMemory( m_szNickName , MAX_NICKNAME_LENGTH );
	ZeroMemory( m_szName , MAX_NAME_LENGTH );
	
	m_dwPos = 0;	//�÷��̾� ��ġ          
	m_byLevel = 0;	//����
	m_byStr = 0;	//���ݷ�
	m_byDur = 0;	//����
	m_dwHp = 0;		//�����
	m_dwExp = 0;	//����ġ
	m_dwTPos = 0;   //������ ��ġ
	m_bVisible = true;
}
void cPlayer::SetPlayerInfoFromPacket( char* pPlayerInfo )
{
	VBuffer()->SetBuffer( pPlayerInfo );

	VBuffer()->GetInteger( (int&)m_dwPKey );			//����Ű
	VBuffer()->GetString( m_szId );
	VBuffer()->GetString( m_szNickName );
	VBuffer()->GetString( m_szName );
	VBuffer()->GetInteger( (int&)m_dwPos );
	VBuffer()->GetChar( (char&)m_byLevel );
	VBuffer()->GetChar( (char&)m_byStr );
	VBuffer()->GetChar( (char&)m_byDur );
	VBuffer()->GetInteger( (int&)m_dwHp );
	VBuffer()->GetInteger( (int&)m_dwExp );
}
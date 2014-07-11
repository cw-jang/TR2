#include "StdAfx.h"
#include ".\cprocesspacket.h"

cProcessPacket::cProcessPacket(void)
{
}

cProcessPacket::~cProcessPacket(void)
{
}
void cProcessPacket::fnNPCLoginPlayerCn( cConnection* pConnection,  DWORD dwSize , char* pRecvedMsg )
{
	NPCLoginPlayerCn* pLoginPlayer = (NPCLoginPlayerCn*)pRecvedMsg;
    //�÷��̾� �߰� 
	cPlayer* pPlayer = PlayerManager()->GetEmptyPlayer();
	if( NULL == pPlayer )
	{
		LOG( LOG_INFO_LOW,
		"SYSTEM | cProcessPacket::fnNPCLoginPlayerCn() | �� �̻� �÷��̾ NPC������ �α��� �� �����ϴ�.");
		return;	
	}
	pPlayer->SetPKey( pLoginPlayer->s_dwPKey );
	pPlayer->SetPos( pLoginPlayer->s_dwPos );
	PlayerManager()->AddPlayer( pPlayer );

	NpcManager()->GatherVBuffer_NpcInfo();
	ConnectionManager()->Send_GatherVBuffer();

	LOG( LOG_INFO_LOW,
		"SYSTEM | cProcessPacket::fnNPCLoginPlayerCn() | PKey(%d)�÷��̾� �α���, ���� �÷��̾� ��(%d)",
		pLoginPlayer->s_dwPKey , PlayerManager()->GetPlayerCnt() );
}

void cProcessPacket::fnNPCMovePlayerCn( cConnection* pConnection,  DWORD dwSize , char* pRecvedMsg )
{
	NPCMovePlayerCn* pMovePlayer = (NPCMovePlayerCn*)pRecvedMsg;
	LOG( LOG_INFO_LOW,
		"SYSTEM | cProcessPacket::fnNPCMovePlayerCn() | PKey(%d)�÷��̾� �̵�",
		pMovePlayer->s_dwPKey );

	cPlayer* pPlayer = PlayerManager()->FindPlayer( pMovePlayer->s_dwPKey );
	if( NULL == pPlayer )
		return;
	pPlayer->SetPos( pMovePlayer->s_dwPos );
}

void cProcessPacket::fnNPCLogoutPlayerCn( cConnection* pConnection,  DWORD dwSize , char* pRecvedMsg )
{
	NPCLogoutPlayerCn* pLogoutPlayer = (NPCLogoutPlayerCn*)pRecvedMsg;
	//�÷��̾� ����
	PlayerManager()->RemovePlayer( pLogoutPlayer->s_dwPKey );
	LOG( LOG_INFO_LOW,
		"SYSTEM | cProcessPacket::fnNPCLogoutPlayerCn() | PKey(%d)�÷��̾� �α׾ƿ�, ���� �÷��̾� ��(%d)",
		pLogoutPlayer->s_dwPKey , PlayerManager()->GetPlayerCnt() );
}

void cProcessPacket::fnNPCDeadPlayerCn( cConnection* pConnection,  DWORD dwSize , char* pRecvedMsg )
{
	NPCDeadPlayerCn* pDeadPlayer = (NPCDeadPlayerCn*)pRecvedMsg;
	LOG( LOG_INFO_LOW,
		"SYSTEM | cProcessPacket::fnNPCLogoutPlayerCn() | PKey(%d)�÷��̾ ����!!",
		pDeadPlayer->s_dwPKey  );
	cPlayer* pPlayer = PlayerManager()->FindPlayer( pDeadPlayer->s_dwPKey );
	if( NULL == pPlayer )
		return;
	pPlayer->SetIsDead( true );

}
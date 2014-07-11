#include "StdAfx.h"
#include ".\cprocesspacket.h"

cProcessPacket::cProcessPacket(void)
{
}

cProcessPacket::~cProcessPacket(void)
{
}

void cProcessPacket::fnLoginPlayerRq( cPlayer* pPlayer,  DWORD dwSize , char* pRecvedMsg )
{
	//�÷��̾� ����
	pPlayer->SetIsConfirm( true );
	//�÷��̾� �߰�
	PlayerManager()->AddPlayer( pPlayer );
	//�÷��̾� �ӽ� ���� ����
	pPlayer->SetTempPlayInfo();
	//////////////////////////////////////////////////////////
	//�÷��̾� ���� ���ӵ� Ŭ���̾�Ʈ�� ����
	pPlayer->Send_PlayerInfo();
	
	///////////////////////////////////////////////////////////
	//���� ���忡 ���ӵǾ��ִ� �÷��̾�� ������ ������ �÷��̾�� ����
	PlayerManager()->Send_WorldPlayerInfosToConnectPlayer( pPlayer );

	///////////////////////////////////////////////////////////
	//���� ���忡 ���ӵǾ��ִ� �÷��̾�鿡�� ������ �÷��̾��� ���� ����
	PlayerManager()->Send_LoginPlayer( pPlayer );

	LOG( LOG_INFO_LOW , 
		"SYSTEM | cIocpGameServer::fnLoginAq() | ����Ű(%d) �α� , ���� �÷��̾� ��(%d)"
		, pPlayer->GetPKey() , PlayerManager()->GetPlayerCnt() );

	///////////////////////////////////////////////////////////
	//Npc������ �÷��̾ �α��� �ߴٴ� ���� �˸���.
	cConnection* pNpcConn = IocpGameServer()->GetNpcServerConn();
	if( NULL == pNpcConn )
		return;
	NPCLoginPlayerCn* pLoginPlayer = (NPCLoginPlayerCn*)pNpcConn->PrepareSendPacket( sizeof( NPCLoginPlayerCn ) );
	if( NULL == pLoginPlayer )
		return;
	pLoginPlayer->s_sType = NPC_LoginPlayer_Cn;
	pLoginPlayer->s_dwPKey = pPlayer->GetPKey();
	pLoginPlayer->s_dwPos = pPlayer->GetPos();
	pNpcConn->SendPost( sizeof( NPCLoginPlayerCn ) );
	
}

void cProcessPacket::fnMovePlayerCn( cPlayer* pPlayer,  DWORD dwSize , char* pRecvedMsg )
{
	MovePlayerCn* pMove = (MovePlayerCn*)pRecvedMsg;
	LOG( LOG_INFO_LOW , 
		"SYSTEM | cIocpGameServer::fnMovePlayerCn() | ����Ű(%d) �̵� : (%d) -> (%d)"
		, pPlayer->GetPKey() , pMove->s_dwCPos , pMove->s_dwTPos );
	pPlayer->SetBPos( pMove->s_dwCPos );
	pPlayer->SetPos( pMove->s_dwTPos );
	
	bool bRet = AreaManager()->TransAreaPlayer( pPlayer );
	AreaManager()->Send_MovePlayerToActiveAreas( pPlayer );
	//���� �÷��̾ �ٸ� �������� �̵��Ͽ��ٸ�
	if( true == bRet )
		AreaManager()->Send_MovePlayerToInActiveAreas( pPlayer );

	///////////////////////////////////////////////////////////
	//Npc������ �÷��̾ �̵� �ߴٴ� ���� �˸���.
	cConnection* pNpcConn = IocpGameServer()->GetNpcServerConn();
	if( NULL == pNpcConn )
		return;
	NPCMovePlayerCn* pMovePlayer = (NPCMovePlayerCn*)pNpcConn->PrepareSendPacket( sizeof( NPCMovePlayerCn ) );
	if( NULL == pMovePlayer )
		return;
	pMovePlayer->s_sType = NPC_MovePlayer_Cn;
	pMovePlayer->s_dwPKey = pPlayer->GetPKey();
	pMovePlayer->s_dwPos = pPlayer->GetPos();
	pNpcConn->SendPost( sizeof( NPCMovePlayerCn ) );

}

void cProcessPacket::fnKeepAliveCn( cPlayer* pPlayer,  DWORD dwSize , char* pRecvedMsg )
{
	LOG( LOG_INFO_LOW , 
		"SYSTEM | cIocpGameServer::fnKeepAliveCn() | ����Ű(%d) KeepAlive"
		, pPlayer->GetPKey() );

	pPlayer->SetKeepAliveTick( IocpGameServer()->GetServerTick() );
}

void cProcessPacket::fnNPCNpcInfoVSn( cPlayer* pPlayer,  DWORD dwSize , char* pRecvedMsg )
{
	cConnection* pConnection = (cConnection*)pPlayer;
	LOG( LOG_INFO_LOW , 
		"SYSTEM | cIocpGameServer::fnNPCNpcInfoSn() | NPC ������ ����" );
	PlayerManager()->Send_RecvBufferFromNpcServer( pRecvedMsg, dwSize );
}

void cProcessPacket::fnNPCUpdateNpcVSn( cPlayer* pPlayer,  DWORD dwSize , char* pRecvedMsg )
{
	cConnection* pConnection = (cConnection*)pPlayer;
	LOG( LOG_INFO_LOW , 
		"SYSTEM | cIocpGameServer::fnNPCUpdateNpcVSn() | NPC ���� ������ ����" );
	PlayerManager()->Send_RecvBufferFromNpcServer( pRecvedMsg, dwSize );

}

void cProcessPacket::fnNPCAttackNpcToPlayerSn( cPlayer* pPlayer,  DWORD dwSize , char* pRecvedMsg )
{
	LOG( LOG_INFO_LOW , 
		"SYSTEM | cIocpGameServer::fnNPCUpdateNpcVSn() | NPC�� �÷��̾� ����" );
	cConnection* pConnection = (cConnection*)pPlayer;
	NPCAttackNpcToPlayerSn* pAttackNpc = (NPCAttackNpcToPlayerSn*)pRecvedMsg;
	cPlayer* pAttackedPlayer = PlayerManager()->FindPlayer( pAttackNpc->s_dwPKey );
	if( NULL == pAttackedPlayer )
		return;
	if( pAttackedPlayer->GetHp() != 0 )
	{
		pAttackedPlayer->SetHp( pAttackedPlayer->GetHp() - 1 );
		if( pAttackedPlayer->GetHp() == 0 )
		{
			cConnection* pNpcConn = IocpGameServer()->GetNpcServerConn();
			if( NULL == pNpcConn )
				return;
			NPCDeadPlayerCn* pDead = (NPCDeadPlayerCn*)pNpcConn->PrepareSendPacket( sizeof( NPCDeadPlayerCn ) );
			if( NULL == pDead )
				return;
			pDead->s_sType = NPC_DeadPlayer_Cn;
			pDead->s_dwPKey = pAttackNpc->s_dwPKey;
			pNpcConn->SendPost( sizeof( NPCDeadPlayerCn ) );
		}
	}

	PlayerManager()->Send_RecvBufferFromNpcServer( pRecvedMsg, dwSize );
}
#pragma once

class cProcessPacket
{
public:
	cProcessPacket(void);
	~cProcessPacket(void);
	//Ŭ���̾�Ʈ���� ���� ��Ŷ
	static void fnLoginPlayerRq( cPlayer* pPlayer,  DWORD dwSize , char* pRecvedMsg );
	static void fnMovePlayerCn( cPlayer* pPlayer,  DWORD dwSize , char* pRecvedMsg );
	static void fnKeepAliveCn( cPlayer* pPlayer,  DWORD dwSize , char* pRecvedMsg );

	//NPC�������� ���� ��Ŷ
	static void fnNPCNpcInfoVSn( cPlayer* pPlayer,  DWORD dwSize , char* pRecvedMsg );
	static void fnNPCUpdateNpcVSn( cPlayer* pPlayer,  DWORD dwSize , char* pRecvedMsg );
	static void fnNPCAttackNpcToPlayerSn( cPlayer* pPlayer,  DWORD dwSize , char* pRecvedMsg );

};

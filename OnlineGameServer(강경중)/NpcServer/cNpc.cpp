#include "StdAfx.h"
#include ".\cnpc.h"

cNpc::cNpc(void)
{
	m_pFsmClass = new cFsm::cFsmClass();
	Init();	
}

cNpc::~cNpc(void)
{
	if( m_pFsmClass )
		delete m_pFsmClass;
}

void cNpc::Init()
{
	m_eNpcType = NONE_NPC;
	m_dwPos = 0;
	m_dwNpcKey = 0;
	m_dwTagetPlayerPKey = 0;
	ZeroMemory( m_szName , MAX_NPCNAME );
}
void cNpc::OnProcess()
{
	switch( m_pFsmClass->GetCurState() )
	{
	case NPC_NORMAL:
		{
			DoWander();
		}
		break;
	case NPC_ANGRY:
		{
			DoAttack();
		}
		break;
	default:
		{
			LOG( LOG_ERROR_NORMAL ,
				"SYSTEM | cDetectNpc::OnProcess() | ����(%d)�� �������� ���� ��Ȳ�̴�.",
				m_pFsmClass->GetCurState() );
		}
		break;
	}
}
void cNpc::SetTempNpcInfo()
{
	m_dwNpcKey = NpcManager()->GenerateNpcKey();
	sprintf( m_szName , "Npc%d" , ++m_dwNpcKey );
	m_dwPos = rand() % 3600 + 1;
}

void cNpc::DoWander()
{
	switch( m_pFsmClass->GetCurState() )
	{
	case NPC_NORMAL:
		{
			int nPosX = m_dwPos % COL_LINE;
			int nPosY = m_dwPos / COL_LINE;
			int nTemp = 0;
			if( rand()%2 )
				nTemp =  -1;
			else
				nTemp =  1;

			//�������� ������ ������
			int nDir = rand() % 3;
			switch( nDir )
			{
				//����
			case 0:
				{
					nPosX += nTemp;
					//�ʵ带 ������
					if( nPosX <= 0 )
						nPosX += (nTemp * nTemp);
					else if( nPosX >= ROW_LINE )
						nPosX -= (nTemp * nTemp);
				}
				break;
				//����
			case 1:
				{
					nPosY += nTemp;
					if( nPosY <= 0 )
						nPosY += (nTemp * nTemp);
					else if( nPosY >= ROW_LINE )
						nPosY -= (nTemp * nTemp);
				}
				break;
				//�밢��
			case 2:
				{
					nPosX += nTemp;
					nPosY += nTemp;
					if( nPosX <= 0  )
						nPosX += (nTemp * nTemp);
					else if( nPosX >= ROW_LINE )
						nPosX -= (nTemp * nTemp);
					if( nPosY <= 0 )
						nPosY += (nTemp * nTemp);
					else if( nPosY >= ROW_LINE )
						nPosY -= (nTemp * nTemp);
				}
				break;
			}
			m_dwPos = nPosY * ROW_LINE + nPosX;
		}
		break;
	}
}

void cNpc::DoAttack()
{
	cPlayer* pPlayer = PlayerManager()->FindPlayer( m_dwTagetPlayerPKey );
	//Ž���� ����� �����ų� ������
	if( NULL == pPlayer || pPlayer->GetIsDead() )
	{
		m_dwTagetPlayerPKey = 0;
		m_pFsmClass->TranslateState( EVENT_PLAYER_RUNAWAY );
		return;
	}
	//NPC Ž�� ���� ������ �÷��̾ ���������� üũ
	DWORD dwPlayerPos = pPlayer->GetPos();
	int nDiff1 = ( dwPlayerPos % COL_LINE ) - ( m_dwPos % COL_LINE );
	int nDiff2 = ( dwPlayerPos / COL_LINE ) - ( m_dwPos / COL_LINE );
	if( abs( nDiff1 ) > 1 || abs( nDiff2 ) > 1 )
	{
		m_dwTagetPlayerPKey = 0;
		m_pFsmClass->TranslateState( EVENT_PLAYER_RUNAWAY );
		return;
	}
	//NPC�� �÷��̾ �����Ͽ��� ��Ŷ�� ���Ӽ����� �˸�
	ConnectionManager()->Send_AttackNpcToPlayerSn( m_dwNpcKey , m_dwTagetPlayerPKey );
}


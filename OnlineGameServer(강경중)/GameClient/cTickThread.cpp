#include "StdAfx.h"
#include ".\ctickthread.h"


cTickThread::cTickThread(void)
{
	
}

cTickThread::~cTickThread(void)
{
}

void cTickThread::OnProcess()
{
	srand( GetTickCount() );
	cPlayer* pMyPlayer = PlayerManager()->GetMyPlayer();
	if( NULL == pMyPlayer )
		return;
	int nPosX = pMyPlayer->GetPos() % COL_LINE;
	int nPosY = pMyPlayer->GetPos() / COL_LINE;
	int nTemp = 0;
	if( rand()%2 )
		nTemp = ( rand() %5 ) * -1;
	else
		nTemp = ( rand() %5 ) * 1;
			
	//�������� ������ ������
	int nDir = rand() % 3;
	switch( nDir )
	{
		//����
	case 0:
		{
			nPosX += nTemp;
		}
		break;
		//����
	case 1:
		{
			nPosY += nTemp;
		}
		break;
		//�밢��
	case 2:
		{
			nPosX += nTemp;
			nPosY += nTemp;
		}
		break;
	}
	
	pMyPlayer->SetTPos( (nPosY * COL_LINE) +nPosX );
	///////////////////////////////////////////
	//������  ��Ŷ�� ������ ������.
	MovePlayerCn Move;
	Move.s_nLength = sizeof( MovePlayerCn );
	Move.s_sType = MovePlayer_Cn;
	Move.s_dwCPos = pMyPlayer->GetPos();
	Move.s_dwTPos = pMyPlayer->GetTPos();
	AsyncSocket()->SendMsg( (char*)&Move , sizeof( MovePlayerCn ));
	
}
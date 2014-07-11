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
	//TEMPPLAYER_UPDATE_TICK ���� TempPlayer�� ��ǥ ����
	if( ( m_dwTickCount % TEMPPLAYER_UPDATE_TICK ) == 0 )
		PlayerManager()->UpdateTempPlayerPos();
	//KEEPALIVE_TICK���� ���ӵǾ� �ִ� �÷��̾�κ��� �޼����� ���� ������ 
	//���� �÷��̾��� �ν��ϰ� ������ �����Ѵ�.
	if( ( m_dwTickCount % KEEPALIVE_TICK ) == 0 )
		PlayerManager()->CheckKeepAliveTick( m_dwTickCount );

}
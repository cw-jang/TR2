#include "StdAfx.h"
#include ".\ciocpchatserver.h"

IMPLEMENT_SINGLETON( cIocpChatServer )

cIocpChatServer::cIocpChatServer(void)
{
	if( NULL == m_pIocpServer )
		m_pIocpServer = this;
}

cIocpChatServer::~cIocpChatServer(void)
{
}

//client�� ���� ������ �Ǿ��� �� ȣ��Ǵ� �Լ�
bool cIocpChatServer::OnAccept( cConnection *lpConnection )
{
	ConnectionManager()->AddConnection( lpConnection );
	LOG( LOG_INFO_LOW , 
		"SYSTEM | cIocpChatServer::OnAccept() | IP[%s] Socket[%d] ���� UserCnt[%d]",
		lpConnection->GetConnectionIp(), lpConnection->GetSocket() ,
		ConnectionManager()->GetConnectionCnt() );

	return true;
}

//client���� packet�� �������� �� ���� �� �ְ� ó���Ǿ����� ��Ŷó��
bool cIocpChatServer::OnRecv(cConnection* lpConnection,  DWORD dwSize , char* pRecvedMsg)
{
	unsigned short usType;
	CopyMemory( &usType , pRecvedMsg + 4 , PACKET_TYPE_LENGTH );
	switch( usType )
	{
	case PACKET_CHAT:
		{
			Packet_Chat* pChat = (Packet_Chat*)pRecvedMsg;
			ConnectionManager()->BroadCast_Chat( pChat->s_szIP , pChat->s_szChatMsg );
			LOG( LOG_INFO_LOW , "[�޼���] IP : %s , Msg : %s" , pChat->s_szIP , pChat->s_szChatMsg );
		}
		break;
	default:
		LOG( LOG_ERROR_NORMAL , "SYSTEM | cIocpChatServer::OnRecv() | ���ǵ��� ���� ��Ŷ(%d)",
			usType );
		break;
	}


	return true;
}

//client���� packet�� �������� �� ���� �� ���� ��ٷ� ó�� �Ǵ� ��Ŷó��
bool cIocpChatServer::OnRecvImmediately(cConnection* lpConnection,  DWORD dwSize , char* pRecvedMsg)
{
	/////////////////////////////////////////////////////////////////
	//��Ŷ�� ó���Ǹ� return true; ó�� ���� �ʾҴٸ� return false;
	return false;
}

//client�� ������ ����Ǿ��� �� ȣ��Ǵ� �Լ�
void cIocpChatServer::OnClose(cConnection* lpConnection)
{
	ConnectionManager()->RemoveConnection( lpConnection );
	LOG( LOG_INFO_LOW , 
		"SYSTEM | cIocpChatServer::OnClose() | IP[%s] Socket[%d] ���� UserCnt[%d]",
		lpConnection->GetConnectionIp(), lpConnection->GetSocket() ,
		ConnectionManager()->GetConnectionCnt() );

}
bool cIocpChatServer::OnSystemMsg( cConnection* lpConnection , DWORD dwMsgType , LPARAM lParam )
{
	return true;
}
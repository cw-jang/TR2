#pragma once

class cIocpChatServer : public cIocpServer , cSingleton
{
	DECLEAR_SINGLETON( cIocpChatServer );
public:
	cIocpChatServer(void);
	~cIocpChatServer(void);

	//client�� ���� ������ �Ǿ��� �� ȣ��Ǵ� �Լ�
	virtual	bool	OnAccept( cConnection *lpConnection );
	//client���� packet�� �������� �� ���� �� �ְ� ó���Ǿ����� ��Ŷó��
	virtual	bool	OnRecv(cConnection* lpConnection,  DWORD dwSize , char* pRecvedMsg);
	//client���� packet�� �������� �� ���� �� ���� ��ٷ� ó�� �Ǵ� ��Ŷó��
	virtual	bool	OnRecvImmediately(cConnection* lpConnection,  DWORD dwSize , char* pRecvedMsg);
	//client�� ������ ����Ǿ��� �� ȣ��Ǵ� �Լ�
	virtual	void	OnClose(cConnection* lpConnection);
	
	virtual bool	OnSystemMsg( cConnection* lpConnection , DWORD dwMsgType , LPARAM lParam );


};
CREATE_FUNCTION( cIocpChatServer , IocpChatServer )
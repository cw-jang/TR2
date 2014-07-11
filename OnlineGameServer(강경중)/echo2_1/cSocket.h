#pragma once
#include <winsock2.h>

class cSocket
{
public:
	cSocket(void);
	~cSocket(void);
	
	//------���� Ŭ���̾�Ʈ �����Լ�-------//
	//������ �ʱ�ȭ�ϴ� �Լ�
	bool InitSocket();
	//������ ������ ���� ��Ų��.
	void CloseSocket( SOCKET socketClose , bool bIsForce = false );
	
	//------������ �Լ�-------//
	//������ �ּ������� ���ϰ� �����Ű�� ���� ��û�� �ޱ� ���� �� ������ ����ϴ� �Լ�
	bool BindandListen( int nBindPort );
	//���� ��û�� �����ϰ� �޼����� �޾Ƽ� ó���ϴ� �Լ�
	bool StartServer();

	//------Ŭ���̾�Ʈ�� �Լ�-------//
	//IP,Port�� �ش��ϴ� ������ ������ �Ѵ�.
	bool Connect( char* pszIP , int nPort );

private:
	//Ŭ���̾�Ʈ������ ������ �����ϱ� ���� ����
	//���������� Ŭ���̾�Ʈ�� ���ӿ�û�� ��ٸ��� ����
	SOCKET		m_socket;

	//�������� Ŭ���̾�Ʈ�� ���� ��û�� ������ �Ŀ� Ŭ���̾�Ʈ�� ����Ǿ� ���� ����
	SOCKET		m_socketConnect;

	//���� ����
	char		m_szSocketBuf[ 1024 ];
};

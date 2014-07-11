#pragma once

class cPlayer
{
public:
	cPlayer(void);
	~cPlayer(void);

	inline DWORD GetPKey() { return m_dwPKey; }
	inline void SetPKey( DWORD dwPKey ) { m_dwPKey = dwPKey; }
	inline char* GetId() { return m_szId; }
	inline void	 SetId( char* szId ) { strncpy( m_szId , szId, MAX_ID_LENGTH ); }
	inline char* GetNickName() { return m_szNickName; }
	inline void	 SetNickName( char* szNickName ) { strncpy( m_szNickName, szNickName, MAX_NICKNAME_LENGTH ); }
	inline char* GetName() { return m_szName; }
	inline void	 SetName( char* szName ) { strncpy( m_szName , szName, MAX_NAME_LENGTH ); }
	inline DWORD GetPos() { return m_dwPos; }
	inline void	SetPos( DWORD dwPos ) { m_dwPos = dwPos; }
	inline DWORD GetTPos() { return m_dwTPos; }
	inline void	SetTPos( DWORD dwTPos ) { m_dwTPos = dwTPos; }
	inline BYTE GetLevel() { return m_byLevel; }
	inline void SetLevel( BYTE byLevel ) { m_byLevel = byLevel; }
	inline BYTE GetStr() { return m_byStr; }
	inline void SetStr( BYTE byStr ) { m_byStr = byStr; }
	inline BYTE GetDur() { return m_byDur; }
	inline void SetDur( BYTE byDur ) { m_byDur = byDur; }
	inline DWORD GetHp() { return m_dwHp; }
	inline void SetHp( DWORD dwHp ) { m_dwHp = dwHp; }
	inline DWORD GetExp() { return m_dwExp; }
	inline void SetExp( DWORD dwExp ) { m_dwExp = dwExp; }
	inline bool GetVisible() { return m_bVisible; }
	inline void SetVisible( bool bVisible ) { m_bVisible = bVisible; }
	

	//������ �ʱ�ȭ ��Ų��.
	void Init();
	//�÷��̾� ���� ��Ŷ�� �޾� ������ �����Ѵ�.
	void SetPlayerInfoFromPacket( char* pPlayerInfo );
	
private:
	//ĳ������ �⺻ �Ӽ�
	DWORD	m_dwPKey;	//�÷��̾� ���� ��ȣ
	char	m_szId[ MAX_ID_LENGTH ];  //���̵�
	char	m_szNickName[ MAX_NICKNAME_LENGTH ];  //����
	char	m_szName[ MAX_NAME_LENGTH ];  //�̸�

	DWORD	m_dwPos;	//�÷��̾� ��ġ          
	BYTE	m_byLevel;	//����
	BYTE	m_byStr;	//���ݷ�
	BYTE	m_byDur;	//����
	DWORD	m_dwHp;		//�����
	DWORD	m_dwExp;	//����ġ
	bool	m_bVisible;	//���

	DWORD	m_dwTPos;	//������ ��ġ

};

#pragma once
class NETLIB_API cVBuffer :  public cSingleton
{
	DECLEAR_SINGLETON( cVBuffer );
public:
	cVBuffer( int nMaxBufSize = 1024 * 50 );
	~cVBuffer(void);
	
	void GetChar( char& cCh );
	void GetShort( short& sNum );
	void GetInteger( int& nNum );
	void GetString( char* pszBuffer );
	void GetStream( char* pszBuffer, short sLen);	
	void SetInteger( int nI );
	void SetShort( short sShort );
	void SetChar( char cCh);
	void SetString( char* pszBuffer );
	void SetStream( char* pszBuffer , short sLen);

	void SetBuffer( char* pVBuffer );

	inline int	GetMaxBufSize() { return m_nMaxBufSize; }
	inline int	GetCurBufSize() { return m_nCurBufSize; }
	inline char*	GetCurMark() { return m_pCurMark; }
	inline char*    GetBeginMark() { return m_pszVBuffer; }

	bool CopyBuffer( char* pDestBuffer );
	void Init();			
	 
private:

	char* m_pszVBuffer;		//���� ����
	char* m_pCurMark;		//���� ���� ��ġ

	int	m_nMaxBufSize;		//�ִ� ���� ������
	int m_nCurBufSize;		//���� ���� ���� ������

	cVBuffer(const cVBuffer &rhs);
    cVBuffer &operator=(const cVBuffer &rhs);
};

CREATE_FUNCTION( cVBuffer , VBuffer );
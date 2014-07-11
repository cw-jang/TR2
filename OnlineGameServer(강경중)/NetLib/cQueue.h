#pragma once

template < class TYPE >
class cQueue : public cMonitor
{
public:
	cQueue( int nMaxSize = MAX_QUEUESIZE );
	~cQueue(void);

	bool	PushQueue( TYPE typeQueueItem );		//queue�� ����Ÿ�� �ִ´�.
	void	PopQueue();								//queue���� ����Ÿ�� �����.

	bool	IsEmptyQueue();							//queue�� ������� �˷��ش�.
	TYPE	GetFrontQueue();						//queue���� ����Ÿ�� ���´�.
	int		GetQueueSize();							//queue�� ���� ����ִ� item�� ������ �˷��ش�.
	int		GetQueueMaxSize() { return m_nQueueMaxSize; }  //�ִ� queue����� �˷��ش�
	void	SetQueueMaxSize( int nMaxSize ) { m_nQueueMaxSize = nMaxSize; }   //�ִ� queue size�� ���� �ش�
	void	ClearQueue();						//queue�� �ִ� ��� item�� �����Ѵ�.

private:

	TYPE*			m_arrQueue;
	int				m_nQueueMaxSize;				//ť������ ������ ����
	cMonitor		m_csQueue;
	
	int				m_nCurSize;
	int				m_nEndMark;
	int				m_nBeginMark;
	
};
template <class TYPE>
cQueue< TYPE >::cQueue< TYPE >( int nMaxSize )
{
	m_arrQueue = new TYPE[ nMaxSize ];
	m_nQueueMaxSize = nMaxSize;
	ClearQueue();
	
}

template <class TYPE>
cQueue< TYPE >::~cQueue< TYPE >(void)
{
	delete [] m_arrQueue;
}

template <class TYPE>
bool cQueue< TYPE >::PushQueue( TYPE typeQueueItem )
{
	cMonitor::Owner lock( m_csQueue );
	{
		
		//���س��� ����� �Ѿ��ٸ� ���̻� ť�� ���� �ʴ´�.
		if( m_nCurSize >= m_nQueueMaxSize )
			return false;
		
		m_nCurSize++;
		if( m_nEndMark == m_nQueueMaxSize )
			m_nEndMark = 0;
		m_arrQueue[ m_nEndMark++ ] = typeQueueItem;
		
	}
	return true;


}

template <class TYPE>
TYPE cQueue< TYPE >::GetFrontQueue()	
{
	TYPE typeQueueItem;
	cMonitor::Owner lock( m_csQueue );
	{
		if( m_nCurSize <= 0 )
			return NULL;
		if( m_nBeginMark == m_nQueueMaxSize )
			m_nBeginMark = 0;
		typeQueueItem = m_arrQueue[ m_nBeginMark++ ];
		
	}
	return typeQueueItem;

}

template <class TYPE>
void cQueue< TYPE >::PopQueue()
{
	cMonitor::Owner lock( m_csQueue );
	{
		m_nCurSize--;
	}
}

template <class TYPE>
bool cQueue< TYPE >::IsEmptyQueue()
{
	bool bFlag = false;
	cMonitor::Owner lock( m_csQueue );
	{
		bFlag = ( m_nCurSize > 0 ) ? true : false;
	}

	return flag;
}

template <class TYPE>
int cQueue< TYPE >::GetQueueSize()
{
	int nSize;
	cMonitor::Owner lock( m_csQueue );
	{
		nSize = m_nCurSize;
	}
	
	return nSize;
}

template <class TYPE>
void cQueue< TYPE >::ClearQueue()
{
	cMonitor::Owner lock( m_csQueue );
	{
		m_nCurSize = 0;
		m_nEndMark = 0;
		m_nBeginMark = 0;
	}
}

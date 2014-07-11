#include "precompile.h"
#include "cringbuffer.h"
cRingBuffer::cRingBuffer(void)
{
	m_pRingBuffer = NULL;
	m_pBeginMark = NULL;
	m_pEndMark = NULL;
	m_pCurrentMark = NULL;
	m_pGettedBufferMark = NULL;
	m_pLastMoveMark = NULL;
	m_nUsedBufferSize = 0;
	m_uiAllUserBufSize = 0;
}

cRingBuffer::~cRingBuffer(void)
{
	if( NULL != m_pBeginMark )
		delete [] m_pBeginMark;

}

bool cRingBuffer::Initialize()
{
	cMonitor::Owner lock( m_csRingBuffer );
	{
		m_nUsedBufferSize = 0;
		m_pCurrentMark = m_pBeginMark;
		m_pGettedBufferMark = m_pBeginMark;
		m_pLastMoveMark = m_pEndMark;
		m_uiAllUserBufSize = 0;

	}
	return true;
}

bool cRingBuffer::Create( int nBufferSize )
{
	if( NULL != m_pBeginMark )
		delete [] m_pBeginMark;

	m_pBeginMark = new char[ nBufferSize ];
	
	if( NULL == m_pBeginMark )
		return false;

	m_pEndMark = m_pBeginMark + nBufferSize - 1;
	m_nBufferSize = nBufferSize;

	Initialize();
	return true;
}

char* cRingBuffer::ForwardMark( int nForwardLength )
{
	char* pPreCurrentMark = NULL;
	cMonitor::Owner lock( m_csRingBuffer );
	{
		//������ �����÷� üũ
		if( m_nUsedBufferSize + nForwardLength > m_nBufferSize )
			return NULL;
		if( ( m_pEndMark - m_pCurrentMark ) >= nForwardLength )
		{
			pPreCurrentMark = m_pCurrentMark;
			m_pCurrentMark += nForwardLength;
		}
		else
		{
			//��ȯ �Ǳ� �� ������ ��ǥ�� ����
			m_pLastMoveMark = m_pCurrentMark;
			m_pCurrentMark = m_pBeginMark + nForwardLength;
			pPreCurrentMark = m_pBeginMark;
		}
		
	}
	return pPreCurrentMark;
}

char* cRingBuffer::ForwardMark( int nForwardLength 
							   , int nNextLength 
							   , DWORD dwRemainLength )
{
	cMonitor::Owner lock( m_csRingBuffer );
	{	
		//������ �����÷� üũ
		if( m_nUsedBufferSize + nForwardLength + nNextLength > m_nBufferSize )
			return NULL;
		
		if( ( m_pEndMark - m_pCurrentMark ) > ( nNextLength + nForwardLength ) )
			m_pCurrentMark += nForwardLength;

		//���� ������ ���̺��� ������ ���� �޼����� ũ�� ���� �޼����� ó������ 
		//�����Ѵ��� ��ȯ �ȴ�.
		else
		{
			//��ȯ �Ǳ� �� ������ ��ǥ�� ����
			m_pLastMoveMark = m_pCurrentMark;
			CopyMemory( m_pBeginMark , 
				m_pCurrentMark - ( dwRemainLength - nForwardLength ) , 
				dwRemainLength );
			m_pCurrentMark = m_pBeginMark + dwRemainLength;
		}
	}
	return m_pCurrentMark;
}

void cRingBuffer::BackwardMark( int nBackwardLength )
{
	cMonitor::Owner lock( m_csRingBuffer );
	{
		//nBackwardLength�縸ŭ ���� ���������͸� �ڷ� ������.
		m_nUsedBufferSize -= nBackwardLength;
		m_pCurrentMark -= nBackwardLength;
	}
}

void cRingBuffer::ReleaseBuffer( int nReleaseSize )
{
	cMonitor::Owner lock( m_csRingBuffer );
	{
		m_nUsedBufferSize -= nReleaseSize;
	}
}

//���� ���۾� ����(�̰��� �ϴ� ������ SendPost()�Լ��� ��Ƽ �����忡�� ���ư��⶧����
//PrepareSendPacket()����(ForwardMark()����) ���� ���� �÷������� PrepareSendPacket�Ѵ����� �����͸�
//ä�� �ֱ����� �ٷ� �ٸ� �����忡�� SendPost()�� �Ҹ��ٸ� ������ ������ �����Ͱ� �� �� �ִ�.
//�װ� �����ϱ� ���� �����͸� �� ä�� ���¿����� ���� ���� ����� ������ �� �־���Ѵ�.
//���Լ��� sendpost�Լ����� �Ҹ��� �ȴ�.
void cRingBuffer::SetUsedBufferSize( int nUsedBufferSize )
{
	cMonitor::Owner lock( m_csRingBuffer );
	{
		m_nUsedBufferSize += nUsedBufferSize;
		m_uiAllUserBufSize += nUsedBufferSize;
	}
}

char* cRingBuffer::GetBuffer( int nReadSize , int* pReadSize )
{
	char* pRet = NULL;
	cMonitor::Owner lock( m_csRingBuffer );
	{
		//���������� �� �о��ٸ� �� �о���� ������ �����ʹ� �Ǿ����� �ű��.
		if( m_pLastMoveMark == m_pGettedBufferMark )
		{
			m_pGettedBufferMark = m_pBeginMark;
			m_pLastMoveMark = m_pEndMark;
		}

		//���� ���ۿ� �ִ� size�� �о���� size���� ũ�ٸ�
		if( m_nUsedBufferSize > nReadSize )
		{
			//�������� ������ �Ǵ�.
			if( ( m_pLastMoveMark - m_pGettedBufferMark ) >= nReadSize )
			{
				*pReadSize = nReadSize;
				pRet = m_pGettedBufferMark;
				m_pGettedBufferMark += nReadSize;
			}
			else
			{
				*pReadSize = (int)( m_pLastMoveMark - m_pGettedBufferMark );
				pRet = m_pGettedBufferMark;
				m_pGettedBufferMark += *pReadSize;
			}
		}
		else if( m_nUsedBufferSize > 0 )
		{
			//�������� ������ �Ǵ�.
			if( ( m_pLastMoveMark - m_pGettedBufferMark ) >= m_nUsedBufferSize )
			{
				*pReadSize = m_nUsedBufferSize;
				pRet = m_pGettedBufferMark;
				m_pGettedBufferMark += m_nUsedBufferSize;
			}
			else
			{
				*pReadSize = (int)( m_pLastMoveMark - m_pGettedBufferMark );
				pRet = m_pGettedBufferMark;
				m_pGettedBufferMark += *pReadSize;
			}
		}
	}
	return pRet;
}

#include "StdAfx.h"
#include "cfsm.h"

cFsm::cFsmState::cFsmState( eState enumState ): m_eState(enumState)
{
}

cFsm::cFsmState::~cFsmState()
{ 
	m_mapState.clear();
}

void cFsm::cFsmState::InsertState( eEvent inputEvent, eState inputState )
{
	STATE_IT state_it = m_mapState.find( inputEvent );
	if( state_it != m_mapState.end() )
	{
		LOG( LOG_ERROR_NORMAL,
			"SYSTEM | cFsm::cFsmState::InsertState() | �̺�Ʈ(%d)�� m_mapState�� �̹� �����ϰ� �ִ�.",
			inputEvent );
		return;

	}
	m_mapState.insert( STATE_PAIR( inputEvent , inputState ) );
}

void cFsm::cFsmState::RemoveState( eEvent inputEvent )
{
	STATE_IT state_it = m_mapState.find( inputEvent );
	if( state_it == m_mapState.end() )
	{
		LOG( LOG_ERROR_NORMAL,
			"SYSTEM | cFsm::cFsmState::RemoveState() | �̺�Ʈ(%d)�� m_mapState�� �������� �ʴ´�.",
			inputEvent );
		return;

	}
	m_mapState.erase( inputEvent );
}

eState cFsm::cFsmState::GetNextState( eEvent inputEvent )
{
	STATE_IT state_it = m_mapState.find( inputEvent );
	if( state_it == m_mapState.end() )
	{
		LOG( LOG_ERROR_NORMAL,
			"SYSTEM | cFsm::cFsmState::GetNextState() | �̺�Ʈ(%d)�� m_mapState�� �������� �ʴ´�.",
			inputEvent );
		return NPC_NONE;

	}
	return state_it->second;
}

DWORD cFsm::cFsmState::GetStateCount()
{
	return (DWORD)m_mapState.size();
}


/////////////////////////////////////////////////////////////////////////////
// cFSM::cFsmClass class
//
cFsm::cFsmClass::cFsmClass()
	: m_pCurFsmState(0), m_eCurEvent( EVENT_NONE )
{

}

cFsm::cFsmClass::~cFsmClass()
{
	FSMSTATE_IT fsmstate_it;
	for( fsmstate_it = m_mapFsmState.begin() ; fsmstate_it != m_mapFsmState.end() ; fsmstate_it++ )
	{
		cFsmState* pFsmState = fsmstate_it->second;
		delete pFsmState;
	}
	m_mapFsmState.clear();
}


void cFsm::cFsmClass::InsertFsmState( eState inputState, eEvent inputEvent, eState inputNextState )
{
	cFsmState* pFsmState = NULL;
	FSMSTATE_IT fsmstate_it = m_mapFsmState.find( inputState );
	//������ ���°� �̹� ���� �Ѵٸ� �� ���� ��ü�� �����ͼ� �̺�Ʈ�� ���� ���¸� �߰�
	if( fsmstate_it != m_mapFsmState.end() )
	{
		pFsmState = (cFsmState*)fsmstate_it->second;
		pFsmState->InsertState( inputEvent , inputNextState );
		return;

	}
	//������ ���°� ���ٸ� ���� ��ü�� �����ϰ� �̺�Ʈ�� ���� ���¸� �߰�
	else
	{
		pFsmState = new cFsmState( inputState );
		m_mapFsmState.insert( FSMSTATE_PAIR( inputState , pFsmState ) );
	}
	pFsmState->InsertState( inputEvent , inputNextState );
}

void cFsm::cFsmClass::RemoveFsmState( eState inputState, eEvent inputEvent )
{
	FSMSTATE_IT fsmstate_it = m_mapFsmState.find( inputState );
	//�ش� ���°�ü�� �������� �ʴ´ٸ�
	if( fsmstate_it == m_mapFsmState.end() )
	{
		LOG( LOG_ERROR_NORMAL ,
			"SYSTEM | cFsm::cFsmClass::RemoveFsmState | ����(%d) ��ü�� m_mapFsmState�� �������� �ʴ´�.",
			inputState );
		return;
	}
	cFsmState* pFsmState = fsmstate_it->second;
	//�̺�Ʈ�� �ش��ϴ� ���� ������ �����
	pFsmState->RemoveState( inputEvent );
	//���� ��ü�� ���� ������ ���ٸ� ��ü�� �����.
	if( pFsmState->GetStateCount() == 0 )
	{
		delete pFsmState;
		m_mapFsmState.erase( fsmstate_it );
	}
}

void cFsm::cFsmClass::SetCurFsmState(eState inputState)
{
	FSMSTATE_IT fsmstate_it = m_mapFsmState.find( inputState );
	//�ش� ���°�ü�� �������� �ʴ´ٸ�
	if( fsmstate_it == m_mapFsmState.end() )
	{
		LOG( LOG_ERROR_NORMAL ,
			"SYSTEM | cFsm::cFsmClass::SetCurFsmState | ����(%d) ��ü�� m_mapFsmState�� �������� �ʴ´�.",
			inputState );
		return;
	}
	m_pCurFsmState = fsmstate_it->second;
}

eState cFsm::cFsmClass::GetCurState()
{
	//���� ���°� �����Ǿ� �ִٸ�
	if( m_pCurFsmState )
		return m_pCurFsmState->GetState();
	return NPC_NONE;
}

eState cFsm::cFsmClass::GetNextState(eEvent inputEvent)
{
	//���� ���°� �����Ǿ� �ִٸ�
	if( m_pCurFsmState )
	{
		m_pCurFsmState->GetNextState( inputEvent );
		return m_pCurFsmState->GetState();
	}
	return NPC_NONE;
}

void cFsm::cFsmClass::TranslateState(eEvent inputEvent)
{
	if( m_pCurFsmState )
	{
		eState enumCurState = m_pCurFsmState->GetState();
		eState enumNextState = m_pCurFsmState->GetNextState( inputEvent );

		FSMSTATE_IT fsmstate_it = m_mapFsmState.find( enumNextState );
		//�ش� ���°�ü�� �������� �ʴ´ٸ�
		if( fsmstate_it == m_mapFsmState.end() )
		{
			LOG( LOG_ERROR_NORMAL ,
				"SYSTEM | cFsm::cFsmClass::TranslateState | ����(%d) ��ü�� m_mapFsmState�� �������� �ʴ´�.",
				enumNextState );
			return;
		}
		m_pCurFsmState = fsmstate_it->second;
		m_eCurEvent = inputEvent;
		LOG( LOG_INFO_LOW,
			"SYSTEM | cFsm::cFsmClass::TranslateState() | NPC���� ���� %s -> %s"
			, strtableState[ enumCurState ] , strtableState[ enumNextState ] );
	}
}
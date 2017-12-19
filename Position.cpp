#include <iostream>

#include "Position.h"
#include "Unit.h"


using namespace std;

CPosition::CPosition()
{
	m_bIndex	= false;
	m_pUnitData	= NULL;
}

CPosition::~CPosition()
{  //종류시 메모리 미해제시 자동으로 메모리 해제
	if ( m_pUnitData != NULL )
	{
		delete m_pUnitData;
		m_pUnitData = NULL;
	}
}

void CPosition::SetIndex()
{
	m_bIndex = true;
}

void CPosition::DelIndex()
{
	m_bIndex = false;
}

bool CPosition::GetIndex()
{
	return m_bIndex;
}

void CPosition::SetUnit(CUnit *_unitData)
{
	m_pUnitData = _unitData;
}

void CPosition::DelUnit()
{
	m_pUnitData = NULL;
}

CUnit* CPosition::GetUnit()
{
	return m_pUnitData;
}


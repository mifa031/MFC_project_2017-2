#include <iostream>

#include "Unit.h"

using namespace std;

CUnit::CUnit(int _x, int _y, char _unitType, int _unitColor)
{
	m_nX				= _x;
	m_nY				= _y;
	m_cUnitType			= _unitType;
	m_bUnitColor		= _unitColor;
	m_bFirstMove		= true;
	m_bEnpassantOK		= false;
	m_bCastling			= false;
}

void CUnit::SetXY(int _x, int _y)
{
	m_nX = _x;
	m_nY = _y;
}
	
char CUnit::GetUnitType()
{
	return m_cUnitType;
}

int CUnit::GetColor()
{
	return m_bUnitColor;
}

void CUnit::DelFirst()
{
	m_bFirstMove = false;
}

bool CUnit::GetFirst()
{
	return m_bFirstMove;
}

void CUnit::SetEnp()
{
	m_bEnpassantOK = true;
}

void CUnit::DelEnp()
{
	m_bEnpassantOK = false;
}

bool CUnit::GetEnp()
{
	return m_bEnpassantOK;
}

void CUnit::SetCast()
{
	m_bCastling = true;
}

void CUnit::DelCast()
{
	m_bCastling = false;
}

bool CUnit::GetCast()
{
	return m_bCastling;
}

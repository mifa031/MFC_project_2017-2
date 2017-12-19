#ifndef __UNIT_H__
#define __UNIT_H__

#include "Position.h"
#include "Unit.h"
#include "stdafx.h"
#include "DefineWords.h"


class CUnit
{
public :
	CUnit(int _x, int _y, char _unitType, int _unitColor);

	int GetColor();
	void SetXY(int _x, int _y);	
	char GetUnitType();
	void DelFirst();
	bool GetFirst();
	void SetEnp();
	void DelEnp();
	bool GetEnp();
	void SetCast();
	void DelCast();
	bool GetCast();
	virtual bool Move(int i, int j, CPosition** board) = 0;

protected :
	int m_nX;//좌표값
	int m_nY;//좌표값

private :
	char m_cUnitType;//유닛의 종류
	int m_bUnitColor;//유닛의 팀
	bool m_bFirstMove;//처음 이동여부
	bool m_bEnpassantOK;//앙파상 여부
	bool m_bCastling;//캐슬링 여부
	
};

#endif

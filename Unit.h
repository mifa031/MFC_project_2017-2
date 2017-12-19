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
	int m_nX;//��ǥ��
	int m_nY;//��ǥ��

private :
	char m_cUnitType;//������ ����
	int m_bUnitColor;//������ ��
	bool m_bFirstMove;//ó�� �̵�����
	bool m_bEnpassantOK;//���Ļ� ����
	bool m_bCastling;//ĳ���� ����
	
};

#endif

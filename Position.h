#ifndef __POSITION_H__
#define __POSITION_H__

#include "stdafx.h"


class CUnit;


class CPosition
{
public :
	CPosition();
	~CPosition();

	void SetIndex();
	void DelIndex();
	bool GetIndex();
	void SetUnit(CUnit *_unitData);
	void DelUnit();
	CUnit* GetUnit();

private :
	bool m_bIndex;//�̵��� ���� ���� ���� ǥ��
	CUnit *m_pUnitData;//���� ������ ������ ����

};

#endif

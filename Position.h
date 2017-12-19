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
	bool m_bIndex;//이동및 선택 가능 여부 표시
	CUnit *m_pUnitData;//말의 정보를 가지고 있음

};

#endif

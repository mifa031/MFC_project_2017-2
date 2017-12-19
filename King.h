#ifndef __KING_H__
#define __KING_H__

#include "Unit.h"
#include "SpecialRule.h"
#include "stdafx.h"


class CKing : public CUnit
{
public :
	CKing(int _x, int _y, int _unitColor);

	bool Move(int i, int j, CPosition** board);

};

#endif

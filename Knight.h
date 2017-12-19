#ifndef __KNIGHT_H__
#define __KNIGHT_H__

#include "Unit.h"
#include "stdafx.h"


class CKnight : public CUnit
{
public :
	CKnight(int _x, int _y, int _unitColor);

	bool Move(int i, int j, CPosition** board);

};

#endif

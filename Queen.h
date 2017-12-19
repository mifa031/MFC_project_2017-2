#ifndef __QUEEN_H__
#define __QUEEN_H__

#include "Unit.h"
#include "stdafx.h"


class CQueen : public CUnit
{
public :
	CQueen(int _x, int _y, int _unitColor);

	bool Move(int i, int j, CPosition** board);

};

#endif

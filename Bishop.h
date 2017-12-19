#ifndef __BISHOP_H__
#define __BISHOP_H__

#include "Unit.h"
#include "stdafx.h"


class CBishop : public CUnit
{
public :
	CBishop(int _x, int _y, int _unitColor);

	bool Move(int i, int j, CPosition** board);

};

#endif

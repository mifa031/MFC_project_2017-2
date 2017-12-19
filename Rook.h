#ifndef __ROOK_H__
#define __ROOK_H__

#include "Unit.h"
#include "stdafx.h"


class CRook : public CUnit
{
public :
	CRook(int _x, int _y, int _unitColor);

	bool Move(int i, int j, CPosition** board);

};

#endif

#ifndef __PAWN_H__
#define __PAWN_H__

#include "Unit.h"
#include "stdafx.h"


class CPawn : public CUnit
{
public :
	CPawn(int _x, int _y, int _unitData);

	bool Move(int i, int j, CPosition** board);

};

#endif

#ifndef __SPECIALRULE_H__
#define __SPECIALRULE_H__

#include "Position.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "Queen.h"
#include "stdafx.h"
#include "PromotionDlg.h"
#include "DefineWords.h"


class CSpecialRule
{
public :
	bool Attacked(int x, int y, int color, CPosition** board);
	int ProMotion(int _nx, int _ny, CPosition** board, int promotionData);
	void Enpassant(int _nx, int _ny, CPosition** board);
	void Castling(CPosition** board);

};
#endif
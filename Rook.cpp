#include <iostream>

#include "Rook.h"


using namespace std;

CRook::CRook(int _x, int _y, int _unitColor)
: CUnit(_x, _y, 'R', _unitColor)
{
	;
}

bool CRook::Move(int i, int j, CPosition** board)
{
	int iX = m_nX;
	int iY = m_nY;

	if ( i == m_nX )
	{//좌우 이동
		if ( j < m_nY )
		{//좌로 이동
			iY--;

			while ( ( iY >= BOARD_MIN_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//장애물을 만날때가지 이동
				if ( iY == j )
				{
					return true;
				}

				iY--;
			}

			if ( ( iY >= BOARD_MIN_RANGE )
				&&( board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//장애물이 적이면 이동가능
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}

		} else
		if ( j > m_nY )
		{//우로 이동
			iY++;

			while ( ( iY <= BOARD_MAX_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//장애물을 만날때가지 이동
				if ( iY == j )
				{
					return true;
				}

				iY++;
			}

			if ( ( iY <= BOARD_MIN_RANGE )
				&&( board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//장애물이 적이면 이동가능
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}
		}

	} else
	if ( j == m_nY )
	{//상하로 이동
		if ( i < m_nX )
		{//상으로 이동
			iX--;

			while ( ( iX >= BOARD_MIN_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//장애물을 만날때가지 이동
				if ( iX == i )
				{
					return true;
				}

				iX--;
			}

			if ( ( iX >= BOARD_MIN_RANGE )
				&&( board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//장애물이 적이면 이동가능
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}

		} else
		if ( i > m_nX )
		{//하로 이동
			iX++;

			while ( ( iX <= BOARD_MAX_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//장애물을 만날때가지 이동
				if ( iX == i )
				{
					return true;
				}

				iX++;
			}//장애물이 적이면 이동가능

			if ( ( iX <= BOARD_MAX_RANGE )
				&&( board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}
		}
	}

	return false;//모든 경우에 해당하지 않으면 false
}

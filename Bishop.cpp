#include <iostream>

#include "Bishop.h"


using namespace std;

CBishop::CBishop(int _x, int _y, int _unitColor)
: CUnit(_x, _y, 'B', _unitColor)
{
	
}

bool CBishop::Move(int i, int j, CPosition** board)
{
	int iX = m_nX;
	int iY = m_nY;

	if ( i > m_nX )
	{//아래쪽 대각선 이동
		if ( j > m_nY )
		{//오른쪽 아래 대각선 방향
			iX++;
			iY++;

			while ( ( iX <= BOARD_MAX_RANGE )&&( iY <= BOARD_MAX_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//다른 유닛이 없을때 까지 이동
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}

				iX++;
				iY++;
			}

			if ( ( iX <= BOARD_MAX_RANGE )&&( iY <= BOARD_MAX_RANGE )
				&&( board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//적이 있을 경우에 이동
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}

		} else
		if ( j < m_nY )
		{//왼쪽 아래 대각선 방향
			iX++;
			iY--;

			while ( ( iX <= BOARD_MAX_RANGE )&&( iY >= BOARD_MIN_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//다른 유닛이 없을때 까지 이동
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}

				iX++;
				iY--;
			}

			if ( ( iX <= BOARD_MAX_RANGE )&&( iY >= BOARD_MIN_RANGE )
				&&(board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//적이 있을 경우에 이동
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}
		}

	} else
	if ( i < m_nX )
	{//위쪽 대각선 이동
		if ( j > m_nY )
		{//오른쪽 위 대각선 방향
			iX--;
			iY++;

			while ( ( iX >= BOARD_MIN_RANGE )&&( iY <= BOARD_MAX_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//다른 유닛이 없을때 까지 이동
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}

				iX--;
				iY++;
			}

			if ( ( iX >= BOARD_MIN_RANGE )&&( iY <= BOARD_MAX_RANGE )
				&&( board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//적이 있을 경우에 이동
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}

		} else
		if ( j < m_nY )
		{//왼쪽 위 대각선 방향
			iX--;
			iY--;

			while ( ( iX >= BOARD_MIN_RANGE )&&( iY >= BOARD_MIN_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//다른 유닛이 없을때 까지 이동
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}

				iX--;
				iY--;
			}

			if ( ( iX >= BOARD_MIN_RANGE )&&( iY >= BOARD_MIN_RANGE )
				&&( board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//적이 있을 경우에 이동
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}
		}
	}

	return false;//TRUE는 이동, FLASE 는 이동 불가
}

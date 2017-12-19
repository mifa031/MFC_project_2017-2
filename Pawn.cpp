#include <iostream>

#include "Pawn.h"


using namespace std;

CPawn::CPawn(int _x, int _y, int _unitColor)
: CUnit(_x, _y, 'P', _unitColor)
{
	;
}

bool CPawn::Move(int i, int j, CPosition** board)//최초 or 앙파상 or 보통 이동
{
	if ( board[m_nX][m_nY].GetUnit()->GetColor() == WHITE )
	{
		if ( board[m_nX][m_nY].GetUnit()->GetEnp() == true )
		{//앙파상 이동 가능 상태
			if ( ( m_nY - 1 >= BOARD_MIN_RANGE )&&( board[m_nX][m_nY - 1].GetUnit() != NULL )
				&&( board[m_nX][m_nY - 1].GetUnit()->GetEnp() == true ) 
				&&( board[m_nX][m_nY - 1].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{
				if ( ( i == m_nX - 1 )&&( j == m_nY - 1 )&&( board[m_nX - 1][m_nY - 1].GetUnit() == NULL ) )
				{
					return true;
				}

			} else
			if ( ( m_nY + 1 <= BOARD_MAX_RANGE )&&( board[m_nX][m_nY + 1].GetUnit() != NULL )
				&&( board[m_nX][m_nY + 1].GetUnit()->GetEnp() == true ) 
				&&( board[m_nX][m_nY + 1].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{
				if ( ( i == m_nX - 1 )&&( j == m_nY + 1 )&&( board[m_nX - 1][m_nY + 1].GetUnit() == NULL ) )
				{
					return true;
				}
			}
		}

		if ( GetFirst() == true )
		{//처음이동시만 두칸 이동
			if ( ( i == m_nX - 2 )&&( j == m_nY )&&( board[i][j].GetUnit() == NULL )
				&&( board[m_nX - 1][m_nY].GetUnit() == NULL ) )
			{//두칸 이동
				return true;
			}
		}

		if ( ( i == m_nX - 1 )&&( j == m_nY )&&( board[i][j].GetUnit() == NULL ) )
		{//한칸 이동
			return true;	

		} else
		if ( ( i == m_nX - 1 )&&( ( j == m_nY - 1 ) || ( j == m_nY + 1 ) )
			&&( ( board[i][j].GetUnit() != NULL )&&( board[i][j].GetUnit()->GetColor() == BLACK ) ) )
		{//적이 있을 경우(대각선)
			return true;			
		}

	} else
	{
		if ( board[m_nX][m_nY].GetUnit()->GetEnp() == true )
		{//앙파상 이동 가능 상태
			if ( ( m_nY - 1 >= BOARD_MIN_RANGE )&&( board[m_nX][m_nY - 1].GetUnit() != NULL )
				&&( board[m_nX][m_nY - 1].GetUnit()->GetEnp() == true )
				&&( board[m_nX][m_nY - 1].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{
				if ( ( i == m_nX + 1 )&&( j == m_nY - 1 )&&( board[m_nX + 1][m_nY - 1].GetUnit() == NULL ) )
				{
					return true;
				}

			} else
			if ( ( m_nY + 1 <= BOARD_MAX_RANGE )&&( board[m_nX][m_nY + 1].GetUnit() != NULL )
				&&( board[m_nX][m_nY + 1].GetUnit()->GetEnp() == true )
				&&( board[m_nX][m_nY + 1].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{
				if ( ( i == m_nX + 1 )&&( j == m_nY + 1 )&&( board[m_nX + 1][m_nY + 1].GetUnit() == NULL ) )
				{
					return true;
				}
			}
		}

		if ( GetFirst() == true )
		{//처음이동시만 두칸 이동 가능
			if ( ( i == m_nX + 2 )&&( j == m_nY )&&( board[i][j].GetUnit() == NULL )
				&&( board[m_nX + 1][m_nY].GetUnit() == NULL ) )
			{//두칸 이동
				return true;
			}
		}

		if( ( i == m_nX + 1 )&&( j == m_nY )&&( board[i][j].GetUnit() == NULL ) )
		{//한칸 이동
			return true;

		} else
		if ( ( i == m_nX + 1 )&&( ( j == m_nY - 1 ) || ( j == m_nY + 1 ) )
			&&( ( board[i][j].GetUnit() != NULL )&&( board[i][j].GetUnit()->GetColor() == WHITE ) ) )
		{//적이 있을 경우
			return true;
		}
	}

	return false;//TRUE는 이동, FLASE 는 이동 불가
}

#include <iostream>

#include "Pawn.h"


using namespace std;

CPawn::CPawn(int _x, int _y, int _unitColor)
: CUnit(_x, _y, 'P', _unitColor)
{
	;
}

bool CPawn::Move(int i, int j, CPosition** board)//���� or ���Ļ� or ���� �̵�
{
	if ( board[m_nX][m_nY].GetUnit()->GetColor() == WHITE )
	{
		if ( board[m_nX][m_nY].GetUnit()->GetEnp() == true )
		{//���Ļ� �̵� ���� ����
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
		{//ó���̵��ø� ��ĭ �̵�
			if ( ( i == m_nX - 2 )&&( j == m_nY )&&( board[i][j].GetUnit() == NULL )
				&&( board[m_nX - 1][m_nY].GetUnit() == NULL ) )
			{//��ĭ �̵�
				return true;
			}
		}

		if ( ( i == m_nX - 1 )&&( j == m_nY )&&( board[i][j].GetUnit() == NULL ) )
		{//��ĭ �̵�
			return true;	

		} else
		if ( ( i == m_nX - 1 )&&( ( j == m_nY - 1 ) || ( j == m_nY + 1 ) )
			&&( ( board[i][j].GetUnit() != NULL )&&( board[i][j].GetUnit()->GetColor() == BLACK ) ) )
		{//���� ���� ���(�밢��)
			return true;			
		}

	} else
	{
		if ( board[m_nX][m_nY].GetUnit()->GetEnp() == true )
		{//���Ļ� �̵� ���� ����
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
		{//ó���̵��ø� ��ĭ �̵� ����
			if ( ( i == m_nX + 2 )&&( j == m_nY )&&( board[i][j].GetUnit() == NULL )
				&&( board[m_nX + 1][m_nY].GetUnit() == NULL ) )
			{//��ĭ �̵�
				return true;
			}
		}

		if( ( i == m_nX + 1 )&&( j == m_nY )&&( board[i][j].GetUnit() == NULL ) )
		{//��ĭ �̵�
			return true;

		} else
		if ( ( i == m_nX + 1 )&&( ( j == m_nY - 1 ) || ( j == m_nY + 1 ) )
			&&( ( board[i][j].GetUnit() != NULL )&&( board[i][j].GetUnit()->GetColor() == WHITE ) ) )
		{//���� ���� ���
			return true;
		}
	}

	return false;//TRUE�� �̵�, FLASE �� �̵� �Ұ�
}

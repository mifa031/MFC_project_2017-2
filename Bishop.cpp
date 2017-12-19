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
	{//�Ʒ��� �밢�� �̵�
		if ( j > m_nY )
		{//������ �Ʒ� �밢�� ����
			iX++;
			iY++;

			while ( ( iX <= BOARD_MAX_RANGE )&&( iY <= BOARD_MAX_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//�ٸ� ������ ������ ���� �̵�
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}

				iX++;
				iY++;
			}

			if ( ( iX <= BOARD_MAX_RANGE )&&( iY <= BOARD_MAX_RANGE )
				&&( board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//���� ���� ��쿡 �̵�
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}

		} else
		if ( j < m_nY )
		{//���� �Ʒ� �밢�� ����
			iX++;
			iY--;

			while ( ( iX <= BOARD_MAX_RANGE )&&( iY >= BOARD_MIN_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//�ٸ� ������ ������ ���� �̵�
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}

				iX++;
				iY--;
			}

			if ( ( iX <= BOARD_MAX_RANGE )&&( iY >= BOARD_MIN_RANGE )
				&&(board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//���� ���� ��쿡 �̵�
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}
		}

	} else
	if ( i < m_nX )
	{//���� �밢�� �̵�
		if ( j > m_nY )
		{//������ �� �밢�� ����
			iX--;
			iY++;

			while ( ( iX >= BOARD_MIN_RANGE )&&( iY <= BOARD_MAX_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//�ٸ� ������ ������ ���� �̵�
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}

				iX--;
				iY++;
			}

			if ( ( iX >= BOARD_MIN_RANGE )&&( iY <= BOARD_MAX_RANGE )
				&&( board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//���� ���� ��쿡 �̵�
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}

		} else
		if ( j < m_nY )
		{//���� �� �밢�� ����
			iX--;
			iY--;

			while ( ( iX >= BOARD_MIN_RANGE )&&( iY >= BOARD_MIN_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//�ٸ� ������ ������ ���� �̵�
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}

				iX--;
				iY--;
			}

			if ( ( iX >= BOARD_MIN_RANGE )&&( iY >= BOARD_MIN_RANGE )
				&&( board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//���� ���� ��쿡 �̵�
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}
		}
	}

	return false;//TRUE�� �̵�, FLASE �� �̵� �Ұ�
}

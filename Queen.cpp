#include <iostream>

#include "Queen.h"


using namespace std;

CQueen::CQueen(int _x, int _y, int _unitColor)
: CUnit(_x, _y, 'Q', _unitColor)
{
	;
}

bool CQueen::Move(int i, int j, CPosition** board)
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
			{//��ֹ��� ���������� �̵� ����
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
			{//��ֹ��� ���������� �̵� ����
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}

				iX++;
				iY--;
			}

			if ( ( iX <= BOARD_MAX_RANGE )&&( iY >= BOARD_MIN_RANGE )
				&&( board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
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
			{//��ֹ��� ���������� �̵� ����
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
			{//��ֹ��� ���������� �̵� ����
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}

				iX--;
				iY--;
			}

			if ( ( iX >= BOARD_MIN_RANGE )&&( iY >= BOARD_MIN_RANGE )
				&&( board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}
		}
	}

	if ( i == m_nX )
	{//�¿� �̵�
		if ( j < m_nY )
		{//�·� �̵�
			iY--;

			while ( ( iY >= BOARD_MIN_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//��ֹ��� ���������� �̵�
				if ( iY == j )
				{
					return true;
				}

				iY--;
			}

			if ( ( iY >= BOARD_MIN_RANGE )
				&&( board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//��ֹ��� ���̸� �̵�����
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}

		} else
		if ( j > m_nY )
		{//��� �̵�
			iY++;

			while ( ( iY <= BOARD_MAX_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//��ֹ��� ���������� �̵�
				if ( iY == j )
				{
					return true;
				}

				iY++;
			}

			if ( ( iY <= BOARD_MAX_RANGE )
				&&( board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//��ֹ��� ���̸� �̵�����
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}
		}

	} else
	if ( j == m_nY )
	{//���Ϸ� �̵�
		if ( i < m_nX )
		{//������ �̵�
			iX--;

			while ( ( iX >= BOARD_MIN_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//��ֹ��� ���������� �̵�
				if ( iX == i )
				{
					return true;
				}

				iX--;
			}

			if ( ( iX >= BOARD_MIN_RANGE )
				&&( board[iX][iY].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//��ֹ��� ���̸� �̵�����
				if ( ( i == iX )&&( j == iY ) )
				{
					return true;
				}
			}

		} else
		if ( i > m_nX )
		{//�Ϸ� �̵�
			iX++;

			while ( ( iX <= BOARD_MAX_RANGE )&&( board[iX][iY].GetUnit() == NULL ) )
			{//��ֹ��� ���������� �̵�
				if ( iX == i )
				{
					return true;
				}

				iX++;

			}//��ֹ��� ���̸� �̵�����
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

	return false;//��� ��쿡 �ش����� ������ false	
}

#include <iostream>

#include "King.h"


using namespace std;

CKing::CKing(int _x, int _y, int _unitColor)
: CUnit(_x, _y, 'K', _unitColor)
{
	
}

bool CKing::Move(int i, int j, CPosition** board) // ĳ���� �϶� or ������ ���
{
	if ( ( board[m_nX][m_nY].GetUnit()->GetCast() == true )
		&&( board[m_nX][m_nY].GetUnit()->GetUnitType() == 'K' )
		&&( i == m_nX )&&( ( j == m_nY - 2 ) || ( j == m_nY + 2 ) ) )
	{//ĳ���� ���� �̵� ���� ����
		if ( board[m_nX][LEFT_ROOK_COL].GetUnit()->GetCast() == true )
		{
			if ( j == m_nY - 2 )
			{
				return true;
			}

		} else
		if ( board[m_nX][RIGHT_ROOK_COL].GetUnit()->GetCast() == true )
		{
			if ( j == m_nY + 2 )
			{
				return true;
			}
		}

	} // ������� ĳ���� ��� 
	else
	if ( i == m_nX + 1 )
	{//�Ʒ��� �̵�
		if ( ( j == m_nY - 1 ) || ( j == m_nY ) || ( j == m_nY + 1 ) )
		{
			if ( ( board[i][j].GetUnit() == NULL ) || ( board[i][j].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//����ְų� �ٸ����� ���� �� �̵�
				return true;
			}
		}

	} else
	if ( i == m_nX )
	{//���� or ������
		if ( ( j == m_nY - 1 ) || ( j == m_nY + 1 ) )
		{
			if ( ( board[i][j].GetUnit() == NULL )
				|| ( board[i][j].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//��ĭ�� ��ų� ��� �����̸� �̵�
				return true;
			}
		}

	} else
	if ( i == m_nX - 1 )
	{//���� �̵�
		if ( ( j == m_nY - 1 ) || ( j == m_nY ) || ( j == m_nY + 1 ) )
		{
			if ( ( board[i][j].GetUnit() == NULL ) 
				|| ( board[i][j].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//��ĭ�� ��ų� ��� �����̸� �̵�
				return true;
			}
		}
	}

	return false;//TRUE�� �̵�, FLASE �� �̵� �Ұ�
}

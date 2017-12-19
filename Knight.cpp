#include <iostream>

#include "Knight.h"


using namespace std;

CKnight::CKnight(int _x, int _y, int _unitColor)
: CUnit(_x, _y, 'N', _unitColor)
{
	
}

bool CKnight::Move(int i, int j, CPosition** board)
{
	if ( i == m_nX + 2 )
	{//아래방향(_|_)
		if ( ( ( j == m_nY - 1 ) || ( j == m_nY + 1 ) )&&( ( board[i][j].GetUnit() == NULL) 
			|| ( board[i][j].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) ) )
		{
			return true;
		}

	} else
	if ( i == m_nX - 2 )
	{//위방향(-|-)
		if ( ( ( j == m_nY - 1 ) || ( j == m_nY + 1 ) )&&( ( board[i][j].GetUnit() == NULL ) 
			|| ( board[i][j].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) ) )
		{
			return true;
		}

	} else
	if ( j == m_nY + 2 )
	{//오른쪽 방향(|--)
		if ( ( ( i == m_nX - 1 ) || ( i == m_nX + 1 ) )&&( ( board[i][j].GetUnit() == NULL ) 
			|| ( board[i][j].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) ) )
		{
			return true;
		}

	} else
	if ( j == m_nY - 2 )
	{//왼쪽 방향(--|)
		if ( ( ( i == m_nX - 1 ) || ( i == m_nX + 1 ) )&&( ( board[i][j].GetUnit() == NULL ) 
			|| ( board[i][j].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) ) )
		{
			return true;
		}
	}

	return false;//TRUE는 이동, FLASE 는 이동 불가
}

#include <iostream>

#include "King.h"


using namespace std;

CKing::CKing(int _x, int _y, int _unitColor)
: CUnit(_x, _y, 'K', _unitColor)
{
	
}

bool CKing::Move(int i, int j, CPosition** board) // 캐슬링 일때 or 보통의 경우
{
	if ( ( board[m_nX][m_nY].GetUnit()->GetCast() == true )
		&&( board[m_nX][m_nY].GetUnit()->GetUnitType() == 'K' )
		&&( i == m_nX )&&( ( j == m_nY - 2 ) || ( j == m_nY + 2 ) ) )
	{//캐슬링 상태 이동 가능 지역
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

	} // 여기까지 캐슬링 경우 
	else
	if ( i == m_nX + 1 )
	{//아래쪽 이동
		if ( ( j == m_nY - 1 ) || ( j == m_nY ) || ( j == m_nY + 1 ) )
		{
			if ( ( board[i][j].GetUnit() == NULL ) || ( board[i][j].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//비어있거나 다른편이 있을 때 이동
				return true;
			}
		}

	} else
	if ( i == m_nX )
	{//왼쪽 or 오른쪽
		if ( ( j == m_nY - 1 ) || ( j == m_nY + 1 ) )
		{
			if ( ( board[i][j].GetUnit() == NULL )
				|| ( board[i][j].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//옆칸이 비거나 상대 유닛이면 이동
				return true;
			}
		}

	} else
	if ( i == m_nX - 1 )
	{//위쪽 이동
		if ( ( j == m_nY - 1 ) || ( j == m_nY ) || ( j == m_nY + 1 ) )
		{
			if ( ( board[i][j].GetUnit() == NULL ) 
				|| ( board[i][j].GetUnit()->GetColor() != board[m_nX][m_nY].GetUnit()->GetColor() ) )
			{//옆칸이 비거나 상대 유닛이면 이동
				return true;
			}
		}
	}

	return false;//TRUE는 이동, FLASE 는 이동 불가
}

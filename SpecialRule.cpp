#include <iostream>
#include <conio.h>

#include "SpecialRule.h"


using namespace std;

bool CSpecialRule::Attacked(int x, int y, int color, CPosition** board)
{//x, y의 자리가 공격을 받는지 여부를 검사
	for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
	{
		for ( int j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++ )
		{
			if ( ( board[i][j].GetUnit() != NULL )&&( board[i][j].GetUnit()->GetColor() != color ) )
			{//i,j 위치에 말이 있고 그 말이 상대편일 경우
				if ( board[i][j].GetUnit()->Move(x, y, board) == true )
				{//그 말이 현재 위치를 이동할수 있는지 여부를 조사
					return true;
				}
			}
		}
	}

	return false;//아무 공격도 받지 않는다면 false
}//bool attacked(int x, int y, bool bColor)

int CSpecialRule::ProMotion(int _nx, int _ny, CPosition** board, int promotionData)
{//폰의 프로모션 기능
	int color;//말의 색상을 입력 받음
	CPromotionDlg cSelect;

	color = board[_nx][_ny].GetUnit()->GetColor();

	if ( promotionData == PROMOTION_NONE )
	{
		cSelect.DoModal();
		promotionData = cSelect.GetProData();
	}

	switch ( promotionData )
	{
		case PROMOTION_TO_QUEEN :
		{//퀸으로 프로모션
			delete board[_nx][_ny].GetUnit();
			board[_nx][_ny].SetUnit(new CQueen(_nx, _ny, color));

			break;
		}

		case PROMOTION_TO_ROOK :
		{//룩으로 프로모션
			delete board[_nx][_ny].GetUnit();
			board[_nx][_ny].SetUnit(new CRook(_nx, _ny, color));

			break;
		}

		case PROMOTION_TO_KNIGHT :
		{//나이트로 프로모션
			delete board[_nx][_ny].GetUnit();
			board[_nx][_ny].SetUnit(new CKnight(_nx, _ny, color));

			break;
		}

		case PROMOTION_TO_BISHOP :
		{//비숍으로 프로모션
			delete board[_nx][_ny].GetUnit();
			board[_nx][_ny].SetUnit(new CBishop(_nx, _ny, color));

			break;
		}

	}//switch(cSelect)

	return promotionData;
}//void proMotion(int x, int y)

void CSpecialRule::Enpassant(int _nx, int _ny, CPosition** board)
{//앙파상 설정:앙파상을 유발하는 움직임일 경우 상대방 P에 앙파상 모드를 켬
	if ( ( _ny + 1 <= BOARD_MAX_RANGE )&&( board[_nx][_ny + 1].GetUnit() != NULL )
		&&( board[_nx][_ny + 1].GetUnit()->GetUnitType() == 'P' )
		&&( board[_nx][_ny + 1].GetUnit()->GetColor() != board[_nx][_ny].GetUnit()->GetColor() ) )
	{//상대방이 오른편에 있을 경우
		board[_nx][_ny + 1].GetUnit()->SetEnp();
		board[_nx][_ny].GetUnit()->SetEnp();

	} else
	if ( ( _ny - 1 >= BOARD_MIN_RANGE )&&( board[_nx][_ny - 1].GetUnit() != NULL )
		&&( board[_nx][_ny - 1].GetUnit()->GetUnitType() == 'P')
		&&( board[_nx][_ny - 1].GetUnit()->GetColor() != board[_nx][_ny].GetUnit()->GetColor() ) )
	{//상대방이 왼편에 있을 경우
		board[_nx][_ny - 1].GetUnit()->SetEnp();
		board[_nx][_ny].GetUnit()->SetEnp();
	}
}//void enPassant(int _nx, int _ny)

void CSpecialRule::Castling(CPosition** board)
{//캐슬링 설정
	bool bCastlingMoveAttack;//킹유닛과 킹과 룩사이의 위치가 공격받는지 여부
	int iY;
	int i, j;

	for ( i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
	{//캐슬링 모두 해제
		for ( j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++ )
		{
			if ( board[i][j].GetUnit() != NULL )
			{
				board[i][j].GetUnit()->DelCast();
			}
		}
	}

	if ( ( board[WHITE_CASTLING_ROW][KING_BASE_COL].GetUnit() != NULL )
		&&( board[WHITE_CASTLING_ROW][KING_BASE_COL].GetUnit()->GetUnitType() == 'K' )
		&&( board[WHITE_CASTLING_ROW][KING_BASE_COL].GetUnit()->GetFirst() == true ) )
	{//백킹이 움직이지 않았을경우
		if ( ( board[WHITE_CASTLING_ROW][LEFT_ROOK_COL].GetUnit() != NULL )
			&&( board[WHITE_CASTLING_ROW][LEFT_ROOK_COL].GetUnit()->GetFirst() == true ) )
		{//백 왼쪽 캐슬링여부:룩이 움직이지 않은 경우
			if ( ( board[WHITE_CASTLING_ROW][KING_BASE_COL - 1].GetUnit() == NULL )
				&&( board[WHITE_CASTLING_ROW][KING_BASE_COL - 2].GetUnit() == NULL )
				&&( board[WHITE_CASTLING_ROW][KING_BASE_COL - 3].GetUnit() == NULL ) )
			{//룩과 킹 사이에 비어있는 경우
				iY = KING_BASE_COL;

				do
				{//룩옆자리부터 킹까지 공격을 받는지 여부 조사
					bCastlingMoveAttack = Attacked(WHITE_CASTLING_ROW, iY, WHITE, board);	
					iY--;
				} while ( ( iY > BOARD_MIN_RANGE )&&( bCastlingMoveAttack != true ) );	

				if ( bCastlingMoveAttack == false )
				{//공격을 받지 않는다면 캐슬링모드 설정
					board[WHITE_CASTLING_ROW][KING_BASE_COL].GetUnit()->SetCast();
					board[WHITE_CASTLING_ROW][LEFT_ROOK_COL].GetUnit()->SetCast();
				}
			}
		}

		if ( ( board[WHITE_CASTLING_ROW][RIGHT_ROOK_COL].GetUnit() != NULL )
			&&( board[WHITE_CASTLING_ROW][RIGHT_ROOK_COL].GetUnit()->GetFirst() == true ) )
		{//백 오른쪽 캐슬링여부:룩이 움지이지 않은 경우
			if ( ( board[WHITE_CASTLING_ROW][KING_BASE_COL + 1].GetUnit() == NULL )
				&&( board[WHITE_CASTLING_ROW][KING_BASE_COL + 2].GetUnit() == NULL ) )
			{//룩과 킹 사이에 비어있는 경우
				iY = KING_BASE_COL;

				do
				{//룩옆자리부터 킹까지 공격을 받는지 여부 조사
					bCastlingMoveAttack = Attacked(WHITE_CASTLING_ROW, iY, WHITE, board);	
					iY++;
				} while ( ( iY < BOARD_MAX_RANGE )&&( bCastlingMoveAttack != true ) );
 
				if ( bCastlingMoveAttack == false )
				{//공격을 받지 않는다면 캐슬링모드 설정
					board[WHITE_CASTLING_ROW][KING_BASE_COL].GetUnit()->SetCast();
					board[WHITE_CASTLING_ROW][RIGHT_ROOK_COL].GetUnit()->SetCast();
				}
			}
		}
	}

	if ( ( board[BLACK_CASTLING_ROW][KING_BASE_COL].GetUnit() != NULL )
		&&( board[BLACK_CASTLING_ROW][KING_BASE_COL].GetUnit()->GetUnitType() == 'K' )
		&&( board[BLACK_CASTLING_ROW][KING_BASE_COL].GetUnit() -> GetFirst() == true) )
	{
		if ( ( board[BLACK_CASTLING_ROW][LEFT_ROOK_COL].GetUnit() != NULL )
			&&( board[BLACK_CASTLING_ROW][LEFT_ROOK_COL].GetUnit()->GetFirst() == true ) )
		{//흑 왼쪽 캐슬링:룩이 움직이지 않은 경우
			if ( ( board[BLACK_CASTLING_ROW][KING_BASE_COL - 1].GetUnit() == NULL )
				&&( board[BLACK_CASTLING_ROW][KING_BASE_COL - 2].GetUnit() == NULL )
				&&( board[BLACK_CASTLING_ROW][KING_BASE_COL - 3].GetUnit() == NULL ) )
			{//룩과 킹 사이에 비어있는 경우
				iY = KING_BASE_COL;

				do
				{//룩옆자리부터 킹까지 공격을 받는지 여부 조사
					bCastlingMoveAttack = Attacked(BLACK_CASTLING_ROW, iY, BLACK, board);	
					iY--;
				} while ( ( iY > BOARD_MIN_RANGE )&&( bCastlingMoveAttack != true ) );

				if ( bCastlingMoveAttack == false )
				{//공격을 받지 않는다면 캐슬링모드 설정
					board[BLACK_CASTLING_ROW][KING_BASE_COL].GetUnit()->SetCast();
					board[BLACK_CASTLING_ROW][LEFT_ROOK_COL].GetUnit()->SetCast();
				}
			}
		}

		if ( ( board[BLACK_CASTLING_ROW][RIGHT_ROOK_COL].GetUnit() != NULL )
			&&( board[BLACK_CASTLING_ROW][RIGHT_ROOK_COL].GetUnit()->GetFirst() == true ) )
		{//흑 오른쪽 캐슬링:룩이 움지이지 않은 경우
			if ( ( board[BLACK_CASTLING_ROW][KING_BASE_COL + 1].GetUnit() == NULL )
				&&( board[BLACK_CASTLING_ROW][KING_BASE_COL + 2].GetUnit() == NULL ) )
			{//룩과 킹 사이에 비어있는 경우
				iY = KING_BASE_COL;

				do
				{//룩옆자리부터 킹까지 공격을 받는지 여부 조사
					bCastlingMoveAttack = Attacked(BLACK_CASTLING_ROW, iY, BLACK, board);	
					iY++;
				} while ( ( iY < BOARD_MAX_RANGE )&&( bCastlingMoveAttack != true ) );

				if ( bCastlingMoveAttack == false )
				{//공격을 받지 않는다면 캐슬링모드 설정
					board[BLACK_CASTLING_ROW][KING_BASE_COL].GetUnit()->SetCast();
					board[BLACK_CASTLING_ROW][RIGHT_ROOK_COL].GetUnit()->SetCast();
				}
			}
		}
	}
}//void castling()

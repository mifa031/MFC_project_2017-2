#include <iostream>
#include <conio.h>

#include "SpecialRule.h"


using namespace std;

bool CSpecialRule::Attacked(int x, int y, int color, CPosition** board)
{//x, y�� �ڸ��� ������ �޴��� ���θ� �˻�
	for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
	{
		for ( int j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++ )
		{
			if ( ( board[i][j].GetUnit() != NULL )&&( board[i][j].GetUnit()->GetColor() != color ) )
			{//i,j ��ġ�� ���� �ְ� �� ���� ������� ���
				if ( board[i][j].GetUnit()->Move(x, y, board) == true )
				{//�� ���� ���� ��ġ�� �̵��Ҽ� �ִ��� ���θ� ����
					return true;
				}
			}
		}
	}

	return false;//�ƹ� ���ݵ� ���� �ʴ´ٸ� false
}//bool attacked(int x, int y, bool bColor)

int CSpecialRule::ProMotion(int _nx, int _ny, CPosition** board, int promotionData)
{//���� ���θ�� ���
	int color;//���� ������ �Է� ����
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
		{//������ ���θ��
			delete board[_nx][_ny].GetUnit();
			board[_nx][_ny].SetUnit(new CQueen(_nx, _ny, color));

			break;
		}

		case PROMOTION_TO_ROOK :
		{//������ ���θ��
			delete board[_nx][_ny].GetUnit();
			board[_nx][_ny].SetUnit(new CRook(_nx, _ny, color));

			break;
		}

		case PROMOTION_TO_KNIGHT :
		{//����Ʈ�� ���θ��
			delete board[_nx][_ny].GetUnit();
			board[_nx][_ny].SetUnit(new CKnight(_nx, _ny, color));

			break;
		}

		case PROMOTION_TO_BISHOP :
		{//������� ���θ��
			delete board[_nx][_ny].GetUnit();
			board[_nx][_ny].SetUnit(new CBishop(_nx, _ny, color));

			break;
		}

	}//switch(cSelect)

	return promotionData;
}//void proMotion(int x, int y)

void CSpecialRule::Enpassant(int _nx, int _ny, CPosition** board)
{//���Ļ� ����:���Ļ��� �����ϴ� �������� ��� ���� P�� ���Ļ� ��带 ��
	if ( ( _ny + 1 <= BOARD_MAX_RANGE )&&( board[_nx][_ny + 1].GetUnit() != NULL )
		&&( board[_nx][_ny + 1].GetUnit()->GetUnitType() == 'P' )
		&&( board[_nx][_ny + 1].GetUnit()->GetColor() != board[_nx][_ny].GetUnit()->GetColor() ) )
	{//������ ������ ���� ���
		board[_nx][_ny + 1].GetUnit()->SetEnp();
		board[_nx][_ny].GetUnit()->SetEnp();

	} else
	if ( ( _ny - 1 >= BOARD_MIN_RANGE )&&( board[_nx][_ny - 1].GetUnit() != NULL )
		&&( board[_nx][_ny - 1].GetUnit()->GetUnitType() == 'P')
		&&( board[_nx][_ny - 1].GetUnit()->GetColor() != board[_nx][_ny].GetUnit()->GetColor() ) )
	{//������ ���� ���� ���
		board[_nx][_ny - 1].GetUnit()->SetEnp();
		board[_nx][_ny].GetUnit()->SetEnp();
	}
}//void enPassant(int _nx, int _ny)

void CSpecialRule::Castling(CPosition** board)
{//ĳ���� ����
	bool bCastlingMoveAttack;//ŷ���ְ� ŷ�� ������� ��ġ�� ���ݹ޴��� ����
	int iY;
	int i, j;

	for ( i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
	{//ĳ���� ��� ����
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
	{//��ŷ�� �������� �ʾ������
		if ( ( board[WHITE_CASTLING_ROW][LEFT_ROOK_COL].GetUnit() != NULL )
			&&( board[WHITE_CASTLING_ROW][LEFT_ROOK_COL].GetUnit()->GetFirst() == true ) )
		{//�� ���� ĳ��������:���� �������� ���� ���
			if ( ( board[WHITE_CASTLING_ROW][KING_BASE_COL - 1].GetUnit() == NULL )
				&&( board[WHITE_CASTLING_ROW][KING_BASE_COL - 2].GetUnit() == NULL )
				&&( board[WHITE_CASTLING_ROW][KING_BASE_COL - 3].GetUnit() == NULL ) )
			{//��� ŷ ���̿� ����ִ� ���
				iY = KING_BASE_COL;

				do
				{//�迷�ڸ����� ŷ���� ������ �޴��� ���� ����
					bCastlingMoveAttack = Attacked(WHITE_CASTLING_ROW, iY, WHITE, board);	
					iY--;
				} while ( ( iY > BOARD_MIN_RANGE )&&( bCastlingMoveAttack != true ) );	

				if ( bCastlingMoveAttack == false )
				{//������ ���� �ʴ´ٸ� ĳ������� ����
					board[WHITE_CASTLING_ROW][KING_BASE_COL].GetUnit()->SetCast();
					board[WHITE_CASTLING_ROW][LEFT_ROOK_COL].GetUnit()->SetCast();
				}
			}
		}

		if ( ( board[WHITE_CASTLING_ROW][RIGHT_ROOK_COL].GetUnit() != NULL )
			&&( board[WHITE_CASTLING_ROW][RIGHT_ROOK_COL].GetUnit()->GetFirst() == true ) )
		{//�� ������ ĳ��������:���� �������� ���� ���
			if ( ( board[WHITE_CASTLING_ROW][KING_BASE_COL + 1].GetUnit() == NULL )
				&&( board[WHITE_CASTLING_ROW][KING_BASE_COL + 2].GetUnit() == NULL ) )
			{//��� ŷ ���̿� ����ִ� ���
				iY = KING_BASE_COL;

				do
				{//�迷�ڸ����� ŷ���� ������ �޴��� ���� ����
					bCastlingMoveAttack = Attacked(WHITE_CASTLING_ROW, iY, WHITE, board);	
					iY++;
				} while ( ( iY < BOARD_MAX_RANGE )&&( bCastlingMoveAttack != true ) );
 
				if ( bCastlingMoveAttack == false )
				{//������ ���� �ʴ´ٸ� ĳ������� ����
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
		{//�� ���� ĳ����:���� �������� ���� ���
			if ( ( board[BLACK_CASTLING_ROW][KING_BASE_COL - 1].GetUnit() == NULL )
				&&( board[BLACK_CASTLING_ROW][KING_BASE_COL - 2].GetUnit() == NULL )
				&&( board[BLACK_CASTLING_ROW][KING_BASE_COL - 3].GetUnit() == NULL ) )
			{//��� ŷ ���̿� ����ִ� ���
				iY = KING_BASE_COL;

				do
				{//�迷�ڸ����� ŷ���� ������ �޴��� ���� ����
					bCastlingMoveAttack = Attacked(BLACK_CASTLING_ROW, iY, BLACK, board);	
					iY--;
				} while ( ( iY > BOARD_MIN_RANGE )&&( bCastlingMoveAttack != true ) );

				if ( bCastlingMoveAttack == false )
				{//������ ���� �ʴ´ٸ� ĳ������� ����
					board[BLACK_CASTLING_ROW][KING_BASE_COL].GetUnit()->SetCast();
					board[BLACK_CASTLING_ROW][LEFT_ROOK_COL].GetUnit()->SetCast();
				}
			}
		}

		if ( ( board[BLACK_CASTLING_ROW][RIGHT_ROOK_COL].GetUnit() != NULL )
			&&( board[BLACK_CASTLING_ROW][RIGHT_ROOK_COL].GetUnit()->GetFirst() == true ) )
		{//�� ������ ĳ����:���� �������� ���� ���
			if ( ( board[BLACK_CASTLING_ROW][KING_BASE_COL + 1].GetUnit() == NULL )
				&&( board[BLACK_CASTLING_ROW][KING_BASE_COL + 2].GetUnit() == NULL ) )
			{//��� ŷ ���̿� ����ִ� ���
				iY = KING_BASE_COL;

				do
				{//�迷�ڸ����� ŷ���� ������ �޴��� ���� ����
					bCastlingMoveAttack = Attacked(BLACK_CASTLING_ROW, iY, BLACK, board);	
					iY++;
				} while ( ( iY < BOARD_MAX_RANGE )&&( bCastlingMoveAttack != true ) );

				if ( bCastlingMoveAttack == false )
				{//������ ���� �ʴ´ٸ� ĳ������� ����
					board[BLACK_CASTLING_ROW][KING_BASE_COL].GetUnit()->SetCast();
					board[BLACK_CASTLING_ROW][RIGHT_ROOK_COL].GetUnit()->SetCast();
				}
			}
		}
	}
}//void castling()

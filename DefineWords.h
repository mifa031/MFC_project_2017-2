#ifndef __DEFINEWORDS_H__
#define __DEFINEWORDS_H__


// ��� �������� ��尪��
#define USER_MODE			1
#define GAME_MODE			2
#define CHAT_MODE			3
#define RENEW_MODE			4
#define RENEW_ACK_MODE		5
#define CLOSE_MODE			6
#define EXIT_MODE			7
#define TIME_OVER_MODE		8

#define RENEW_YES			1
#define RENEW_NO			0

// ���ӷ� ������
#define FIRST_PAGE			1
#define SECOND_PAGE			2
#define THIRD_PAGE			3

// �α��� ���
#define SERVER_MODE			0
#define CLIENT_MODE			1
#define LOCAL_MODE			2

// ���� �޼���
#define UM_ACCEPT			( WM_USER + 10 )
#define UM_DATARECEIVE		( WM_USER + 11 )
#define UM_CLOSELOGIN		( WM_USER + 12 )

// ���� ���� ������
#define SELECT_INPUT		0
#define MOVE_INPUT			1
#define WHITE_TURN			0
#define BLACK_TURN			1

// ��Ʈ��ũ ��� ��Ʈ
#define PORT_NUM			6565

// ���θ�� ������
#define PROMOTION_NONE		0
#define PROMOTION_TO_QUEEN	1
#define PROMOTION_TO_ROOK	2
#define PROMOTION_TO_KNIGHT	3
#define PROMOTION_TO_BISHOP	4

// ü���� ���� ��� ������
#define BOARD_MAX_RANGE		7
#define BOARD_MIN_RANGE		0
#define KING_BASE_COL		4
#define WHITE_ENPASSANT_ROW	3
#define BLACK_ENPASSANT_ROW	4
#define LEFT_ROOK_COL		0
#define RIGHT_ROOK_COL		7
#define BLACK_CASTLING_ROW	0
#define WHITE_CASTLING_ROW	7
#define WHITE_PROMOTION_ROW	0
#define BLACK_PROMOTION_ROW	7
#define	BLACK				1
#define WHITE				0

// Ÿ�̸� �ð� ����
#define DEFAULT_REMAIN_TIME	180

#endif

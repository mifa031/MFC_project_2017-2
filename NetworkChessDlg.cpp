
#include "stdafx.h"
#include "NetworkChess.h"
#include "NetworkChessDlg.h"


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();


	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  



protected:
	
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	
END_MESSAGE_MAP()

CNetworkChessDlg::CNetworkChessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetworkChessDlg::IDD, pParent)
{

	m_strStatusView = _T("");
	m_strBlackUser = _T("");
	m_strWhiteUser = _T("");
	m_strChatInput = _T("");
	m_strCountDown = _T("");
	

	// 멤버 변수들 초기화
	m_nInputMode = SELECT_INPUT;
	m_nX = NULL;
	m_nY = NULL;
	m_nTargetX = NULL;
	m_nTargetY = NULL;
	m_nOldX = NULL;
	m_nOldY = NULL;
	b_x = 0;
	b_y = 0;
	m_nOldTargetX = NULL;
	m_nOldTargetY = NULL;
	m_bEnpassantB = true;
	m_bEnpassantW = true;
	m_nTurn = WHITE_TURN;//기본턴은 백말 먼저
	m_bConnect = false;
	m_GameSock = NULL;
	m_ServerSock = NULL;
	is_on_chat_input = 1;

	SetBoard();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
	}
}

CNetworkChessDlg NetDlg;

void CNetworkChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHAT_INPUT, m_ctrEdit);
	DDX_Control(pDX, IDC_CHAT_LIST, m_ctrChatList);
	DDX_Text(pDX, IDC_STATUS_VIEW, m_strStatusView);
	DDX_Text(pDX, IDC_BLACK_USER, m_strBlackUser);
	DDX_Text(pDX, IDC_WHITE_USER, m_strWhiteUser);
	DDX_Text(pDX, IDC_CHAT_INPUT, m_strChatInput);
}

BEGIN_MESSAGE_MAP(CNetworkChessDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
	ON_BN_CLICKED(IDC_RENEW_BUTTON, OnRenewButton)
	ON_BN_CLICKED(IDC_CLOSE_LINK_BUTTON, OnCloseLinkButton)
	ON_BN_CLICKED(IDC_GAME_INFO_BUTTON, OnGameInfoButton)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_MESSAGE(UM_ACCEPT, OnAccept)
	ON_MESSAGE(UM_DATARECEIVE, OnReceive)
	ON_MESSAGE(UM_CLOSELOGIN, OnCloseFromLogin)
	ON_COMMAND(ID_ENTERKEY, OnEnterkey)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


BOOL CNetworkChessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	ASSERT(( IDM_ABOUTBOX & 0xFFF0 ) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if ( pSysMenu != NULL )
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if ( !strAboutMenu.IsEmpty() )
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	
	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	
	ViewLoginDlg();// 로그인 창을 DoModal로 띄어줌

	p1 = AfxBeginThread(SpeakThreadWhiteTurn, this, THREAD_PRIORITY_LOWEST);


	return TRUE;  
}

void CNetworkChessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();

	} else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CNetworkChessDlg::OnPaint() 
{
	CPaintDC dc(this);

	if ( IsIconic() )
	{
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);

	} else
	{
		CDialog::OnPaint();
	}

	CDC dcBoardMem, dcMem;
	CBitmap bitmapWhite, bitmapBlack, bitmapSelWhite, bitmapSelBlack;
	CBitmap bitmapWK, bitmapWQ, bitmapWR;
	CBitmap bitmapWB, bitmapWP, bitmapWN;
	CBitmap bitmapBK, bitmapBQ, bitmapBR; 
	CBitmap bitmapBP, bitmapBN, bitmapBB;
	CBitmap bitmapWKC, bitmapBKC;
	CBitmap bitmapEP, bitmapCS, bitmapSkin;

	bitmapWhite.LoadBitmap(IDB_WHITEBOARD);
	bitmapBlack.LoadBitmap(IDB_BLACKBOARD);
	bitmapSelWhite.LoadBitmap(IDB_WHITESELBOARD);
	bitmapSelBlack.LoadBitmap(IDB_BLACKSELBOARD);
	bitmapSkin.LoadBitmap(IDB_CHESS_SKIN);

	bitmapWK.LoadBitmap(IDB_WHITEKING);
	bitmapWKC.LoadBitmap(IDB_WHITEKING_CHECK);
	bitmapWQ.LoadBitmap(IDB_WHITEQUEEN);
	bitmapWR.LoadBitmap(IDB_WHITEROOK);
	bitmapWB.LoadBitmap(IDB_WHITEBISHOP);
	bitmapWP.LoadBitmap(IDB_WHITEPAWN);
	bitmapWN.LoadBitmap(IDB_WHITEKNIGHT);
	bitmapBK.LoadBitmap(IDB_BLACKKING);
	bitmapBKC.LoadBitmap(IDB_BLACKKING_CHECK);
	bitmapBQ.LoadBitmap(IDB_BLACKQUEEN);
	bitmapBR.LoadBitmap(IDB_BLACKROOK);
	bitmapBP.LoadBitmap(IDB_BLACKPAWN);
	bitmapBN.LoadBitmap(IDB_BLACKKNIGHT);
	bitmapBB.LoadBitmap(IDB_BLACKBISHOP);
	bitmapCS.LoadBitmap(IDB_CASTLING);
	bitmapEP.LoadBitmap(IDB_ENPASANT);

	dcMem.CreateCompatibleDC(&dc);
	dcBoardMem.CreateCompatibleDC(&dc);


	CBitmap* pOldBoardBitmap = dcBoardMem.SelectObject(&bitmapSkin);
	CBitmap* pOldBitmap = dcMem.SelectObject(&bitmapBK);

	// 배경 그림
	dc.BitBlt(0, 0, 641, 423, &dcBoardMem, 0, 0, SRCCOPY);

	// 처음 위치 12, 11
	for ( int i = 0; i < 8; i++ )
	{
		for ( int j = 0; j < 8; j++ )
		{
			if ( ( i + j ) % 2 )
			{// 하얀판
				if ( m_Board[i][j].GetIndex() == true )
				{//인덱스가 잡힌경우 인덱스 출력
					dcBoardMem.SelectObject(&bitmapSelWhite);
					dc.BitBlt(( 50 * j ) + 12, ( 50 * i ) + 12, 50, 50, &dcBoardMem, 0, 0, SRCCOPY);

				} else
				{
					dcBoardMem.SelectObject(&bitmapWhite);
					dc.BitBlt(( 50 * j ) + 12, ( 50 * i ) + 12, 50, 50, &dcBoardMem, 0, 0, SRCCOPY);
				}

			} else
			{// 검은판
				if ( m_Board[i][j].GetIndex() == true )
				{//인덱스가 잡힌경우 인덱스 출력
					dcBoardMem.SelectObject(&bitmapSelBlack);
					dc.BitBlt(( 50 * j ) + 12, ( 50 * i ) + 12, 50, 50, &dcBoardMem, 0, 0, SRCCOPY);

				} else
				{
					dcBoardMem.SelectObject(&bitmapBlack);
					dc.BitBlt(( 50 * j ) + 12, ( 50 * i ) + 12, 50, 50, &dcBoardMem, 0, 0, SRCCOPY);
				}
			}

			if ( m_Board[i][j].GetUnit() != NULL )
			{// 유닛이 있을 경우 유닛에 따라 해당 유닛을 그려줌
				if ( m_Board[i][j].GetUnit()->GetColor() == BLACK )
				{
					switch ( m_Board[i][j].GetUnit()->GetUnitType() )
					{
						case 'K' :
						{
							if ( m_bCheckMateB == true )
							{
								dcMem.SelectObject(&bitmapBKC);

							} else
							{
								dcMem.SelectObject(&bitmapBK);
							}

							dc.BitBlt(( 50 * j ) + 17, ( 50 * i ) + 17, 40, 40, &dcMem, 0, 0, SRCCOPY);

							break;
						}

						case 'Q' :
						{
							dcMem.SelectObject(&bitmapBQ);
							dc.BitBlt(( 50 * j ) + 17, ( 50 * i ) + 17, 40, 40, &dcMem, 0, 0, SRCCOPY);

							break;
						}

						case 'R' :
						{
							dcMem.SelectObject(&bitmapBR);
							dc.BitBlt(( 50 * j ) + 17, ( 50 * i ) + 17, 40, 40, &dcMem, 0, 0, SRCCOPY);
	
							break;
						}

						case 'B' :
						{
							dcMem.SelectObject(&bitmapBB);
							dc.BitBlt(( 50 * j ) + 17, ( 50 * i ) + 17, 40, 40, &dcMem, 0, 0, SRCCOPY);

							break;
						}

						case 'N' :
						{
							dcMem.SelectObject(&bitmapBN);
							dc.BitBlt(( 50 * j ) + 17, ( 50 * i ) + 17, 40, 40, &dcMem, 0, 0, SRCCOPY);

							break;
						}

						case 'P' :
						{
							dcMem.SelectObject(&bitmapBP);
							dc.BitBlt(( 50 * j ) + 17, ( 50 * i ) + 17, 40, 40, &dcMem, 0, 0, SRCCOPY);

							break;
						}
					}

				} else
				{
					switch ( m_Board[i][j].GetUnit()->GetUnitType() )
					{
						case 'K' :
						{
							if ( m_bCheckMateW == true )
							{
								dcMem.SelectObject(&bitmapWKC);

							} else
							{
								dcMem.SelectObject(&bitmapWK);
							}

							dc.BitBlt(( 50 * j ) + 17, ( 50 * i ) + 17, 40, 40, &dcMem, 0, 0, SRCCOPY);

							break;
						}

						case 'Q' :
						{
							dcMem.SelectObject(&bitmapWQ);
							dc.BitBlt(( 50 * j ) + 17, ( 50 * i ) + 17, 40, 40, &dcMem, 0, 0, SRCCOPY);

							break;
						}

						case 'R' :
						{
							dcMem.SelectObject(&bitmapWR);
							dc.BitBlt(( 50 * j ) + 17, ( 50 * i ) + 17, 40, 40, &dcMem, 0, 0, SRCCOPY);

							break;
						}

						case 'B' :
						{
							dcMem.SelectObject(&bitmapWB);
							dc.BitBlt(( 50 * j ) + 17, ( 50 * i ) + 17, 40, 40, &dcMem, 0, 0, SRCCOPY);

							break;
						}

						case 'N' :
						{
							dcMem.SelectObject(&bitmapWN);
							dc.BitBlt(( 50 * j ) + 17, ( 50 * i ) + 17, 40, 40, &dcMem, 0, 0, SRCCOPY);

							break;
						}

						case 'P' :
						{
							dcMem.SelectObject(&bitmapWP);
							dc.BitBlt(( 50 * j ) + 17, ( 50 * i ) + 17, 40, 40, &dcMem, 0, 0, SRCCOPY);

							break;
						}
					}
				}

				if ( m_Board[i][j].GetUnit()->GetEnp() == true )
				{// 앙파상 모드 표시
					dcMem.SelectObject(&bitmapEP);
					dc.BitBlt(( 50 * j ) + 12, ( 50 * i ) + 12, 9, 5, &dcMem, 0, 0, SRCCOPY);
				}

				if ( m_Board[i][j].GetUnit()->GetCast() == true )
				{// 캐슬링 모드 표시
					dcMem.SelectObject(&bitmapCS);
					dc.BitBlt(( 50 * j ) + 12, ( 50 * i ) + 12, 9, 5, &dcMem, 0, 0, SRCCOPY);
				}
			}
		}
	}

	CPen pen;
	pen.CreatePen(PS_SOLID, 3, RGB(255,0,322));
	CPen *pOldPen = dc.SelectObject(&pen);
	if (delline == 0) {
		dc.SelectStockObject(NULL_PEN);
	}
	dc.MoveTo(( m_nOldY * 50 ) + 37, ( m_nOldX * 50 ) + 37);
	dc.LineTo(( m_nOldTargetY * 50 ) + 37, ( m_nOldTargetX * 50 ) + 37);
	
	dc.SelectObject(&pOldPen);
	dcMem.SelectObject(&pOldBitmap);
	dcBoardMem.SelectObject(&pOldBoardBitmap);
	
	
	if (m_nTurn == BLACK_TURN)
	{

		
		m_strStatusView = "";
		UpdateData(false);
		m_strStatusView.AppendFormat("흑의 차례입니다                          ");
		UpdateData(FALSE);

	}
	else
		if (m_nTurn == WHITE_TURN)
		{
			m_strStatusView = "";
			UpdateData(false);
			m_strStatusView.AppendFormat("백의 차례입니다                          ");
			UpdateData(FALSE);
		}
	if (m_bCheckMateW == true)
	{
		

		m_strStatusView.AppendFormat("백의 킹이 체크입니다");
		UpdateData(false);

	}
	if (m_bCheckMateB == true)
	{
	
		m_strStatusView.AppendFormat("흑의 킹이 체크입니다");
		UpdateData(false);

	}
}


HCURSOR CNetworkChessDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNetworkChessDlg::SetMoveIndex()
{// 말이 이동할 곳에 인덱스를 매겨주는 함수
	m_bIndexOK = false;
		
	for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
	{//선택 말에 대한 이동 인덱스 값 새로 매김
		for ( int j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++ )
		{
			m_Board[i][j].DelIndex();//전단계 인덱스 삭제

			if ( ( m_Board[m_nX][m_nY].GetUnit() != NULL )&&
				( m_Board[m_nX][m_nY].GetUnit()->Move(i, j, m_Board) == true ) )
			{
				m_Board[i][j].SetIndex();
				m_bIndexOK = true;//인덱스 매겨진 공간이 있는지 확인
			}
		}
	}
}

void CNetworkChessDlg::EndGame()
{// 게임 종료시 발생
	
	int iGameOver = 0;

	for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
	{//게임의 끝남
		for ( int j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++ )
		{
			if ( ( m_Board[i][j].GetUnit() != NULL )&&( m_Board[i][j].GetUnit()->GetUnitType() == 'K' ) )
			{//킹을 찾음
				if ( m_Board[i][j].GetUnit()->GetColor() == BLACK )
				{//흑킹이 있으면 1증가
					iGameOver++;

				} else
				{//백킹이 있으면 3증가
					iGameOver += 3;
				}
			}//if ( ( m_Board[i][j].GetUnit()->GetUnitType() == 'K' )
		}
	}

	switch ( iGameOver )
	{
		case 1 :
		{//흑킹만 있는 경우
			AfxMessageBox(_T("흑팀이 이겼습니다!!"));

			for (int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{//메모리 해제
				delete[] m_Board[i];
			}
			delete[] m_Board;
			m_Board = NULL;

			SetBoard();
			m_nTurn = BLACK_TURN;
			if (m_LoginDlg.GetUserName() == m_strBlackUser)
				IndexSetting();
			delline = 0;
			Invalidate();

			break;
		}

		case 3 :
		{//백킹만 있는경우
			AfxMessageBox(_T("백팀이 이겼습니다!!"));

			for(int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{
				delete[] m_Board[i];
			}
			delete[] m_Board;
			m_Board = NULL;

			SetBoard();
			m_nTurn = WHITE_TURN;
			if (m_LoginDlg.GetUserName() == m_strWhiteUser)
				IndexSetting();
			delline = 0;
			Invalidate();
			break;
		}

		default :
		{//4인경우.. 두개의 킹이 다 존재하므로 게임 계속
			break;
		}
	}//switch(endGame)

}

void CNetworkChessDlg::SetBoard()
{// 체스판을 셋팅
	m_Board = new CPosition*[8];

	for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
	{
		m_Board[i] = new CPosition[8];
	}

	m_Board[0][0].SetUnit(new CRook(0, 0, BLACK));
	m_Board[0][1].SetUnit(new CKnight(0, 1, BLACK));
	m_Board[0][2].SetUnit(new CBishop(0, 2, BLACK));
	m_Board[0][3].SetUnit(new CQueen(0, 3, BLACK));
	m_Board[0][4].SetUnit(new CKing(0, 4, BLACK));
	m_Board[0][5].SetUnit(new CBishop(0, 5, BLACK));
	m_Board[0][6].SetUnit(new CKnight(0, 6, BLACK));
	m_Board[0][7].SetUnit(new CRook(0, 7, BLACK));
	m_Board[1][0].SetUnit(new CPawn(1, 0, BLACK));
	m_Board[1][1].SetUnit(new CPawn(1, 1, BLACK));
	m_Board[1][2].SetUnit(new CPawn(1, 2, BLACK));
	m_Board[1][3].SetUnit(new CPawn(1, 3, BLACK));
	m_Board[1][4].SetUnit(new CPawn(1, 4, BLACK));
	m_Board[1][5].SetUnit(new CPawn(1, 5, BLACK));
	m_Board[1][6].SetUnit(new CPawn(1, 6, BLACK));
	m_Board[1][7].SetUnit(new CPawn(1, 7, BLACK));

	m_Board[6][0].SetUnit(new CPawn(6, 0, WHITE));
	m_Board[6][1].SetUnit(new CPawn(6, 1, WHITE));
	m_Board[6][2].SetUnit(new CPawn(6, 2, WHITE));
	m_Board[6][3].SetUnit(new CPawn(6, 3, WHITE));
	m_Board[6][4].SetUnit(new CPawn(6, 4, WHITE));
	m_Board[6][5].SetUnit(new CPawn(6, 5, WHITE));
	m_Board[6][6].SetUnit(new CPawn(6, 6, WHITE));
	m_Board[6][7].SetUnit(new CPawn(6, 7, WHITE));
	m_Board[7][0].SetUnit(new CRook(7, 0, WHITE));
	m_Board[7][1].SetUnit(new CKnight(7, 1, WHITE));
	m_Board[7][2].SetUnit(new CBishop(7, 2, WHITE));
	m_Board[7][3].SetUnit(new CQueen(7, 3, WHITE));
	m_Board[7][4].SetUnit(new CKing(7, 4, WHITE));
	m_Board[7][5].SetUnit(new CBishop(7, 5, WHITE));
	m_Board[7][6].SetUnit(new CKnight(7, 6, WHITE));
	m_Board[7][7].SetUnit(new CRook(7, 7, WHITE));

}

void CNetworkChessDlg::IndexSetting()
{// 턴에 따라 선택할 수 있는 말들의 인덱스를 매겨줌
	int i, j;

	switch ( m_nTurn )
	{
		case WHITE_TURN ://백순서
		{
			for ( i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{//백말들 인덱스 값 매김
				for ( j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++ )
				{
					m_Board[i][j].DelIndex();//전단계 인덱스 삭제

					if ( ( m_Board[i][j].GetUnit() != NULL )&&( m_Board[i][j].GetUnit()->GetColor() == WHITE ) )
					{//백말에게만 인덱스 설정
						m_Board[i][j].SetIndex();

						if ( m_Board[i][j].GetUnit()->GetUnitType() == 'K' )
						{//킹의 체크메이트 여부 확인
							m_bCheckMateW = m_spRule.Attacked(i, j, WHITE, m_Board);
						}

					} else
					{
						if ( ( m_Board[i][j].GetUnit() != NULL )&&( m_Board[i][j].GetUnit()->GetColor() == BLACK ) )
						{//상대팀 킹의 체크메이트 여부 확인
							if ( m_Board[i][j].GetUnit()->GetUnitType() == 'K' )
							{
								m_bCheckMateB = m_spRule.Attacked(i, j, BLACK, m_Board);
							}
						}
					}
				}
			}

			break;
		}

		case BLACK_TURN ://흑순서
		{
			for ( i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{//흑말들 인덱스 값 매김
				for ( j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++ )
				{
					m_Board[i][j].DelIndex();//전단계 인덱스 삭제

					if ( ( m_Board[i][j].GetUnit() != NULL )&&( m_Board[i][j].GetUnit()->GetColor() == BLACK ) )
					{//흑말에게만 인덱스 설정
						m_Board[i][j].SetIndex();

						if ( m_Board[i][j].GetUnit()->GetUnitType() == 'K' )
						{//킹의 체크메이트 여부 확인
							m_bCheckMateB = m_spRule.Attacked(i, j, BLACK, m_Board);
						}

					} else
					{
						if ( ( m_Board[i][j].GetUnit() != NULL )&&( m_Board[i][j].GetUnit()->GetColor() == WHITE ) )
						{//상대팀 킹의 체크메이트 여부 확인
							if ( m_Board[i][j].GetUnit()->GetUnitType() == 'K' )
							{
								m_bCheckMateW = m_spRule.Attacked(i, j, WHITE, m_Board);
							}
						}
					}
				}
			}

			break;
		}
	}
}

void CNetworkChessDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// 마우스 클릭 위치의 좌표값을 체스판의 좌표로 바꿈
	int x = ( point.y - 12 ) / 50;
	int y = ( point.x - 11 ) / 50;

	// 상태창 비움
	
	
	// 해당 상황에 따라 좌표를 SelectUnit함수로 넘겨줘서 게임을 진행
	if ( m_LoginDlg.GetChInfo() == LOCAL_MODE )
	{
		SelectUnit(x, y);

	} else
	if ( ( m_LoginDlg.GetChInfo() == m_nTurn )&&( m_bConnect == true ) )
	{
		SelectUnit(x, y);
	}

	Invalidate();

	CDialog::OnLButtonDown(nFlags, point);
}

void CNetworkChessDlg::SelectUnit(int x, int y, int promotionData)
{
	int i, j;

	if ( ( x >= 0 )&&( x < 8 )&&( y >= 0 )&&(y < 8) )
	{//좌표값 입력(말판이 아닌곳 입력시 패스)
		switch ( m_nInputMode )
		{
			case SELECT_INPUT :
			{// 말 선택 모드
				m_nX = x;
				m_nY = y;

				if ( ( m_Board[m_nX][m_nY].GetUnit() == NULL ) ||
					( m_Board[m_nX][m_nY].GetUnit()->GetColor() != m_nTurn ) )
				{// 선택한곳에 말이 없을시 재입력 받음
					AfxMessageBox(_T("잘못 선택하셨습니다"));
					UpdateData(false);

					break;
				}

				SetMoveIndex();

				if ( m_bIndexOK == false )
				{// 선택말이 이동할수 없을시 재입력 받음
					AfxMessageBox(_T("선택한 말이 이동할 수 없습니다. 다시 선택해주세요"));
					UpdateData(false);
					IndexSetting();
					break;
				}

				if ( m_Board[m_nX][m_nY].GetUnit()->GetEnp() == false )
				{//앙파상 모드에서 다른말 이동시 앙파상 해제
					for ( i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
					{//앙파상 모드에 있는 말을 찾아냄
						for ( j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++ )
						{
							if ( ( m_Board[i][j].GetUnit() != NULL )&&
								( m_Board[i][j].GetUnit()->GetEnp() == true ) )
							{//앙파상 모드 해제
								m_Board[i][j].GetUnit()->DelEnp();
							}
						}
					}
				}
				
				m_nInputMode = MOVE_INPUT;

				break;
			}

			case MOVE_INPUT :
			{// 말 이동 모드
				m_nTargetX = x;
				m_nTargetY = y;
				if (m_Board[m_nX][m_nY].GetUnit() != m_Board[m_nTargetX][m_nTargetY].GetUnit()
					&&(m_Board[m_nTargetX][m_nTargetY].GetUnit()!=NULL)
					&&(m_Board[m_nX][m_nY].GetUnit()->GetColor()==m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetColor())) 
				{
					
					m_nInputMode = SELECT_INPUT;
					CNetworkChessDlg::SelectUnit(m_nTargetX, m_nTargetY, 0);
				}
				
				if(m_Board[m_nTargetX][m_nTargetY].GetIndex() == true)
				{
					if ( ( m_Board[m_nX][m_nY].GetUnit()->GetCast() == true )&&
						( m_Board[m_nX][m_nY].GetUnit()->GetUnitType() == 'K' )&&
						( ( m_nTargetY == m_nY + 2 ) || ( m_nTargetY == m_nY - 2 ) ) )
					{//캐슬링 이동
						if ( m_nTargetY == m_nY + 2 )
						{//오른쪽 캐슬링
							m_Board[m_nTargetX][m_nTargetY].SetUnit(m_Board[m_nX][m_nY].GetUnit());//킹이동
							m_Board[m_nX][m_nY].DelUnit();
							m_Board[m_nTargetX][m_nTargetY].GetUnit()->SetXY(m_nTargetX, m_nTargetY);
							m_Board[m_nTargetX][m_nTargetY].GetUnit()->DelCast();

							m_Board[m_nX][m_nY + 1].SetUnit(m_Board[m_nX][7].GetUnit());//룩이동
							m_Board[m_nX][RIGHT_ROOK_COL].DelUnit();
							m_Board[m_nX][m_nY + 1].GetUnit()->SetXY(m_nX, m_nY + 1);

						} else
						if ( m_nTargetY == m_nY - 2 )
						{//왼쪽 캐슬링
							m_Board[m_nTargetX][m_nTargetY].SetUnit(m_Board[m_nX][m_nY].GetUnit());//킹이동
							m_Board[m_nX][m_nY].DelUnit();
							m_Board[m_nTargetX][m_nTargetY].GetUnit()->SetXY(m_nTargetX, m_nTargetY);
							m_Board[m_nTargetX][m_nTargetY].GetUnit()->DelCast();

							m_Board[m_nX][m_nY - 1].SetUnit(m_Board[m_nX][0].GetUnit());//룩이동
							m_Board[m_nX][LEFT_ROOK_COL].DelUnit();
							m_Board[m_nX][m_nY - 1].GetUnit()->SetXY(m_nX, m_nY - 1);
						}

					} else
					if ( ( m_Board[m_nX][m_nY].GetUnit()->GetEnp() == true )&&
						( ( m_nTargetX == m_nX - 1 ) || ( m_nTargetX == m_nX + 1 ) )&&
						( ( m_nTargetY == m_nY - 1 ) || ( m_nTargetY == m_nY + 1 ) ) )
					{//앙파상 이동	
						if ( m_Board[m_nX][m_nY].GetUnit()->GetColor() == WHITE )
						{//흑말 앙파상
							delete m_Board[m_nTargetX + 1][m_nTargetY].GetUnit();//앙파상 당한 말 삭제
							m_Board[m_nTargetX + 1][m_nTargetY].DelUnit();
							m_Board[m_nTargetX][m_nTargetY].SetUnit(m_Board[m_nX][m_nY].GetUnit());//이동
							m_Board[m_nX][m_nY].DelUnit();
							m_Board[m_nTargetX][m_nTargetY].GetUnit()->SetXY(m_nTargetX, m_nTargetY);
							m_bEnpassantB = false;//앙파상은 게임중 한번만 가능

						} else
						{//백말 앙파상
							delete m_Board[m_nTargetX - 1][m_nTargetY].GetUnit();//앙파상 당한 말 삭제
							m_Board[m_nTargetX - 1][m_nTargetY].DelUnit();
							m_Board[m_nTargetX][m_nTargetY].SetUnit(m_Board[m_nX][m_nY].GetUnit());//이동
							m_Board[m_nX][m_nY].DelUnit();
							m_Board[m_nTargetX][m_nTargetY].GetUnit() -> SetXY(m_nTargetX, m_nTargetY);
							m_bEnpassantW = false;//앙파상은 게임중 한번만 가능
						}

						m_Board[m_nTargetX][m_nTargetY].GetUnit() -> DelEnp();//이동후 앙파상 해제

					}
					else
					{//일반적 경우 이동
						if (m_nX != m_nTargetX || m_nY != m_nTargetY) 
						{
							if (m_Board[m_nTargetX][m_nTargetY].GetUnit() != NULL)
							{//이동할곳 지움
								delete m_Board[m_nTargetX][m_nTargetY].GetUnit();
							}

							//test
							/*CString str;
							str.Format("%d %d %d %d", m_nX, m_nY, m_nTargetX, m_nTargetY);
							AfxMessageBox(str);*/

							m_Board[m_nTargetX][m_nTargetY].SetUnit(m_Board[m_nX][m_nY].GetUnit());//이동
							m_Board[m_nX][m_nY].DelUnit();
							m_Board[m_nTargetX][m_nTargetY].GetUnit()->SetXY(m_nTargetX, m_nTargetY);

							if ((m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetUnitType() == 'P') &&
								(m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetEnp() == true))
							{//앙파상 모드에서 일반적 이동일 경우 앙파상 모드 해제
								for (i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++)
								{//앙파상 모드에 있는 말을 찾아냄
									for (j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++)
									{
										if ((m_Board[i][j].GetUnit() != NULL) && (m_Board[i][j].GetUnit()->GetEnp() == true))
										{//앙파상 모드 해제
											m_Board[i][j].GetUnit()->DelEnp();
										}
									}
								}
							}
						}
					}

					if ( ( m_bEnpassantB != false )&&
						( m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetColor() == BLACK ) )
					{//흑말의 앙파상 여부 확인
						if ( ( ( m_nTargetX == WHITE_ENPASSANT_ROW ) ||
							( m_nTargetX == BLACK_ENPASSANT_ROW ) )&&
							( m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetUnitType() == 'P' )&&
							( m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetFirst() == true ) )
						{//앙파상 설정		
							m_spRule.Enpassant(m_nTargetX, m_nTargetY, m_Board);
						}

					} else
					if ( ( m_bEnpassantW != false )&&
						( m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetColor() == WHITE ) )
					{//백말의 앙파상 여부 확인
						if ( ( ( m_nTargetX == WHITE_ENPASSANT_ROW ) ||
							( m_nTargetX == BLACK_ENPASSANT_ROW ) )&&
							( m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetUnitType() == 'P' )&&
							( m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetFirst() == true ) )
						{//앙파상 설정	
							m_spRule.Enpassant(m_nTargetX, m_nTargetY, m_Board);
						}
					}

					m_spRule.Castling(m_Board);//캐슬링 여부 확인

					m_Board[m_nTargetX][m_nTargetY].GetUnit()->DelFirst();//처음 이동시 firstMove값을 false 로 바꿈

					m_nInputMode = SELECT_INPUT;
					if (m_nX != m_nTargetX || m_nY != m_nTargetY) {
						if (m_nTurn == BLACK_TURN)
						{

							m_nTurn = WHITE_TURN;
							p1 = AfxBeginThread(SpeakThreadWhiteTurn, this, THREAD_PRIORITY_LOWEST);

						}
						else if (m_nTurn == WHITE_TURN)
						{

							m_nTurn = BLACK_TURN;
							p1 = AfxBeginThread(SpeakThreadBlackTurn, this, THREAD_PRIORITY_LOWEST);

						}
					}
					delline = 1;
					//EndGame();// 게임 종료 확인
					
					DelIndex();
					
					if ( ( m_LoginDlg.GetChInfo() == LOCAL_MODE)
						|| ( m_nTurn == m_LoginDlg.GetChInfo() ) )
					{
						IndexSetting();
					}

					if ( ( m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetUnitType() == 'P' )&&
						( ( m_nTargetX == WHITE_PROMOTION_ROW ) ||
						( m_nTargetX == BLACK_PROMOTION_ROW ) ) )
					{//프로 모션 모드
						promotionData = m_spRule.ProMotion(m_nTargetX, m_nTargetY, m_Board, promotionData);
					}

					if ( ( m_LoginDlg.GetChInfo() != LOCAL_MODE )&&( m_LoginDlg.GetChInfo() != m_nTurn ) )
					{// 이동한 데이터를 상대방으로 보냄
						CData data;
						data.SetGameMode();
						data.SetX(m_nX);
						data.SetY(m_nY);
						data.SetTargetX(m_nTargetX);
						data.SetTargetY(m_nTargetY);
						data.SetPromotionData(promotionData);
						*m_GameSock << data;
					}
					
					m_nRemainTime = DEFAULT_REMAIN_TIME;
					KillTimer(0);
					TimerBegin();
					
					m_nOldX = m_nX;
					m_nOldY = m_nY;
					m_nOldTargetX = m_nTargetX;
					m_nOldTargetY = m_nTargetY;
				}
			}
		}
	}
	EndGame();
	Invalidate();
}

LRESULT CNetworkChessDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{// 데이터를 받을시 소켓에서 보내오는 유저메세지를 핸들링하는 함수
	CData data;
	*m_GameSock >> data;

	switch ( data.GetSockMode() )
	{
		case USER_MODE :
		{// 처음 연결시 발생하며 유저 이름을 교환
			if ( m_LoginDlg.GetChInfo() == SERVER_MODE )
			{
				m_strBlackUser = data.GetStrData();

			} else
			{
				m_strWhiteUser = data.GetStrData();
				data.SetUserMode();
				data.SetStrData(m_LoginDlg.GetUserName());
				*m_GameSock << data;
			}

			UpdateData(false);

			break;
		}

		case GAME_MODE :
		{// 게임진행시 유닛의 이동 내역을 받아 상대방의 유닛을 움직임
			SelectUnit(data.GetX(), data.GetY());
			SelectUnit(data.GetTargetX(), data.GetTargetY(), data.GetPromotionData());
			EndGame();

			m_nOldX = data.GetX();
			m_nOldY = data.GetY();
			m_nOldTargetX = data.GetTargetX();
			m_nOldTargetY = data.GetTargetY();


			break;
		}

		case CHAT_MODE :
		{// 상대방이 채팅창을 통해 입력시 발생
			// 수신된 데이터 채팅창 표시
			CString strTemp;
			if ( m_LoginDlg.GetChInfo() == SERVER_MODE )
			{
				strTemp = m_strBlackUser + " > " + data.GetStrData();

			} else
			{
				strTemp = m_strWhiteUser + " > " + data.GetStrData();
			}

			m_ctrChatList.AddString(strTemp);
			m_ctrChatList.SetCurSel(m_ctrChatList.GetCount()-1);
			m_ctrChatList.SetCurSel(-1);
			
			NetDlg.temp_chat = strTemp;

			p1 = AfxBeginThread(SpeakThreadChat, this, THREAD_PRIORITY_LOWEST);
			
			break;
		}

		case RENEW_MODE :
		{// 게임의 재시작을 요청시 발생
			data.SetRenewAckMode();// 재시작 요청에 대한 답변 모드

			if ( AfxMessageBox(_T("재시작 요청이 들어왔습니다!! 재시작하시겠습니까?"), MB_YESNO, 0) == IDYES )
			{
				data.SetAckTrue();
				for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
				{//메모리 해제
					delete[] m_Board[i];
				}
				delete[] m_Board;
				m_Board = NULL;

				SetBoard();
				m_nTurn = WHITE_TURN;

				delline = 0;
				Invalidate();
				if(m_LoginDlg.GetUserName()==m_strWhiteUser)
				IndexSetting();

			} else
			{
				data.SetAckFalse();
			}

			*m_GameSock << data;

			break;
		}

		case RENEW_ACK_MODE :
		{// 재시작의 대한 답변을 통해 재시작 여부 결정
			if ( data.GetAck() == RENEW_YES )
			{
				for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
				{//메모리 해제
					delete[] m_Board[i];
				}
				delete[] m_Board;
				m_Board = NULL;

				SetBoard();

				m_nTurn = WHITE_TURN;
				delline = 0;
				
				if (m_LoginDlg.GetUserName() == m_strWhiteUser)
				IndexSetting();
			}

			break;
		}
		
		case TIME_OVER_MODE :
		{
			KillTimer(0);

			m_strStatusView = "시간이 지나서 턴이 넘어왔습니다!!";
			UpdateData(false);
					
			if ( m_nTurn == WHITE_TURN )
			{
				m_nTurn = BLACK_TURN;

				p1 = AfxBeginThread(SpeakThreadBlackTurn, this, THREAD_PRIORITY_LOWEST);

				
			} else
			{
				m_nTurn = WHITE_TURN;

				p1 = AfxBeginThread(SpeakThreadWhiteTurn, this, THREAD_PRIORITY_LOWEST);
				

			}

			m_nInputMode = SELECT_INPUT;
			
			m_nRemainTime = DEFAULT_REMAIN_TIME;

			TimerBegin();

			DelIndex();
			IndexSetting();

			Invalidate();

			break;
		}

		case CLOSE_MODE :
		case EXIT_MODE :
		{// 상대방이 연결 해제시 발생 
			KillTimer(0);
			
			CString strTemp = "연결이 해제되었습니다";
			
			m_ctrChatList.AddString(strTemp);
			
			NetDlg.temp_chat = strTemp;

			p1 = AfxBeginThread(SpeakThreadChat, this, THREAD_PRIORITY_LOWEST);
			

			
			for( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{
				delete[] m_Board[i];
			}
			delete[] m_Board;
			m_Board = NULL;

			SetBoard();
			m_nTurn = WHITE_TURN;

			if ( m_ServerSock != NULL )
			{
				delete m_ServerSock;
				m_ServerSock = NULL;
			}

			if ( m_GameSock != NULL )
			{
				delete m_GameSock;
				m_GameSock = NULL;
			}

			m_bConnect = false;

			ViewLoginDlg();
			
			break;
		}
	}
	Invalidate();

	return 0L;
}

LRESULT CNetworkChessDlg::OnAccept(WPARAM wParam, LPARAM lParam)
{// 클라이언트의 접속을 받아들이는 메세지 핸들러 함수
	CData data;
	m_GameSock = new CGameSocket();
	// 접속 요청을 받아들임
	if ( !m_ServerSock->Accept(*m_GameSock) )
	{
		AfxMessageBox(_T("Accept를 실패 하였습니다."));

		return 0L;
	}
	
	GetDlgItem(IDC_CHAT_INPUT)->EnableWindow(true);

	m_nTurn = WHITE_TURN;
	IndexSetting();

	TimerBegin();

	// 데이터 소켓 초기화
	m_GameSock->Init(this);
	m_bConnect = true;

	m_strStatusView = "";
	UpdateData(false);

	// 유저 아이디 보냄
	data.SetUserMode();
	data.SetStrData(m_LoginDlg.GetUserName());
	*m_GameSock << data;
	CString strTemp = _T("연결되었습니다");
	m_ctrChatList.AddString(strTemp);
	
	NetDlg.temp_chat = strTemp;

	p1 = AfxBeginThread(SpeakThreadChat, this, THREAD_PRIORITY_LOWEST);

	
	return 0L;
}

LRESULT CNetworkChessDlg::OnCloseFromLogin(WPARAM wParam, LPARAM lParam)
{// 로그인 대화창에서 프로그램 종료 메세지 호출시 메인 대화상자 종료
	CDialog::OnOK();

	return 0L;
}

void CNetworkChessDlg::OnEnterkey() 
{// 채팅 입력창의 엔터키에 대한 이벤트 처리
	UpdateData();

	if ( m_LoginDlg.GetChInfo() == LOCAL_MODE )
	{
		m_ctrChatList.AddString(m_strChatInput);
		NetDlg.temp_chat = m_strChatInput;
	} else
	{
		// 데이터 전송
		CData data;
		data.SetChatMode();
		m_strChatInput.Append(NULL);
		data.SetStrData(m_strChatInput);
		*m_GameSock << data;
		// 전송된 문자열을 상단 윈도우에도 표시
		CString strTemp;

		if ( m_LoginDlg.GetChInfo() == SERVER_MODE )
		{
			strTemp = m_strWhiteUser + " > " + m_strChatInput;

		} else
		{
			strTemp = m_strBlackUser + " > " + m_strChatInput;
		}

		m_ctrChatList.AddString(strTemp);
		NetDlg.temp_chat = strTemp;
	}
	m_ctrChatList.SetCurSel(m_ctrChatList.GetCount() + 1);
	m_ctrChatList.SetCurSel(-1);
	

	p1 = AfxBeginThread(SpeakThreadChat, this, THREAD_PRIORITY_LOWEST);


	// Edit 컨트롤 초기화
	m_strChatInput.Empty();

	UpdateData(FALSE);
}

void CNetworkChessDlg::OnOK()
{// 엔터키의 비정상 종료를 막기 위한 함수
	OnEnterkey();
}

void CNetworkChessDlg::OnExitButton() 
{// 종료 버튼 눌렀을 경우 발생
	KillTimer(0);

	if ( AfxMessageBox(_T("종료 하시겠습니까?"), MB_YESNO) == IDYES )
	{// 종료 여부를 물어봄
		if ( ( m_LoginDlg.GetChInfo() != LOCAL_MODE )&&( m_bConnect == true ) )
		{// 상대방에게 자신이 종료한다고 알려줌
			CData data;
			data.SetExitMode();
			*m_GameSock << data;
		}

		if ( m_Board != NULL )
		{
			for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{//메모리 해제
				delete[] m_Board[i];
			}
			delete[] m_Board;
			m_Board = NULL;
		}

		if ( m_ServerSock != NULL )
		{
			delete m_ServerSock;
			m_ServerSock = NULL;
		}

		if ( m_GameSock != NULL )
		{
			delete m_GameSock;
			m_GameSock = NULL;
		}

		CDialog::OnOK();
	}
}

void CNetworkChessDlg::OnRenewButton() 
{// 재시작 버튼을 눌렀을 경우 발생
	if ( m_LoginDlg.GetChInfo() == LOCAL_MODE )
	{
		if ( m_Board != NULL )
		{
			for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{//메모리 해제
				delete[] m_Board[i];
			}
			delete[] m_Board;
			m_Board = NULL;
		}

		SetBoard();
		m_nTurn = WHITE_TURN;
		if (m_LoginDlg.GetUserName() == m_strWhiteUser)
			IndexSetting();
		delline = 0;
		
		Invalidate();

	} else
	{
		CData data;
		data.SetRenewMode();
		*m_GameSock << data;
	}
}

void CNetworkChessDlg::OnCloseLinkButton() 
{// 연결 해제시 발생
	if ( AfxMessageBox(_T("연결을 해제하시겠습니까?"), MB_YESNO) == IDYES )
	{// 여부를 확인
		KillTimer(0);

		if ( ( m_LoginDlg.GetChInfo() != LOCAL_MODE )&&( m_bConnect == true ) )
		{// 상대방에게 연결 해제한다고 알려줌
			CData data;
			data.SetCloseMode();
			*m_GameSock << data;
		}

		if ( m_ServerSock != NULL )
		{
			delete m_ServerSock;
			m_ServerSock = NULL;
		}

		if ( m_GameSock != NULL )
		{
			delete m_GameSock;
			m_GameSock = NULL;
		}

		m_bConnect = false;

		CString strTemp = "연결이 해제되었습니다";
		m_ctrChatList.AddString(strTemp);
		NetDlg.temp_chat = strTemp;
		
		if ( m_Board != NULL )
		{
			for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{//메모리 해제
				delete[] m_Board[i];
			}
			delete[] m_Board;
			m_Board = NULL;
		}
		
		SetBoard();

		ViewLoginDlg();
	}

	p1 = AfxBeginThread(SpeakThreadChat, this, THREAD_PRIORITY_LOWEST);
}

void CNetworkChessDlg::OnGameInfoButton() 
{// 게임 정보 대화상자를 보여줌
	CAboutDlg aboutDlg;

	aboutDlg.DoModal();
}

BOOL CNetworkChessDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( ( pMsg->message==WM_KEYDOWN )&&( pMsg->wParam==VK_ESCAPE ) )
	{// ESC키를 잡아서 비정상 종료 안하도록 잡아줌
		return TRUE;
	}

	if (pMsg->message == WM_RBUTTONDOWN)
	{
		CRect rect;
		this->GetDlgItem(IDC_EXIT_BUTTON)->GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt)) {
			p1 = AfxBeginThread(SpeakThreadEXITButton, this, THREAD_PRIORITY_LOWEST);

		}
		this->GetDlgItem(IDC_CLOSE_LINK_BUTTON)->GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt)) {
			p1 = AfxBeginThread(SpeakThreadCLINKButton, this, THREAD_PRIORITY_LOWEST);

		}
		this->GetDlgItem(IDC_RENEW_BUTTON)->GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt)) {
			p1 = AfxBeginThread(SpeakThreadRENEWButton, this, THREAD_PRIORITY_LOWEST);

		}
		this->GetDlgItem(IDC_GAME_INFO_BUTTON)->GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt)) {
			p1 = AfxBeginThread(SpeakThreadINFOButton, this, THREAD_PRIORITY_LOWEST);

		}
		this->GetDlgItem(IDC_CHAT_INPUT)->GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt)) {
			p1 = AfxBeginThread(SpeakThreadChatInput, this, THREAD_PRIORITY_LOWEST);

		}

	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CNetworkChessDlg::OnClose() 
{// X버튼을 통해 종료시 발생
	if ( AfxMessageBox(_T("종료 하시겠습니까?"), MB_YESNO) == IDYES )
	{// 종료 여부 확인
		KillTimer(0);
		
		if ( ( m_LoginDlg.GetChInfo() != LOCAL_MODE )&&( m_bConnect == true ) )
		{// 상대방에게 종료 한다고 알려줌
			CData data;
			data.SetExitMode();
			*m_GameSock << data;
		}

		if ( m_Board != NULL )
		{
			for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{//메모리 해제
				delete[] m_Board[i];
			}
			delete[] m_Board;
			m_Board = NULL;
		}

		if ( m_ServerSock != NULL )
		{
			delete m_ServerSock;
			m_ServerSock = NULL;
		}

		if ( m_GameSock != NULL )
		{
			delete m_GameSock;
			m_GameSock = NULL;
		}

		CDialog::OnClose();
	}
}

void CNetworkChessDlg::ViewLoginDlg()
{// 로그인 대화상자를 띄워서 채널을 다시 맞추고 연결을 재설정 함
	bool m_bTurn = true;

	while ( m_bTurn )
	{
		m_LoginDlg.DoModal();// 로그인 대화상자 호출
		m_bTurn = false;

		switch ( m_LoginDlg.GetChInfo())
		{
			case SERVER_MODE :
			{// 서버 모드일 경우
				GetDlgItem(IDC_CHAT_INPUT)->EnableWindow(false);
				// 서버 소켓을 생성
				m_ServerSock = new CServerSocket();
				// Init을 통해 포트 넘버를 바인드하고 리슨대기함
				m_ServerSock->Init(this, PORT_NUM);
				m_strStatusView = "클라이언트의 접속을 기다림!!";
				m_strWhiteUser = m_LoginDlg.GetUserName();
				UpdateData(false);

				break;
			}

			case CLIENT_MODE :
			{// 클라이언트 모드일 경우
				// 게임용 소켓을 생성
				GetDlgItem(IDC_CHAT_INPUT)->EnableWindow(true);
				m_GameSock = new CGameSocket();

				if( !m_GameSock->Create() ) 
				{// 소켓 생성
					AfxMessageBox(_T("소켓 생성 실패!!"));

					if ( m_GameSock != NULL )
					{
						delete m_GameSock;
						m_GameSock = NULL;
					}

					m_bTurn = true;
				}

				if( !m_GameSock->Connect(m_LoginDlg.GetIpAddr(), PORT_NUM) )
				{// 서버에 연결 시도
					AfxMessageBox(_T("접속 실패!!"));

					if ( m_GameSock != NULL )
					{
						delete m_GameSock;
						m_GameSock = NULL;
					}

					// 접속 실패시 다시 로그인 창을 띄움
					m_bTurn = true;

				} else
				{// 연결 성공시
					m_strBlackUser = m_LoginDlg.GetUserName();
					m_ctrChatList.AddString(_T("연결되었습니다"));
					
					NetDlg.temp_chat = _T("연결되었습니다");

					p1 = AfxBeginThread(SpeakThreadChat, this, THREAD_PRIORITY_LOWEST);
					
					m_bConnect = true;
					m_GameSock->Init(this);
					m_nTurn = WHITE_TURN;
					TimerBegin();
				}

				break;
			}

			case LOCAL_MODE :
			{// 로컬 모드일 경우 바로 인덱스를 잡아줌
				GetDlgItem(IDC_CHAT_INPUT)->EnableWindow(true);
				TimerBegin();
				
				IndexSetting();

				break;
			}
		}
	}
}

void CNetworkChessDlg::DelIndex()
{
	for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
	{//백말들 인덱스 값 매김
		for ( int j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++ )
		{
			m_Board[i][j].DelIndex();//전단계 인덱스 삭제
		}
	}
}

void CNetworkChessDlg::TimerBegin()
{
	m_nRemainTime = DEFAULT_REMAIN_TIME;
	SetTimer(0, 1000, NULL);
}

void CNetworkChessDlg::OnTimer(UINT nIDEvent) 
{
	m_strCountDown = _T("");
	if (m_nRemainTime == DEFAULT_REMAIN_TIME) {
		p1 = AfxBeginThread(SpeakThreadDefaultTime, this, THREAD_PRIORITY_LOWEST);
	}
	else if (m_nRemainTime == 30) {
		p1 = AfxBeginThread(SpeakThreadTime30, this, THREAD_PRIORITY_LOWEST);
	}
	else if (m_nRemainTime == 10) {
		p1 = AfxBeginThread(SpeakThreadTime10, this, THREAD_PRIORITY_LOWEST);
	}
	else if (m_nRemainTime == 0) {
		if (m_nTurn == WHITE_TURN)
		{
			p1 = AfxBeginThread(SpeakThreadBlackTurn, this, THREAD_PRIORITY_LOWEST);
		}
		else
		{
			p1 = AfxBeginThread(SpeakThreadWhiteTurn, this, THREAD_PRIORITY_LOWEST);
		}
	}

	m_strCountDown.Format("%d", m_nRemainTime--);
	SetDlgItemText(ID_COUNTDOWN,m_strCountDown);
	if ( m_nRemainTime < 0 )
	{
		switch ( m_LoginDlg.GetChInfo() )
		{
			case SERVER_MODE :
			case CLIENT_MODE :
			{
				if ( m_LoginDlg.GetChInfo() == m_nTurn )
				{
					if ( m_nTurn == WHITE_TURN )
					{
						m_nTurn = BLACK_TURN;
						
					} else
					{
						m_nTurn = WHITE_TURN;
					}

					m_nInputMode = SELECT_INPUT;

					CData data;
					data.SetTimeOverMode();
					*m_GameSock << data;

					m_strStatusView = "시간이 지나서 상대방으로 턴이 넘어갑니다!!";
					UpdateData(false);
				
					m_nRemainTime = DEFAULT_REMAIN_TIME;

					KillTimer(0);
					TimerBegin();
					
					DelIndex();
					Invalidate();
				}

				break;
			}

			case LOCAL_MODE:
			{

				m_strStatusView = "시간이 지나서 상대방으로 턴이 넘어갑니다!!";
				UpdateData(false);
				
				if ( m_nTurn == WHITE_TURN )
				{
					m_nTurn = BLACK_TURN;
					
				} else
				{
					m_nTurn = WHITE_TURN;
				}
				
				m_nRemainTime = DEFAULT_REMAIN_TIME;
				
				DelIndex();
				IndexSetting();

				KillTimer(0);
				TimerBegin();

				Invalidate();
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}


void CNetworkChessDlg::speak(wchar_t* string)
{
	CoInitialize(NULL);
	CComPtr<ISpVoice> p_voice_engine;
	p_voice_engine.CoCreateInstance(CLSID_SpVoice);
	p_voice_engine->Speak(string, 0, NULL);
	p_voice_engine.Release();
	CoUninitialize();
}

UINT CNetworkChessDlg::SpeakThreadClient(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"게임영역");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadBoard(LPVOID _mothod)
{
	USES_CONVERSION;
	CString str;
	wchar_t* w_str;
	str.Format(_T("%d %d"), NetDlg.b_x, NetDlg.b_y);
	w_str = T2W(str.GetBuffer(0));
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(w_str);
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadBoardUnit(LPVOID _mothod)
{
	USES_CONVERSION;
	CString str = _T("");
	CString pos_str;
	CString turn_str;
	CString unit_str;
	wchar_t* w_str;

	switch (NetDlg.b_color) {
	case BLACK:
		turn_str = _T("흑");
		break;
	case WHITE:
		turn_str = _T("백");
		break;
	}

	switch (NetDlg.b_unit)
	{
	case 'K':
	{
		unit_str = _T("킹");
		break;
	}

	case 'Q':
	{
		unit_str = _T("퀸");
		break;
	}

	case 'R':
	{
		unit_str = _T("룩");
		break;
	}

	case 'B':
	{
		unit_str = _T("비숍");
		break;
	}

	case 'N':
	{
		unit_str = _T("나이트");
		break;
	}

	case 'P':
	{
		unit_str = _T("폰");
		break;
	}
	}

	pos_str.Format(_T(" %d %d"), NetDlg.b_x, NetDlg.b_y);

	str.Append(turn_str);
	str.Append(unit_str);
	str.Append(pos_str);
	w_str = T2W(str.GetBuffer(0));

	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(w_str);
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadEXITButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"종료버튼");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadCLINKButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"접속해제버튼");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadRENEWButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"재시작버튼");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadINFOButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"게임정보버튼");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadDefaultTime(LPVOID _mothod)
{
	USES_CONVERSION;
	CString time_str;
	wchar_t* w_str;
	time_str.Format("%d초남았습니다", DEFAULT_REMAIN_TIME);
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	w_str = T2W(time_str.GetBuffer(0));
	pDlg->speak(w_str);
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadTime30(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"30초남았습니다");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadTime10(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"10초남았습니다");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadBlackTurn(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"흑턴");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadWhiteTurn(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"백턴");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadChatInput(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"채팅입력칸");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadChat(LPVOID _mothod)
{
	USES_CONVERSION;
	CString chat_str = NetDlg.temp_chat;
	wchar_t* w_str;
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	w_str = T2W(chat_str.GetBuffer(0));
	pDlg->speak(w_str);
	return 0;
}

void CNetworkChessDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// 마우스 클릭 위치의 좌표값을 체스판의 좌표로 바꿈
	NetDlg.b_x = (point.y - 12) / 50;
	NetDlg.b_y = (point.x - 11) / 50;

	if ((NetDlg.b_x >= 0) && (NetDlg.b_x < 8) && (NetDlg.b_y >= 0) && (NetDlg.b_y < 8)) { //체스판 내부
		if (m_Board[NetDlg.b_x][NetDlg.b_y].GetUnit() == NULL)
		{ //선택한곳에 말이 없을시
			//좌표만 말해주기
			p1 = AfxBeginThread(SpeakThreadBoard, this, THREAD_PRIORITY_LOWEST);

		}
		else if(m_Board[NetDlg.b_x][NetDlg.b_y].GetUnit() != NULL) 
		{ //선택한곳에 말이 있을시
			//말+좌표 말해주기
			NetDlg.b_color = m_Board[NetDlg.b_x][NetDlg.b_y].GetUnit()->GetColor();
			NetDlg.b_unit = m_Board[NetDlg.b_x][NetDlg.b_y].GetUnit()->GetUnitType();
			p1 = AfxBeginThread(SpeakThreadBoardUnit, this, THREAD_PRIORITY_LOWEST);

		}
	}
	else { //체스판 바깥
		p1 = AfxBeginThread(SpeakThreadClient, this, THREAD_PRIORITY_LOWEST);
	}

	CDialog::OnRButtonDown(nFlags, point);
}

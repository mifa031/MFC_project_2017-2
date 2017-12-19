
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
	

	// ��� ������ �ʱ�ȭ
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
	m_nTurn = WHITE_TURN;//�⺻���� �鸻 ����
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

	
	ViewLoginDlg();// �α��� â�� DoModal�� �����

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

	// ��� �׸�
	dc.BitBlt(0, 0, 641, 423, &dcBoardMem, 0, 0, SRCCOPY);

	// ó�� ��ġ 12, 11
	for ( int i = 0; i < 8; i++ )
	{
		for ( int j = 0; j < 8; j++ )
		{
			if ( ( i + j ) % 2 )
			{// �Ͼ���
				if ( m_Board[i][j].GetIndex() == true )
				{//�ε����� ������� �ε��� ���
					dcBoardMem.SelectObject(&bitmapSelWhite);
					dc.BitBlt(( 50 * j ) + 12, ( 50 * i ) + 12, 50, 50, &dcBoardMem, 0, 0, SRCCOPY);

				} else
				{
					dcBoardMem.SelectObject(&bitmapWhite);
					dc.BitBlt(( 50 * j ) + 12, ( 50 * i ) + 12, 50, 50, &dcBoardMem, 0, 0, SRCCOPY);
				}

			} else
			{// ������
				if ( m_Board[i][j].GetIndex() == true )
				{//�ε����� ������� �ε��� ���
					dcBoardMem.SelectObject(&bitmapSelBlack);
					dc.BitBlt(( 50 * j ) + 12, ( 50 * i ) + 12, 50, 50, &dcBoardMem, 0, 0, SRCCOPY);

				} else
				{
					dcBoardMem.SelectObject(&bitmapBlack);
					dc.BitBlt(( 50 * j ) + 12, ( 50 * i ) + 12, 50, 50, &dcBoardMem, 0, 0, SRCCOPY);
				}
			}

			if ( m_Board[i][j].GetUnit() != NULL )
			{// ������ ���� ��� ���ֿ� ���� �ش� ������ �׷���
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
				{// ���Ļ� ��� ǥ��
					dcMem.SelectObject(&bitmapEP);
					dc.BitBlt(( 50 * j ) + 12, ( 50 * i ) + 12, 9, 5, &dcMem, 0, 0, SRCCOPY);
				}

				if ( m_Board[i][j].GetUnit()->GetCast() == true )
				{// ĳ���� ��� ǥ��
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
		m_strStatusView.AppendFormat("���� �����Դϴ�                          ");
		UpdateData(FALSE);

	}
	else
		if (m_nTurn == WHITE_TURN)
		{
			m_strStatusView = "";
			UpdateData(false);
			m_strStatusView.AppendFormat("���� �����Դϴ�                          ");
			UpdateData(FALSE);
		}
	if (m_bCheckMateW == true)
	{
		

		m_strStatusView.AppendFormat("���� ŷ�� üũ�Դϴ�");
		UpdateData(false);

	}
	if (m_bCheckMateB == true)
	{
	
		m_strStatusView.AppendFormat("���� ŷ�� üũ�Դϴ�");
		UpdateData(false);

	}
}


HCURSOR CNetworkChessDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNetworkChessDlg::SetMoveIndex()
{// ���� �̵��� ���� �ε����� �Ű��ִ� �Լ�
	m_bIndexOK = false;
		
	for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
	{//���� ���� ���� �̵� �ε��� �� ���� �ű�
		for ( int j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++ )
		{
			m_Board[i][j].DelIndex();//���ܰ� �ε��� ����

			if ( ( m_Board[m_nX][m_nY].GetUnit() != NULL )&&
				( m_Board[m_nX][m_nY].GetUnit()->Move(i, j, m_Board) == true ) )
			{
				m_Board[i][j].SetIndex();
				m_bIndexOK = true;//�ε��� �Ű��� ������ �ִ��� Ȯ��
			}
		}
	}
}

void CNetworkChessDlg::EndGame()
{// ���� ����� �߻�
	
	int iGameOver = 0;

	for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
	{//������ ����
		for ( int j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++ )
		{
			if ( ( m_Board[i][j].GetUnit() != NULL )&&( m_Board[i][j].GetUnit()->GetUnitType() == 'K' ) )
			{//ŷ�� ã��
				if ( m_Board[i][j].GetUnit()->GetColor() == BLACK )
				{//��ŷ�� ������ 1����
					iGameOver++;

				} else
				{//��ŷ�� ������ 3����
					iGameOver += 3;
				}
			}//if ( ( m_Board[i][j].GetUnit()->GetUnitType() == 'K' )
		}
	}

	switch ( iGameOver )
	{
		case 1 :
		{//��ŷ�� �ִ� ���
			AfxMessageBox(_T("������ �̰���ϴ�!!"));

			for (int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{//�޸� ����
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
		{//��ŷ�� �ִ°��
			AfxMessageBox(_T("������ �̰���ϴ�!!"));

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
		{//4�ΰ��.. �ΰ��� ŷ�� �� �����ϹǷ� ���� ���
			break;
		}
	}//switch(endGame)

}

void CNetworkChessDlg::SetBoard()
{// ü������ ����
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
{// �Ͽ� ���� ������ �� �ִ� ������ �ε����� �Ű���
	int i, j;

	switch ( m_nTurn )
	{
		case WHITE_TURN ://�����
		{
			for ( i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{//�鸻�� �ε��� �� �ű�
				for ( j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++ )
				{
					m_Board[i][j].DelIndex();//���ܰ� �ε��� ����

					if ( ( m_Board[i][j].GetUnit() != NULL )&&( m_Board[i][j].GetUnit()->GetColor() == WHITE ) )
					{//�鸻���Ը� �ε��� ����
						m_Board[i][j].SetIndex();

						if ( m_Board[i][j].GetUnit()->GetUnitType() == 'K' )
						{//ŷ�� üũ����Ʈ ���� Ȯ��
							m_bCheckMateW = m_spRule.Attacked(i, j, WHITE, m_Board);
						}

					} else
					{
						if ( ( m_Board[i][j].GetUnit() != NULL )&&( m_Board[i][j].GetUnit()->GetColor() == BLACK ) )
						{//����� ŷ�� üũ����Ʈ ���� Ȯ��
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

		case BLACK_TURN ://�����
		{
			for ( i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{//�渻�� �ε��� �� �ű�
				for ( j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++ )
				{
					m_Board[i][j].DelIndex();//���ܰ� �ε��� ����

					if ( ( m_Board[i][j].GetUnit() != NULL )&&( m_Board[i][j].GetUnit()->GetColor() == BLACK ) )
					{//�渻���Ը� �ε��� ����
						m_Board[i][j].SetIndex();

						if ( m_Board[i][j].GetUnit()->GetUnitType() == 'K' )
						{//ŷ�� üũ����Ʈ ���� Ȯ��
							m_bCheckMateB = m_spRule.Attacked(i, j, BLACK, m_Board);
						}

					} else
					{
						if ( ( m_Board[i][j].GetUnit() != NULL )&&( m_Board[i][j].GetUnit()->GetColor() == WHITE ) )
						{//����� ŷ�� üũ����Ʈ ���� Ȯ��
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
	// ���콺 Ŭ�� ��ġ�� ��ǥ���� ü������ ��ǥ�� �ٲ�
	int x = ( point.y - 12 ) / 50;
	int y = ( point.x - 11 ) / 50;

	// ����â ���
	
	
	// �ش� ��Ȳ�� ���� ��ǥ�� SelectUnit�Լ��� �Ѱ��༭ ������ ����
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
	{//��ǥ�� �Է�(������ �ƴѰ� �Է½� �н�)
		switch ( m_nInputMode )
		{
			case SELECT_INPUT :
			{// �� ���� ���
				m_nX = x;
				m_nY = y;

				if ( ( m_Board[m_nX][m_nY].GetUnit() == NULL ) ||
					( m_Board[m_nX][m_nY].GetUnit()->GetColor() != m_nTurn ) )
				{// �����Ѱ��� ���� ������ ���Է� ����
					AfxMessageBox(_T("�߸� �����ϼ̽��ϴ�"));
					UpdateData(false);

					break;
				}

				SetMoveIndex();

				if ( m_bIndexOK == false )
				{// ���ø��� �̵��Ҽ� ������ ���Է� ����
					AfxMessageBox(_T("������ ���� �̵��� �� �����ϴ�. �ٽ� �������ּ���"));
					UpdateData(false);
					IndexSetting();
					break;
				}

				if ( m_Board[m_nX][m_nY].GetUnit()->GetEnp() == false )
				{//���Ļ� ��忡�� �ٸ��� �̵��� ���Ļ� ����
					for ( i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
					{//���Ļ� ��忡 �ִ� ���� ã�Ƴ�
						for ( j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++ )
						{
							if ( ( m_Board[i][j].GetUnit() != NULL )&&
								( m_Board[i][j].GetUnit()->GetEnp() == true ) )
							{//���Ļ� ��� ����
								m_Board[i][j].GetUnit()->DelEnp();
							}
						}
					}
				}
				
				m_nInputMode = MOVE_INPUT;

				break;
			}

			case MOVE_INPUT :
			{// �� �̵� ���
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
					{//ĳ���� �̵�
						if ( m_nTargetY == m_nY + 2 )
						{//������ ĳ����
							m_Board[m_nTargetX][m_nTargetY].SetUnit(m_Board[m_nX][m_nY].GetUnit());//ŷ�̵�
							m_Board[m_nX][m_nY].DelUnit();
							m_Board[m_nTargetX][m_nTargetY].GetUnit()->SetXY(m_nTargetX, m_nTargetY);
							m_Board[m_nTargetX][m_nTargetY].GetUnit()->DelCast();

							m_Board[m_nX][m_nY + 1].SetUnit(m_Board[m_nX][7].GetUnit());//���̵�
							m_Board[m_nX][RIGHT_ROOK_COL].DelUnit();
							m_Board[m_nX][m_nY + 1].GetUnit()->SetXY(m_nX, m_nY + 1);

						} else
						if ( m_nTargetY == m_nY - 2 )
						{//���� ĳ����
							m_Board[m_nTargetX][m_nTargetY].SetUnit(m_Board[m_nX][m_nY].GetUnit());//ŷ�̵�
							m_Board[m_nX][m_nY].DelUnit();
							m_Board[m_nTargetX][m_nTargetY].GetUnit()->SetXY(m_nTargetX, m_nTargetY);
							m_Board[m_nTargetX][m_nTargetY].GetUnit()->DelCast();

							m_Board[m_nX][m_nY - 1].SetUnit(m_Board[m_nX][0].GetUnit());//���̵�
							m_Board[m_nX][LEFT_ROOK_COL].DelUnit();
							m_Board[m_nX][m_nY - 1].GetUnit()->SetXY(m_nX, m_nY - 1);
						}

					} else
					if ( ( m_Board[m_nX][m_nY].GetUnit()->GetEnp() == true )&&
						( ( m_nTargetX == m_nX - 1 ) || ( m_nTargetX == m_nX + 1 ) )&&
						( ( m_nTargetY == m_nY - 1 ) || ( m_nTargetY == m_nY + 1 ) ) )
					{//���Ļ� �̵�	
						if ( m_Board[m_nX][m_nY].GetUnit()->GetColor() == WHITE )
						{//�渻 ���Ļ�
							delete m_Board[m_nTargetX + 1][m_nTargetY].GetUnit();//���Ļ� ���� �� ����
							m_Board[m_nTargetX + 1][m_nTargetY].DelUnit();
							m_Board[m_nTargetX][m_nTargetY].SetUnit(m_Board[m_nX][m_nY].GetUnit());//�̵�
							m_Board[m_nX][m_nY].DelUnit();
							m_Board[m_nTargetX][m_nTargetY].GetUnit()->SetXY(m_nTargetX, m_nTargetY);
							m_bEnpassantB = false;//���Ļ��� ������ �ѹ��� ����

						} else
						{//�鸻 ���Ļ�
							delete m_Board[m_nTargetX - 1][m_nTargetY].GetUnit();//���Ļ� ���� �� ����
							m_Board[m_nTargetX - 1][m_nTargetY].DelUnit();
							m_Board[m_nTargetX][m_nTargetY].SetUnit(m_Board[m_nX][m_nY].GetUnit());//�̵�
							m_Board[m_nX][m_nY].DelUnit();
							m_Board[m_nTargetX][m_nTargetY].GetUnit() -> SetXY(m_nTargetX, m_nTargetY);
							m_bEnpassantW = false;//���Ļ��� ������ �ѹ��� ����
						}

						m_Board[m_nTargetX][m_nTargetY].GetUnit() -> DelEnp();//�̵��� ���Ļ� ����

					}
					else
					{//�Ϲ��� ��� �̵�
						if (m_nX != m_nTargetX || m_nY != m_nTargetY) 
						{
							if (m_Board[m_nTargetX][m_nTargetY].GetUnit() != NULL)
							{//�̵��Ұ� ����
								delete m_Board[m_nTargetX][m_nTargetY].GetUnit();
							}

							//test
							/*CString str;
							str.Format("%d %d %d %d", m_nX, m_nY, m_nTargetX, m_nTargetY);
							AfxMessageBox(str);*/

							m_Board[m_nTargetX][m_nTargetY].SetUnit(m_Board[m_nX][m_nY].GetUnit());//�̵�
							m_Board[m_nX][m_nY].DelUnit();
							m_Board[m_nTargetX][m_nTargetY].GetUnit()->SetXY(m_nTargetX, m_nTargetY);

							if ((m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetUnitType() == 'P') &&
								(m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetEnp() == true))
							{//���Ļ� ��忡�� �Ϲ��� �̵��� ��� ���Ļ� ��� ����
								for (i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++)
								{//���Ļ� ��忡 �ִ� ���� ã�Ƴ�
									for (j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++)
									{
										if ((m_Board[i][j].GetUnit() != NULL) && (m_Board[i][j].GetUnit()->GetEnp() == true))
										{//���Ļ� ��� ����
											m_Board[i][j].GetUnit()->DelEnp();
										}
									}
								}
							}
						}
					}

					if ( ( m_bEnpassantB != false )&&
						( m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetColor() == BLACK ) )
					{//�渻�� ���Ļ� ���� Ȯ��
						if ( ( ( m_nTargetX == WHITE_ENPASSANT_ROW ) ||
							( m_nTargetX == BLACK_ENPASSANT_ROW ) )&&
							( m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetUnitType() == 'P' )&&
							( m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetFirst() == true ) )
						{//���Ļ� ����		
							m_spRule.Enpassant(m_nTargetX, m_nTargetY, m_Board);
						}

					} else
					if ( ( m_bEnpassantW != false )&&
						( m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetColor() == WHITE ) )
					{//�鸻�� ���Ļ� ���� Ȯ��
						if ( ( ( m_nTargetX == WHITE_ENPASSANT_ROW ) ||
							( m_nTargetX == BLACK_ENPASSANT_ROW ) )&&
							( m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetUnitType() == 'P' )&&
							( m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetFirst() == true ) )
						{//���Ļ� ����	
							m_spRule.Enpassant(m_nTargetX, m_nTargetY, m_Board);
						}
					}

					m_spRule.Castling(m_Board);//ĳ���� ���� Ȯ��

					m_Board[m_nTargetX][m_nTargetY].GetUnit()->DelFirst();//ó�� �̵��� firstMove���� false �� �ٲ�

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
					//EndGame();// ���� ���� Ȯ��
					
					DelIndex();
					
					if ( ( m_LoginDlg.GetChInfo() == LOCAL_MODE)
						|| ( m_nTurn == m_LoginDlg.GetChInfo() ) )
					{
						IndexSetting();
					}

					if ( ( m_Board[m_nTargetX][m_nTargetY].GetUnit()->GetUnitType() == 'P' )&&
						( ( m_nTargetX == WHITE_PROMOTION_ROW ) ||
						( m_nTargetX == BLACK_PROMOTION_ROW ) ) )
					{//���� ��� ���
						promotionData = m_spRule.ProMotion(m_nTargetX, m_nTargetY, m_Board, promotionData);
					}

					if ( ( m_LoginDlg.GetChInfo() != LOCAL_MODE )&&( m_LoginDlg.GetChInfo() != m_nTurn ) )
					{// �̵��� �����͸� �������� ����
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
{// �����͸� ������ ���Ͽ��� �������� �����޼����� �ڵ鸵�ϴ� �Լ�
	CData data;
	*m_GameSock >> data;

	switch ( data.GetSockMode() )
	{
		case USER_MODE :
		{// ó�� ����� �߻��ϸ� ���� �̸��� ��ȯ
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
		{// ��������� ������ �̵� ������ �޾� ������ ������ ������
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
		{// ������ ä��â�� ���� �Է½� �߻�
			// ���ŵ� ������ ä��â ǥ��
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
		{// ������ ������� ��û�� �߻�
			data.SetRenewAckMode();// ����� ��û�� ���� �亯 ���

			if ( AfxMessageBox(_T("����� ��û�� ���Խ��ϴ�!! ������Ͻðڽ��ϱ�?"), MB_YESNO, 0) == IDYES )
			{
				data.SetAckTrue();
				for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
				{//�޸� ����
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
		{// ������� ���� �亯�� ���� ����� ���� ����
			if ( data.GetAck() == RENEW_YES )
			{
				for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
				{//�޸� ����
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

			m_strStatusView = "�ð��� ������ ���� �Ѿ�Խ��ϴ�!!";
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
		{// ������ ���� ������ �߻� 
			KillTimer(0);
			
			CString strTemp = "������ �����Ǿ����ϴ�";
			
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
{// Ŭ���̾�Ʈ�� ������ �޾Ƶ��̴� �޼��� �ڵ鷯 �Լ�
	CData data;
	m_GameSock = new CGameSocket();
	// ���� ��û�� �޾Ƶ���
	if ( !m_ServerSock->Accept(*m_GameSock) )
	{
		AfxMessageBox(_T("Accept�� ���� �Ͽ����ϴ�."));

		return 0L;
	}
	
	GetDlgItem(IDC_CHAT_INPUT)->EnableWindow(true);

	m_nTurn = WHITE_TURN;
	IndexSetting();

	TimerBegin();

	// ������ ���� �ʱ�ȭ
	m_GameSock->Init(this);
	m_bConnect = true;

	m_strStatusView = "";
	UpdateData(false);

	// ���� ���̵� ����
	data.SetUserMode();
	data.SetStrData(m_LoginDlg.GetUserName());
	*m_GameSock << data;
	CString strTemp = _T("����Ǿ����ϴ�");
	m_ctrChatList.AddString(strTemp);
	
	NetDlg.temp_chat = strTemp;

	p1 = AfxBeginThread(SpeakThreadChat, this, THREAD_PRIORITY_LOWEST);

	
	return 0L;
}

LRESULT CNetworkChessDlg::OnCloseFromLogin(WPARAM wParam, LPARAM lParam)
{// �α��� ��ȭâ���� ���α׷� ���� �޼��� ȣ��� ���� ��ȭ���� ����
	CDialog::OnOK();

	return 0L;
}

void CNetworkChessDlg::OnEnterkey() 
{// ä�� �Է�â�� ����Ű�� ���� �̺�Ʈ ó��
	UpdateData();

	if ( m_LoginDlg.GetChInfo() == LOCAL_MODE )
	{
		m_ctrChatList.AddString(m_strChatInput);
		NetDlg.temp_chat = m_strChatInput;
	} else
	{
		// ������ ����
		CData data;
		data.SetChatMode();
		m_strChatInput.Append(NULL);
		data.SetStrData(m_strChatInput);
		*m_GameSock << data;
		// ���۵� ���ڿ��� ��� �����쿡�� ǥ��
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


	// Edit ��Ʈ�� �ʱ�ȭ
	m_strChatInput.Empty();

	UpdateData(FALSE);
}

void CNetworkChessDlg::OnOK()
{// ����Ű�� ������ ���Ḧ ���� ���� �Լ�
	OnEnterkey();
}

void CNetworkChessDlg::OnExitButton() 
{// ���� ��ư ������ ��� �߻�
	KillTimer(0);

	if ( AfxMessageBox(_T("���� �Ͻðڽ��ϱ�?"), MB_YESNO) == IDYES )
	{// ���� ���θ� ���
		if ( ( m_LoginDlg.GetChInfo() != LOCAL_MODE )&&( m_bConnect == true ) )
		{// ���濡�� �ڽ��� �����Ѵٰ� �˷���
			CData data;
			data.SetExitMode();
			*m_GameSock << data;
		}

		if ( m_Board != NULL )
		{
			for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{//�޸� ����
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
{// ����� ��ư�� ������ ��� �߻�
	if ( m_LoginDlg.GetChInfo() == LOCAL_MODE )
	{
		if ( m_Board != NULL )
		{
			for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{//�޸� ����
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
{// ���� ������ �߻�
	if ( AfxMessageBox(_T("������ �����Ͻðڽ��ϱ�?"), MB_YESNO) == IDYES )
	{// ���θ� Ȯ��
		KillTimer(0);

		if ( ( m_LoginDlg.GetChInfo() != LOCAL_MODE )&&( m_bConnect == true ) )
		{// ���濡�� ���� �����Ѵٰ� �˷���
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

		CString strTemp = "������ �����Ǿ����ϴ�";
		m_ctrChatList.AddString(strTemp);
		NetDlg.temp_chat = strTemp;
		
		if ( m_Board != NULL )
		{
			for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{//�޸� ����
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
{// ���� ���� ��ȭ���ڸ� ������
	CAboutDlg aboutDlg;

	aboutDlg.DoModal();
}

BOOL CNetworkChessDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( ( pMsg->message==WM_KEYDOWN )&&( pMsg->wParam==VK_ESCAPE ) )
	{// ESCŰ�� ��Ƽ� ������ ���� ���ϵ��� �����
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
{// X��ư�� ���� ����� �߻�
	if ( AfxMessageBox(_T("���� �Ͻðڽ��ϱ�?"), MB_YESNO) == IDYES )
	{// ���� ���� Ȯ��
		KillTimer(0);
		
		if ( ( m_LoginDlg.GetChInfo() != LOCAL_MODE )&&( m_bConnect == true ) )
		{// ���濡�� ���� �Ѵٰ� �˷���
			CData data;
			data.SetExitMode();
			*m_GameSock << data;
		}

		if ( m_Board != NULL )
		{
			for ( int i = BOARD_MIN_RANGE; i <= BOARD_MAX_RANGE; i++ )
			{//�޸� ����
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
{// �α��� ��ȭ���ڸ� ����� ä���� �ٽ� ���߰� ������ �缳�� ��
	bool m_bTurn = true;

	while ( m_bTurn )
	{
		m_LoginDlg.DoModal();// �α��� ��ȭ���� ȣ��
		m_bTurn = false;

		switch ( m_LoginDlg.GetChInfo())
		{
			case SERVER_MODE :
			{// ���� ����� ���
				GetDlgItem(IDC_CHAT_INPUT)->EnableWindow(false);
				// ���� ������ ����
				m_ServerSock = new CServerSocket();
				// Init�� ���� ��Ʈ �ѹ��� ���ε��ϰ� ���������
				m_ServerSock->Init(this, PORT_NUM);
				m_strStatusView = "Ŭ���̾�Ʈ�� ������ ��ٸ�!!";
				m_strWhiteUser = m_LoginDlg.GetUserName();
				UpdateData(false);

				break;
			}

			case CLIENT_MODE :
			{// Ŭ���̾�Ʈ ����� ���
				// ���ӿ� ������ ����
				GetDlgItem(IDC_CHAT_INPUT)->EnableWindow(true);
				m_GameSock = new CGameSocket();

				if( !m_GameSock->Create() ) 
				{// ���� ����
					AfxMessageBox(_T("���� ���� ����!!"));

					if ( m_GameSock != NULL )
					{
						delete m_GameSock;
						m_GameSock = NULL;
					}

					m_bTurn = true;
				}

				if( !m_GameSock->Connect(m_LoginDlg.GetIpAddr(), PORT_NUM) )
				{// ������ ���� �õ�
					AfxMessageBox(_T("���� ����!!"));

					if ( m_GameSock != NULL )
					{
						delete m_GameSock;
						m_GameSock = NULL;
					}

					// ���� ���н� �ٽ� �α��� â�� ���
					m_bTurn = true;

				} else
				{// ���� ������
					m_strBlackUser = m_LoginDlg.GetUserName();
					m_ctrChatList.AddString(_T("����Ǿ����ϴ�"));
					
					NetDlg.temp_chat = _T("����Ǿ����ϴ�");

					p1 = AfxBeginThread(SpeakThreadChat, this, THREAD_PRIORITY_LOWEST);
					
					m_bConnect = true;
					m_GameSock->Init(this);
					m_nTurn = WHITE_TURN;
					TimerBegin();
				}

				break;
			}

			case LOCAL_MODE :
			{// ���� ����� ��� �ٷ� �ε����� �����
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
	{//�鸻�� �ε��� �� �ű�
		for ( int j = BOARD_MIN_RANGE; j <= BOARD_MAX_RANGE; j++ )
		{
			m_Board[i][j].DelIndex();//���ܰ� �ε��� ����
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

					m_strStatusView = "�ð��� ������ �������� ���� �Ѿ�ϴ�!!";
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

				m_strStatusView = "�ð��� ������ �������� ���� �Ѿ�ϴ�!!";
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
	pDlg->speak(L"���ӿ���");
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
		turn_str = _T("��");
		break;
	case WHITE:
		turn_str = _T("��");
		break;
	}

	switch (NetDlg.b_unit)
	{
	case 'K':
	{
		unit_str = _T("ŷ");
		break;
	}

	case 'Q':
	{
		unit_str = _T("��");
		break;
	}

	case 'R':
	{
		unit_str = _T("��");
		break;
	}

	case 'B':
	{
		unit_str = _T("���");
		break;
	}

	case 'N':
	{
		unit_str = _T("����Ʈ");
		break;
	}

	case 'P':
	{
		unit_str = _T("��");
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
	pDlg->speak(L"�����ư");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadCLINKButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"����������ư");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadRENEWButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"����۹�ư");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadINFOButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"����������ư");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadDefaultTime(LPVOID _mothod)
{
	USES_CONVERSION;
	CString time_str;
	wchar_t* w_str;
	time_str.Format("%d�ʳ��ҽ��ϴ�", DEFAULT_REMAIN_TIME);
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
	pDlg->speak(L"30�ʳ��ҽ��ϴ�");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadTime10(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"10�ʳ��ҽ��ϴ�");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadBlackTurn(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"����");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadWhiteTurn(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"����");
	return 0;
}

UINT CNetworkChessDlg::SpeakThreadChatInput(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"ä���Է�ĭ");
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
	// ���콺 Ŭ�� ��ġ�� ��ǥ���� ü������ ��ǥ�� �ٲ�
	NetDlg.b_x = (point.y - 12) / 50;
	NetDlg.b_y = (point.x - 11) / 50;

	if ((NetDlg.b_x >= 0) && (NetDlg.b_x < 8) && (NetDlg.b_y >= 0) && (NetDlg.b_y < 8)) { //ü���� ����
		if (m_Board[NetDlg.b_x][NetDlg.b_y].GetUnit() == NULL)
		{ //�����Ѱ��� ���� ������
			//��ǥ�� �����ֱ�
			p1 = AfxBeginThread(SpeakThreadBoard, this, THREAD_PRIORITY_LOWEST);

		}
		else if(m_Board[NetDlg.b_x][NetDlg.b_y].GetUnit() != NULL) 
		{ //�����Ѱ��� ���� ������
			//��+��ǥ �����ֱ�
			NetDlg.b_color = m_Board[NetDlg.b_x][NetDlg.b_y].GetUnit()->GetColor();
			NetDlg.b_unit = m_Board[NetDlg.b_x][NetDlg.b_y].GetUnit()->GetUnitType();
			p1 = AfxBeginThread(SpeakThreadBoardUnit, this, THREAD_PRIORITY_LOWEST);

		}
	}
	else { //ü���� �ٱ�
		p1 = AfxBeginThread(SpeakThreadClient, this, THREAD_PRIORITY_LOWEST);
	}

	CDialog::OnRButtonDown(nFlags, point);
}

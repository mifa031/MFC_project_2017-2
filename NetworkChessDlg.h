#include <cstringt.h>
#include "Position.h"
#include "SpecialRule.h"
#include "Pawn.h"
#include "Unit.h"
#include "Knight.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
#include "Unit.h"
#include "LoginDlg.h"
#include "ServerSocket.h"
#include "GameSocket.h"
#include "EnterNotifyEdit.h"
#include "DefineWords.h"
#include <sapi.h>

class CNetworkChessDlg : public CDialog
{// 메인 게임 다이어로그
 //구조
public:
	CNetworkChessDlg(CWnd* pParent = NULL);
	void ViewLoginDlg();
	void IndexSetting();
	void SetBoard();
	void SetMoveIndex();
	void SelectUnit(int x, int y, int promotionData = PROMOTION_NONE);
	void OnOK();
	void EndGame();
	void TimerBegin();
	void DelIndex();
	int delline = 1;
	void speak(wchar_t* string);
	static UINT SpeakThreadClient(LPVOID _mothod);
	static UINT SpeakThreadBoard(LPVOID _mothod);
	static UINT SpeakThreadBoardUnit(LPVOID _mothod);
	static UINT SpeakThreadEXITButton(LPVOID _mothod);
	static UINT SpeakThreadCLINKButton(LPVOID _mothod);
	static UINT SpeakThreadRENEWButton(LPVOID _mothod);
	static UINT SpeakThreadINFOButton(LPVOID _mothod);
	static UINT SpeakThreadDefaultTime(LPVOID _mothod);
	static UINT SpeakThreadTime30(LPVOID _mothod);
	static UINT SpeakThreadTime10(LPVOID _mothod);
	static UINT SpeakThreadBlackTurn(LPVOID _mothod);
	static UINT SpeakThreadWhiteTurn(LPVOID _mothod);
	static UINT SpeakThreadChatInput(LPVOID _mothod);
	bool is_on_chat_input;
	static UINT SpeakThreadChat(LPVOID _mothod);
	CString temp_chat;
	CWinThread* p1;

// 다이어로그 데이터
	enum { IDD = IDD_CHESS_DIALOG };
	CEnterNotifyEdit m_ctrEdit;  
	CListBox m_ctrChatList;      
	CString	m_strStatusView;
	CString	m_strBlackUser;
	CString	m_strWhiteUser;
	CString	m_strChatInput;
	CString m_strCountDown;
	

	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	


protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnExitButton();
	afx_msg void OnRenewButton();
	afx_msg void OnCloseLinkButton();
	afx_msg void OnGameInfoButton();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg LRESULT OnAccept(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceive(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCloseFromLogin(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnterkey();
	
	DECLARE_MESSAGE_MAP()

private :
	CLoginDlg m_LoginDlg;			// 로그인 대화상자 객체
	CGameSocket *m_GameSock;		// 게임에 사용할 소켓
	CServerSocket *m_ServerSock;	// 리슨을 위한 소켓
	CPosition**	m_Board;			// 체스판 배열을 가리키는 포인터
	CSpecialRule m_spRule;			// 특수 규칙들을 적용위한 객체
	int m_nX;						// X좌표
	int m_nY;						// Y좌표
	int m_nTargetX;					// 이동할 X좌표
	int m_nTargetY;					// 이동할 Y좌표
	int m_nOldX;
	int m_nOldY;
	int m_nOldTargetX;
	int m_nOldTargetY;
	int b_x, b_y;                   // 마우스 우클릭시 보드 위의 좌표
	char b_unit;                    // 마우스 우클릭시 보드 위의 말
	int b_color;                    // 마우스 우클릭시 말의 색깔
	int m_nInputMode;				// 말의 선택인지 이동인지 구별
	int m_nTurn;					// 턴
	int m_nRemainTime;				// 타이머 남은 시간
	bool m_bEnpassantB;				// 흑팀의 앙파상 여부확인 변수
	bool m_bEnpassantW;				// 백팀의 앙파상 여부(앙파상은 1회만 가능하기 때문에)확인 변수
	bool m_bCheckMateB;				// 흑팀의 체크메이트 여부
	bool m_bCheckMateW;				// 백팀의 체크메이트 여부
	bool m_bIndexOK;				// 말이 이동할 곳이 있는지 확인함
	bool m_bConnect;				// 소켓으로 연결이 되어있는지 여부확인함
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

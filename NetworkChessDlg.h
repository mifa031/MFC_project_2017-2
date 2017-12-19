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
{// ���� ���� ���̾�α�
 //����
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

// ���̾�α� ������
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
	CLoginDlg m_LoginDlg;			// �α��� ��ȭ���� ��ü
	CGameSocket *m_GameSock;		// ���ӿ� ����� ����
	CServerSocket *m_ServerSock;	// ������ ���� ����
	CPosition**	m_Board;			// ü���� �迭�� ����Ű�� ������
	CSpecialRule m_spRule;			// Ư�� ��Ģ���� �������� ��ü
	int m_nX;						// X��ǥ
	int m_nY;						// Y��ǥ
	int m_nTargetX;					// �̵��� X��ǥ
	int m_nTargetY;					// �̵��� Y��ǥ
	int m_nOldX;
	int m_nOldY;
	int m_nOldTargetX;
	int m_nOldTargetY;
	int b_x, b_y;                   // ���콺 ��Ŭ���� ���� ���� ��ǥ
	char b_unit;                    // ���콺 ��Ŭ���� ���� ���� ��
	int b_color;                    // ���콺 ��Ŭ���� ���� ����
	int m_nInputMode;				// ���� �������� �̵����� ����
	int m_nTurn;					// ��
	int m_nRemainTime;				// Ÿ�̸� ���� �ð�
	bool m_bEnpassantB;				// ������ ���Ļ� ����Ȯ�� ����
	bool m_bEnpassantW;				// ������ ���Ļ� ����(���Ļ��� 1ȸ�� �����ϱ� ������)Ȯ�� ����
	bool m_bCheckMateB;				// ������ üũ����Ʈ ����
	bool m_bCheckMateW;				// ������ üũ����Ʈ ����
	bool m_bIndexOK;				// ���� �̵��� ���� �ִ��� Ȯ����
	bool m_bConnect;				// �������� ������ �Ǿ��ִ��� ����Ȯ����
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

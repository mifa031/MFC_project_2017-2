
#pragma once

#include "DefineWords.h"
#include "afxext.h"
#include <sapi.h>

class CLoginDlg : public CDialog
{// 로그인 다이얼로그
public :
	CLoginDlg(CWnd* pParent = NULL);
	void OnOK();
	CString GetUserName();
	CString GetIpAddr();
	int GetChInfo();
	enum { IDD = IDD_LOGIN_DIALOG };
	CString	m_strIP1;
	CString	m_strIP2;
	CString	m_strIP3;
	CString	m_strIP4;
	CString	m_strUserName;
	CWinThread* p1;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected :
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

private :
	int m_nChInfo;
	CString m_strIPAddr;

public:
	CBitmapButton m_server_button;
	CBitmapButton m_client_button;
	CBitmapButton m_local_button;
	afx_msg void OnBnClickedLocal();
	afx_msg void OnBnClickedServer();
	afx_msg void OnBnClickedClient();
	CBitmapButton m_ok;
	void speak(wchar_t* string);
	static UINT SpeakThreadClient(LPVOID _mothod);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	static UINT SpeakThreadServerButton(LPVOID _mothod);
	static UINT SpeakThreadClientButton(LPVOID _mothod);
	static UINT SpeakThreadLocalButton(LPVOID _mothod);
	static UINT SpeakThreadOkButton(LPVOID _mothod);
	static UINT SpeakThreadIDException(LPVOID _mothod);
	static UINT SpeakThreadIPException(LPVOID _mothod);


	static UINT SpeakThreadLocalRButton(LPVOID _mothod);
	static UINT SpeakThreadServerRButton(LPVOID _mothod);
	static UINT SpeakThreadClientRButton(LPVOID _mothod);
	static UINT SpeakThreadOkRButton(LPVOID _mothod);
	static UINT SpeakThreadUserNameRButton(LPVOID _mothod);
	static UINT SpeakThreadIPRButton(LPVOID _mothod);
};

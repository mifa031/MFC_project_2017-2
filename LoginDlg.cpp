#include "stdafx.h"
#include "NetworkChess.h"
#include "LoginDlg.h"


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	m_strIP1 = _T("127");
	m_strIP2 = _T("0");
	m_strIP3 = _T("0");
	m_strIP4 = _T("1");
	m_strUserName = _T("");
	m_nChInfo = SERVER_MODE;
	p1 = NULL;
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IP1, m_strIP1);
	DDV_MaxChars(pDX, m_strIP1, 3);
	DDX_Text(pDX, IDC_IP2, m_strIP2);
	DDV_MaxChars(pDX, m_strIP2, 3);
	DDX_Text(pDX, IDC_IP3, m_strIP3);
	DDV_MaxChars(pDX, m_strIP3, 3);
	DDX_Text(pDX, IDC_IP4, m_strIP4);
	DDV_MaxChars(pDX, m_strIP4, 3);
	DDX_Text(pDX, IDC_USER_NAME, m_strUserName);
	DDX_Control(pDX, IDC_SERVER, m_server_button);
	DDX_Control(pDX, IDC_CLIENT, m_client_button);
	DDX_Control(pDX, IDC_LOCAL, m_local_button);

	DDX_Control(pDX, IDOK, m_ok);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_LOCAL, &CLoginDlg::OnBnClickedLocal)
	ON_BN_CLICKED(IDC_SERVER, &CLoginDlg::OnBnClickedServer)
	ON_BN_CLICKED(IDC_CLIENT, &CLoginDlg::OnBnClickedClient)
	ON_WM_RBUTTONDOWN()

END_MESSAGE_MAP()


void CLoginDlg::OnPaint() 
{
	CPaintDC dc(this);
	CDC dcMem;
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_LoginBG);
	m_nChInfo = SERVER_MODE;
	switch ( m_nChInfo )
	{
		case SERVER_MODE :
		{
			
			GetDlgItem(IDC_USER_NAME)->EnableWindow(true);
			GetDlgItem(IDC_IP1)->EnableWindow(false);
			GetDlgItem(IDC_IP2)->EnableWindow(false);
			GetDlgItem(IDC_IP3)->EnableWindow(false);
			GetDlgItem(IDC_IP4)->EnableWindow(false);

			break;
		}

		case CLIENT_MODE :
		{
			
			GetDlgItem(IDC_USER_NAME)->EnableWindow(true);
			GetDlgItem(IDC_IP1)->EnableWindow(true);
			GetDlgItem(IDC_IP2)->EnableWindow(true);
			GetDlgItem(IDC_IP3)->EnableWindow(true);
			GetDlgItem(IDC_IP4)->EnableWindow(true);

			break;
		}

		case LOCAL_MODE :
		{
		
			GetDlgItem(IDC_USER_NAME)->EnableWindow(false);
			GetDlgItem(IDC_IP1)->EnableWindow(false);
			GetDlgItem(IDC_IP2)->EnableWindow(false);
			GetDlgItem(IDC_IP3)->EnableWindow(false);
			GetDlgItem(IDC_IP4)->EnableWindow(false);

			break;
		}
	}

	dcMem.CreateCompatibleDC(&dc);
	CBitmap* pOldBitmap = dcMem.SelectObject(&bitmap);
	dc.BitBlt(0, 0, 450, 400, &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(&pOldBitmap);

	UpdateData(false);
}

BOOL CLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_server_button.LoadBitmaps(IDB_HOST,IDB_HOST_ACT, IDB_HOST_ACT,NULL);
	m_server_button.SizeToContent();
	m_client_button.LoadBitmaps(IDB_JOIN,IDB_JOIN_ACT, IDB_JOIN_ACT,NULL);
	m_client_button.SizeToContent();
	m_local_button.LoadBitmaps(IDB_2P, IDB_2P_ACT, IDB_2P_ACT, NULL);
	m_local_button.SizeToContent();
	m_ok.LoadBitmaps(IDB_OK,NULL,NULL,NULL);
	m_ok.SizeToContent();
	
	return TRUE;
}

int CLoginDlg::GetChInfo()
{// 채널의 모드를 반환
	return m_nChInfo;
}

CString CLoginDlg::GetIpAddr()
{// IP주소를 반환
	return m_strIPAddr;
}

CString CLoginDlg::GetUserName()
{// 유저의 이름을 반환
	return m_strUserName;
}


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( ( pMsg->message==WM_KEYDOWN )&&( pMsg->wParam==VK_ESCAPE ) )
	{// ESC키를 통해 나가는 것을 막음
		return TRUE;
	}

	if (pMsg->message == WM_RBUTTONDOWN) 
	{
		CRect rect;
		this->GetDlgItem(IDC_SERVER)->GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt)) {
			p1 = AfxBeginThread(SpeakThreadServerRButton, this, THREAD_PRIORITY_LOWEST);

		}

		this->GetDlgItem(IDC_CLIENT)->GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt)) {
			p1 = AfxBeginThread(SpeakThreadClientRButton, this, THREAD_PRIORITY_LOWEST);
		}

		this->GetDlgItem(IDC_LOCAL)->GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt)) {
			p1 = AfxBeginThread(SpeakThreadLocalRButton, this, THREAD_PRIORITY_LOWEST);
		}

		this->GetDlgItem(IDOK)->GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt)) {
			p1 = AfxBeginThread(SpeakThreadOkRButton, this, THREAD_PRIORITY_LOWEST);
		}
		this->GetDlgItem(IDC_USER_NAME)->GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt)) {
			p1 = AfxBeginThread(SpeakThreadUserNameRButton, this, THREAD_PRIORITY_LOWEST);

		}
		this->GetDlgItem(IDC_IP1)->GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt)) {
			p1 = AfxBeginThread(SpeakThreadIPRButton, this, THREAD_PRIORITY_LOWEST);
		}
		this->GetDlgItem(IDC_IP2)->GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt)) {
			p1 = AfxBeginThread(SpeakThreadIPRButton, this, THREAD_PRIORITY_LOWEST);
		}
		this->GetDlgItem(IDC_IP3)->GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt)) {
			p1 = AfxBeginThread(SpeakThreadIPRButton, this, THREAD_PRIORITY_LOWEST);

		}
		this->GetDlgItem(IDC_IP4)->GetWindowRect(&rect);
		if (rect.PtInRect(pMsg->pt)) {
			p1 = AfxBeginThread(SpeakThreadIPRButton, this, THREAD_PRIORITY_LOWEST);

		}

	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CLoginDlg::OnClose() 
{
	if ( AfxMessageBox("종료 하시겠습니까?", MB_YESNO) == IDYES )
	{// X버튼을 통해 종료시 프로그램 종료
		m_nChInfo = LOCAL_MODE;

		GetParent()->SendMessage(UM_CLOSELOGIN);

		CDialog::OnClose();
	}
}


void CLoginDlg::OnOK()
{
	UpdateData(true);
	p1 = AfxBeginThread(SpeakThreadOkButton, this, THREAD_PRIORITY_LOWEST);

	if ( ( m_nChInfo != LOCAL_MODE )&&( m_strUserName.IsEmpty() ) )
	{// 이름을 입력 안했을 경우
		p1 = AfxBeginThread(SpeakThreadIDException, this, THREAD_PRIORITY_LOWEST);

		AfxMessageBox("이름을 입력해주세요.");
	
		return;
	}

	if ( ( m_nChInfo == false )&&
		( m_strIP1.IsEmpty() ) || ( m_strIP2.IsEmpty() ) ||
		( m_strIP3.IsEmpty() ) || ( m_strIP4.IsEmpty() ) )
	{// IP를 입력 안했을 경우
		p1 = AfxBeginThread(SpeakThreadIPException, this, THREAD_PRIORITY_LOWEST);

		AfxMessageBox("IP주소를 입력해주세요.");

		return;
	}

	// IP값 받음
	m_strIPAddr = m_strIP1 + "." + m_strIP2 + "." + m_strIP3 + "." + m_strIP4;

	CDialog::OnOK();
}

void CLoginDlg::OnBnClickedLocal()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nChInfo = LOCAL_MODE;
	
	GetDlgItem(IDC_USER_NAME)->EnableWindow(false);
	GetDlgItem(IDC_IP1)->EnableWindow(false);
	GetDlgItem(IDC_IP2)->EnableWindow(false);
	GetDlgItem(IDC_IP3)->EnableWindow(false);
	GetDlgItem(IDC_IP4)->EnableWindow(false);

	p1 = AfxBeginThread(SpeakThreadLocalButton, this, THREAD_PRIORITY_LOWEST);
}


void CLoginDlg::OnBnClickedServer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_nChInfo =SERVER_MODE;
	GetDlgItem(IDC_USER_NAME)->EnableWindow(true);
	GetDlgItem(IDC_IP1)->EnableWindow(false);
	GetDlgItem(IDC_IP2)->EnableWindow(false);
	GetDlgItem(IDC_IP3)->EnableWindow(false);
	GetDlgItem(IDC_IP4)->EnableWindow(false);

	CWinThread* p1 = NULL;

	p1 = AfxBeginThread(SpeakThreadServerButton, this, THREAD_PRIORITY_LOWEST);
}


void CLoginDlg::OnBnClickedClient()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	m_nChInfo = CLIENT_MODE;
	GetDlgItem(IDC_USER_NAME)->EnableWindow(true);
	GetDlgItem(IDC_IP1)->EnableWindow(true);
	GetDlgItem(IDC_IP2)->EnableWindow(true);
	GetDlgItem(IDC_IP3)->EnableWindow(true);
	GetDlgItem(IDC_IP4)->EnableWindow(true);

	p1 = AfxBeginThread(SpeakThreadClientButton, this, THREAD_PRIORITY_LOWEST);
}


void CLoginDlg::speak(wchar_t* string)
{
	CoInitialize(NULL);
	ISpVoice * pVoice = NULL;
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
	hr = pVoice->Speak(string, 0, NULL);
	pVoice->Release();
	pVoice = NULL;
	::CoUninitialize();
}


UINT CLoginDlg::SpeakThreadClient(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"게임로그인 화면 영역입니다. Tab키로 각 버튼을 이동할 수 있습니다.");
	return 0;
}




void CLoginDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	

	CDialog::OnRButtonDown(nFlags, point);
}



UINT CLoginDlg::SpeakThreadServerButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"게임방 만들기 버튼클릭");
	return 0;
}


UINT CLoginDlg::SpeakThreadClientButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"대국참여 버튼클릭");
	return 0;
}


UINT CLoginDlg::SpeakThreadLocalButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"2인플레이 버튼클릭");
	return 0;
}


UINT CLoginDlg::SpeakThreadOkButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"확인 버튼클릭");
	return 0;
}


UINT CLoginDlg::SpeakThreadIDException(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"이름을 입력해주세요.");
	return 0;
}


UINT CLoginDlg::SpeakThreadIPException(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"IP주소를 입력해주세요.");
	return 0;
	
}


UINT CLoginDlg::SpeakThreadLocalRButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"2인플레이 버튼");
	return 0;
}


UINT CLoginDlg::SpeakThreadServerRButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"게임방 만들기버튼");
	return 0;
}


UINT CLoginDlg::SpeakThreadClientRButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"대국참여 버튼");
	return 0;
}


UINT CLoginDlg::SpeakThreadOkRButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"확인 버튼");
	return 0;
}


UINT CLoginDlg::SpeakThreadUserNameRButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"이름 입력영역");
	return 0;
}


UINT CLoginDlg::SpeakThreadIPRButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"IP 입력영역");
	return 0;
}

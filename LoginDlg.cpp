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
{// ä���� ��带 ��ȯ
	return m_nChInfo;
}

CString CLoginDlg::GetIpAddr()
{// IP�ּҸ� ��ȯ
	return m_strIPAddr;
}

CString CLoginDlg::GetUserName()
{// ������ �̸��� ��ȯ
	return m_strUserName;
}


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( ( pMsg->message==WM_KEYDOWN )&&( pMsg->wParam==VK_ESCAPE ) )
	{// ESCŰ�� ���� ������ ���� ����
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
	if ( AfxMessageBox("���� �Ͻðڽ��ϱ�?", MB_YESNO) == IDYES )
	{// X��ư�� ���� ����� ���α׷� ����
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
	{// �̸��� �Է� ������ ���
		p1 = AfxBeginThread(SpeakThreadIDException, this, THREAD_PRIORITY_LOWEST);

		AfxMessageBox("�̸��� �Է����ּ���.");
	
		return;
	}

	if ( ( m_nChInfo == false )&&
		( m_strIP1.IsEmpty() ) || ( m_strIP2.IsEmpty() ) ||
		( m_strIP3.IsEmpty() ) || ( m_strIP4.IsEmpty() ) )
	{// IP�� �Է� ������ ���
		p1 = AfxBeginThread(SpeakThreadIPException, this, THREAD_PRIORITY_LOWEST);

		AfxMessageBox("IP�ּҸ� �Է����ּ���.");

		return;
	}

	// IP�� ����
	m_strIPAddr = m_strIP1 + "." + m_strIP2 + "." + m_strIP3 + "." + m_strIP4;

	CDialog::OnOK();
}

void CLoginDlg::OnBnClickedLocal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.


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
	pDlg->speak(L"���ӷα��� ȭ�� �����Դϴ�. TabŰ�� �� ��ư�� �̵��� �� �ֽ��ϴ�.");
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
	pDlg->speak(L"���ӹ� ����� ��ưŬ��");
	return 0;
}


UINT CLoginDlg::SpeakThreadClientButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"�뱹���� ��ưŬ��");
	return 0;
}


UINT CLoginDlg::SpeakThreadLocalButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"2���÷��� ��ưŬ��");
	return 0;
}


UINT CLoginDlg::SpeakThreadOkButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"Ȯ�� ��ưŬ��");
	return 0;
}


UINT CLoginDlg::SpeakThreadIDException(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"�̸��� �Է����ּ���.");
	return 0;
}


UINT CLoginDlg::SpeakThreadIPException(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"IP�ּҸ� �Է����ּ���.");
	return 0;
	
}


UINT CLoginDlg::SpeakThreadLocalRButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"2���÷��� ��ư");
	return 0;
}


UINT CLoginDlg::SpeakThreadServerRButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"���ӹ� ������ư");
	return 0;
}


UINT CLoginDlg::SpeakThreadClientRButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"�뱹���� ��ư");
	return 0;
}


UINT CLoginDlg::SpeakThreadOkRButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"Ȯ�� ��ư");
	return 0;
}


UINT CLoginDlg::SpeakThreadUserNameRButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"�̸� �Է¿���");
	return 0;
}


UINT CLoginDlg::SpeakThreadIPRButton(LPVOID _mothod)
{
	CLoginDlg* fir = (CLoginDlg*)_mothod;
	CLoginDlg* pDlg = (CLoginDlg*)AfxGetApp();
	pDlg->speak(L"IP �Է¿���");
	return 0;
}

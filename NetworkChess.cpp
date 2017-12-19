
#include "stdafx.h"
#include "NetworkChess.h"
#include "NetworkChessDlg.h"


BEGIN_MESSAGE_MAP(CNetworkChessApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


CNetworkChessApp::CNetworkChessApp()
{
	
}

CNetworkChessApp theApp;

BOOL CNetworkChessApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	CNetworkChessDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{	
	}
	else if (nResponse == IDCANCEL)
	{	
	}

	
	return FALSE;
}


#include "stdafx.h"
#include "NetworkChess.h"
#include "PromotionDlg.h"



CPromotionDlg::CPromotionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPromotionDlg::IDD, pParent)
{

	m_nProData = PROMOTION_TO_QUEEN;
}


void CPromotionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPromotionDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_QUEEN, OnRadioQueen)
	ON_BN_CLICKED(IDC_RADIO_ROOK, OnRadioRook)
	ON_BN_CLICKED(IDC_RADIO_KNIGHT, OnRadioKnight)
	ON_BN_CLICKED(IDC_RADIO_BISHOP, OnRadioBishop)
END_MESSAGE_MAP()


void CPromotionDlg::OnRadioQueen() 
{
	m_nProData = PROMOTION_TO_QUEEN;

	Invalidate();
}

void CPromotionDlg::OnRadioRook() 
{
	m_nProData = PROMOTION_TO_ROOK;

	Invalidate();
}

void CPromotionDlg::OnRadioKnight() 
{
	m_nProData = PROMOTION_TO_KNIGHT;

	Invalidate();
}

void CPromotionDlg::OnRadioBishop() 
{
	m_nProData = PROMOTION_TO_BISHOP;

	Invalidate();
}

int CPromotionDlg::GetProData()
{
	return m_nProData;
}

void CPromotionDlg::OnOK() 
{
	CDialog::OnOK();
}

BOOL CPromotionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	((CButton*)GetDlgItem(IDC_RADIO_QUEEN)) -> SetCheck(TRUE);

	return TRUE;
}

BOOL CPromotionDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( ( pMsg->message==WM_KEYDOWN )&&( pMsg->wParam==VK_ESCAPE ) )
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

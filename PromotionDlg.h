
#pragma once

#include "resource.h"
#include "DefineWords.h"



class CPromotionDlg : public CDialog
{ // ���θ�� ���� �ߵ��ϴ� ���̾�α�
public :
	CPromotionDlg(CWnd* pParent = NULL);
	int GetProData();
	enum { IDD = IDD_PROMOTION_DIALOG };

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected :
	afx_msg void OnRadioQueen();
	afx_msg void OnRadioRook();
	afx_msg void OnRadioKnight();
	afx_msg void OnRadioBishop();
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private :
	int m_nProData;

};

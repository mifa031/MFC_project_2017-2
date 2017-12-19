#include "stdafx.h"
#include "NetworkChess.h"
#include "EnterNotifyEdit.h"

CEnterNotifyEdit::CEnterNotifyEdit()
{
	;
}

CEnterNotifyEdit::~CEnterNotifyEdit()
{
	;
}


BEGIN_MESSAGE_MAP(CEnterNotifyEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()


void CEnterNotifyEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{ // 버튼이 눌렀음을 감지
	if ( nChar == VK_RETURN )
	{
		GetParent()->SendMessage(WM_COMMAND, ID_ENTERKEY, 0);
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
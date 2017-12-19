

class CEnterNotifyEdit : public CEdit
{  // 채팅 입력창의 엔터키를 처리하기 위한 클래스
public :
	CEnterNotifyEdit();
	virtual ~CEnterNotifyEdit();

protected :
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
		
}; //


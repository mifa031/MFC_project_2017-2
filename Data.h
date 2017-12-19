#include <cstring>
#include <cstringt.h>
#include "DefineWords.h"

class CData : public CObject
{// 소켓을 통해 주고받을 데이터에 대한 클래스
public :
	CData();
	//함수 정의 부분
	virtual ~CData();
	int GetSockMode();
	int GetX();
	int GetY();
	int GetTargetX();
	int GetTargetY();
	int GetPromotionData();
	int GetAck();
	CString GetStrData();
	void SetX(int x);
	void SetY(int y);
	void SetTargetX(int targetX);
	void SetTargetY(int targetY);
	void SetPromotionData(int proData);
	void SetStrData(CString strData);
	void SetUserMode();
	void SetGameMode();
	void SetChatMode();
	void SetRenewMode();
	void SetRenewAckMode();
	void SetAckTrue();
	void SetAckFalse();
	void SetCloseMode();
	void SetExitMode();	
	void SetTimeOverMode();
	void Serialize(CArchive &archive);

private :
	int m_SockMode;			// 소켓 모드
	int m_nX;				// 선택 X좌표
	int m_nY;				// 선택 Y좌표
	int m_nTargetX;			// 이동 X좌표
	int m_nTargetY;			// 이동 Y좌표
	int m_nPromotionData;	// 프로모션할 유닛
	int m_nAck;				// Renew의 답변 내용
	CString m_strData;		// 채팅 내용

};





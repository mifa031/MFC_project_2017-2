#include <cstring>
#include <cstringt.h>
#include "DefineWords.h"

class CData : public CObject
{// ������ ���� �ְ���� �����Ϳ� ���� Ŭ����
public :
	CData();
	//�Լ� ���� �κ�
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
	int m_SockMode;			// ���� ���
	int m_nX;				// ���� X��ǥ
	int m_nY;				// ���� Y��ǥ
	int m_nTargetX;			// �̵� X��ǥ
	int m_nTargetY;			// �̵� Y��ǥ
	int m_nPromotionData;	// ���θ���� ����
	int m_nAck;				// Renew�� �亯 ����
	CString m_strData;		// ä�� ����

};





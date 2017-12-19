#include "stdafx.h"
#include "NetworkChess.h"
#include "Data.h"

CData::CData()
{
	m_SockMode = NULL;  //���� ��� 
	m_nX = NULL;        //X��ǥ �ʱ�ȭ
	m_nY = NULL;	    //X��ǥ �ʱ�ȭ
	m_nTargetX = NULL;  //�̵� X��ǥ �ʱ�ȭ
	m_nTargetY = NULL;  //�̵� Y��ǥ �ʱ�ȭ
	m_nPromotionData = NULL; // ���θ�� ������ �ʱ�ȭ
	m_nAck = NULL;	//
	m_strData = "";
}

CData::~CData()
{
}


int CData::GetSockMode()
{						
	return m_SockMode;// �������� ��带 ��ȯ��
}

int CData::GetX()
{
	return m_nX;// X��ǥ ��ȯ��
}

int CData::GetY()
{
	return m_nY;// Y��ǥ ��ȯ��
}

int CData::GetTargetX()
{
	return m_nTargetX;// �̵� X��ǥ ��ȯ��
}

int CData::GetTargetY()
{
	return m_nTargetY;// �̵� Y��ǥ ��ȯ��
}

int CData::GetPromotionData()
{
	return m_nPromotionData;// ���θ�� �� ���� �� ��ȯ��
}

int CData::GetAck()
{
	return m_nAck;// Renew�� �亯 ���� ��ȯ��
}

CString CData::GetStrData()
{
	return m_strData;// ��Ʈ�������� ��ȯ��
}

void CData::SetX(int x)
{
	m_nX = x;  // x�� ������
}

void CData::SetY(int y)
{
	m_nY = y;// y�� ������
}

void CData::SetTargetX(int targetX)
{
	m_nTargetX = targetX;// �̵� x�� ������
}

void CData::SetTargetY(int targetY)
{
	m_nTargetY = targetY;// �̵� y�� ������
}

void CData::SetPromotionData(int promotionData)
{
	m_nPromotionData = promotionData;// ���θ�� �� ���� ������
}


void CData::SetStrData(CString strData)
{
	m_strData = strData;// ��Ʈ�� ������
}

void CData::SetUserMode()
{
	m_SockMode = USER_MODE;// �������� ������
}

void CData::SetGameMode()
{
	m_SockMode = GAME_MODE;// ���Ӹ��� ������
}

void CData::SetChatMode()
{
	m_SockMode = CHAT_MODE;// ä�ø��� ������
}

void CData::SetRenewMode()
{
	m_SockMode = RENEW_MODE;// ����� ���� ������
}

void CData::SetRenewAckMode()
{
	m_SockMode = RENEW_ACK_MODE;// ����� ����� �亯���� ������
}

void CData::SetAckTrue()
{
	m_nAck = RENEW_YES;// ����� �亯�� yes�� ������
}

void CData::SetAckFalse()
{
	m_nAck = RENEW_NO;// ����� �亯�� no�� ������
}

void CData::SetCloseMode()
{
	m_SockMode = CLOSE_MODE;// ���� ���� ���� ������
}

void CData::SetExitMode()
{
	m_SockMode = EXIT_MODE;// ���� ���� ������
}

void CData::SetTimeOverMode()
{
	m_SockMode = TIME_OVER_MODE;// Ÿ�� ������ �߻���
}

void CData::Serialize(CArchive &archive)
{// ������ �۽� �� ���� Part
	if( archive.IsStoring() )
	{// �۽� Part
		archive << m_nX;
		archive << m_nY;
		archive << m_nTargetX;
		archive << m_nTargetY;
		archive << m_nAck;
		archive << m_nPromotionData;
		archive << m_SockMode;
		archive << m_strData;

	} else
	{// ���� Part
		archive >> m_nX;
		archive >> m_nY;
		archive >> m_nTargetX;
		archive >> m_nTargetY;
		archive >> m_nAck;
		archive >> m_nPromotionData;
		archive >> m_SockMode;
		archive >> m_strData;
	}
}
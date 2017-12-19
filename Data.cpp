#include "stdafx.h"
#include "NetworkChess.h"
#include "Data.h"

CData::CData()
{
	m_SockMode = NULL;  //소켓 모드 
	m_nX = NULL;        //X좌표 초기화
	m_nY = NULL;	    //X좌표 초기화
	m_nTargetX = NULL;  //이동 X좌표 초기화
	m_nTargetY = NULL;  //이동 Y좌표 초기화
	m_nPromotionData = NULL; // 프로모션 데이터 초기화
	m_nAck = NULL;	//
	m_strData = "";
}

CData::~CData()
{
}


int CData::GetSockMode()
{						
	return m_SockMode;// 데이터의 모드를 반환함
}

int CData::GetX()
{
	return m_nX;// X좌표 반환함
}

int CData::GetY()
{
	return m_nY;// Y좌표 반환함
}

int CData::GetTargetX()
{
	return m_nTargetX;// 이동 X좌표 반환함
}

int CData::GetTargetY()
{
	return m_nTargetY;// 이동 Y좌표 반환함
}

int CData::GetPromotionData()
{
	return m_nPromotionData;// 프로모션 할 유닛 값 반환함
}

int CData::GetAck()
{
	return m_nAck;// Renew의 답변 내용 반환함
}

CString CData::GetStrData()
{
	return m_strData;// 스트링데이터 반환함
}

void CData::SetX(int x)
{
	m_nX = x;  // x값 설정함
}

void CData::SetY(int y)
{
	m_nY = y;// y값 설정함
}

void CData::SetTargetX(int targetX)
{
	m_nTargetX = targetX;// 이동 x값 설정함
}

void CData::SetTargetY(int targetY)
{
	m_nTargetY = targetY;// 이동 y값 설정함
}

void CData::SetPromotionData(int promotionData)
{
	m_nPromotionData = promotionData;// 프로모션 할 유닛 설정함
}


void CData::SetStrData(CString strData)
{
	m_strData = strData;// 스트링 설정함
}

void CData::SetUserMode()
{
	m_SockMode = USER_MODE;// 유저모드로 설정함
}

void CData::SetGameMode()
{
	m_SockMode = GAME_MODE;// 게임모드로 설정함
}

void CData::SetChatMode()
{
	m_SockMode = CHAT_MODE;// 채팅모드로 설정함
}

void CData::SetRenewMode()
{
	m_SockMode = RENEW_MODE;// 재시작 모드로 설정함
}

void CData::SetRenewAckMode()
{
	m_SockMode = RENEW_ACK_MODE;// 재시작 모드의 답변모드로 설정함
}

void CData::SetAckTrue()
{
	m_nAck = RENEW_YES;// 재시작 답변을 yes로 설정함
}

void CData::SetAckFalse()
{
	m_nAck = RENEW_NO;// 재시작 답변을 no로 설정함
}

void CData::SetCloseMode()
{
	m_SockMode = CLOSE_MODE;// 접속 해제 모드로 설정함
}

void CData::SetExitMode()
{
	m_SockMode = EXIT_MODE;// 종료 모드로 설정함
}

void CData::SetTimeOverMode()
{
	m_SockMode = TIME_OVER_MODE;// 타임 오버시 발생함
}

void CData::Serialize(CArchive &archive)
{// 데이터 송신 및 수신 Part
	if( archive.IsStoring() )
	{// 송신 Part
		archive << m_nX;
		archive << m_nY;
		archive << m_nTargetX;
		archive << m_nTargetY;
		archive << m_nAck;
		archive << m_nPromotionData;
		archive << m_SockMode;
		archive << m_strData;

	} else
	{// 수신 Part
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
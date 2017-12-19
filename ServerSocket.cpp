

#include "stdafx.h"
#include "NetworkChess.h" //네트워크 체스 해더파일
#include "ServerSocket.h" //네트워크 체스 헤더파일


CServerSocket::CServerSocket()
{
	;
}

CServerSocket::~CServerSocket()
{
	;
}


#if 0
BEGIN_MESSAGE_MAP(CServerSocket, CSocket)
END_MESSAGE_MAP()
#endif


void CServerSocket::Init(CWnd *pWnd, int nPortNum)
{// Listen을 설정
	this->m_pWnd = pWnd;// 인자로 넘어온 부모의 윈도우 객체를 설정

	Create(nPortNum);// 포트번호를 받아하여 소켓을 생성

	Listen();//Listen
}

void CServerSocket::OnAccept(int nErrorCode) 
{
	m_pWnd->SendMessage(UM_ACCEPT); // 클라이언트가 접속 시 부모 클래스로 
									// 유저메세지를 보내 핸들러 메세지가 동작하도록 함

	CSocket::OnAccept(nErrorCode);
}

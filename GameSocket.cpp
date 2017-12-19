
#include "stdafx.h"
#include "NetworkChess.h"
#include "GameSocket.h"

CGameSocket::CGameSocket()
{
	m_pWnd = NULL;	//윈도우 포인터 초기화
	m_pFile = NULL; //파일 포인터 초기화
	m_pArchiveIn = NULL; //아카이브 포인터(인) 초기화
	m_pArchiveOut = NULL;//아카이브 포인터(인) 초기화
}

CGameSocket::~CGameSocket()
{
	if ( m_pFile != NULL )
	{
		delete m_pFile; //파일 포인터 삭제
		m_pFile = NULL;
	}

	if ( m_pArchiveIn != NULL )
	{
		m_pArchiveIn->Abort();
		delete m_pArchiveIn;//아카이브 포인터(인) 삭제
		m_pArchiveIn = NULL;
	}

	if ( m_pArchiveOut != NULL )
	{
		m_pArchiveOut->Abort();
		delete m_pArchiveOut; //아카이브 포인터(아웃) 삭제
		m_pArchiveOut = NULL;
	}
}



#if 0
BEGIN_MESSAGE_MAP(CGameSocket, CSocket)
END_MESSAGE_MAP()
#endif


void CGameSocket::Init(CWnd *pWnd)
{// 통신 환경을 설정
	m_pWnd = pWnd;					 // 부모 윈도우 클래스의 객체를 설정함
	m_pFile = new CSocketFile(this); // 소켓 파일을 현재 소켓을 이용하여 설정함
	m_pArchiveIn = new CArchive(m_pFile, CArchive::load);// 소켓 파일을 통해 데이터를 받아 올  CArchive클래스 설정함
	m_pArchiveOut = new CArchive(m_pFile, CArchive::store); // 소켓 파일을 통해 데이터를 보낼  CArchive클래스 설정함
}

void CGameSocket::Send(CData *pData)//중요! 송신 파트
{													
	if ( m_pArchiveOut != NULL )
	{										
		pData->Serialize(*m_pArchiveOut); // CData클래스의 Serialize함수를 통해서 CArchive클래스를 통해 데이터를 보냄
	}
}

void CGameSocket::Receive(CData *pData)
{
	if ( m_pArchiveIn != NULL )
	{
		pData->Serialize(*m_pArchiveIn);// CData클래스의 Serialize함수를 통해서 CArchive클래스를 통해 데이터를 받음
	}
}

void CGameSocket::Flush()
{
	if ( m_pArchiveOut != NULL )
	{
		m_pArchiveOut->Flush();// 데이터를 보내는 CArchive클래스의 버퍼를 비움.
	}
}

void CGameSocket::OnReceive(int nErrorCode) 
{	// 데이터가 오면 발생하는 메세지 핸들러 함수
	// 부모 클래스의 유저 메세지를 보내 메세지 핸들러를 통해 데이터를 받을 수 있도록 함
	
	m_pWnd->SendMessage(UM_DATARECEIVE);

	CSocket::OnReceive(nErrorCode);
}

void CGameSocket::OnClose(int nErrorCode) 
{	// 소켓을 닫으면서 동적 메모리를 해제
	// TODO: Add your specialized code here and/or call the base class
	if ( m_pFile != NULL )
	{
		delete m_pFile;//파일 포인터 삭제
		m_pFile = NULL;
	}

	if ( m_pArchiveIn != NULL )
	{
		m_pArchiveIn->Abort();
		delete m_pArchiveIn;//아카이브 포인터(인) 삭제
		m_pArchiveIn = NULL;
	}

	if ( m_pArchiveOut != NULL )
	{
		m_pArchiveOut->Abort();
		delete m_pArchiveOut;//아카이브 포인터(아웃) 삭제
		m_pArchiveOut = NULL;
	}

	CSocket::OnClose(nErrorCode);
}

void CGameSocket::operator <<(CData &data)
{
	Send(&data);// 연산자 오버로딩을 통해 쉽게 메세지를 보낼 수 있도록 함
	Flush();

}

void CGameSocket::operator >>(CData &data)
{
	Receive(&data); // 연산자 오버로딩을 통해 쉽게 메세지를 받을 수 있도록 함
}


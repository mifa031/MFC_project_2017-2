

#include "stdafx.h"
#include "NetworkChess.h" //��Ʈ��ũ ü�� �ش�����
#include "ServerSocket.h" //��Ʈ��ũ ü�� �������


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
{// Listen�� ����
	this->m_pWnd = pWnd;// ���ڷ� �Ѿ�� �θ��� ������ ��ü�� ����

	Create(nPortNum);// ��Ʈ��ȣ�� �޾��Ͽ� ������ ����

	Listen();//Listen
}

void CServerSocket::OnAccept(int nErrorCode) 
{
	m_pWnd->SendMessage(UM_ACCEPT); // Ŭ���̾�Ʈ�� ���� �� �θ� Ŭ������ 
									// �����޼����� ���� �ڵ鷯 �޼����� �����ϵ��� ��

	CSocket::OnAccept(nErrorCode);
}

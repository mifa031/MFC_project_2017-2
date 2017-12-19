
#include "stdafx.h"
#include "NetworkChess.h"
#include "GameSocket.h"

CGameSocket::CGameSocket()
{
	m_pWnd = NULL;	//������ ������ �ʱ�ȭ
	m_pFile = NULL; //���� ������ �ʱ�ȭ
	m_pArchiveIn = NULL; //��ī�̺� ������(��) �ʱ�ȭ
	m_pArchiveOut = NULL;//��ī�̺� ������(��) �ʱ�ȭ
}

CGameSocket::~CGameSocket()
{
	if ( m_pFile != NULL )
	{
		delete m_pFile; //���� ������ ����
		m_pFile = NULL;
	}

	if ( m_pArchiveIn != NULL )
	{
		m_pArchiveIn->Abort();
		delete m_pArchiveIn;//��ī�̺� ������(��) ����
		m_pArchiveIn = NULL;
	}

	if ( m_pArchiveOut != NULL )
	{
		m_pArchiveOut->Abort();
		delete m_pArchiveOut; //��ī�̺� ������(�ƿ�) ����
		m_pArchiveOut = NULL;
	}
}



#if 0
BEGIN_MESSAGE_MAP(CGameSocket, CSocket)
END_MESSAGE_MAP()
#endif


void CGameSocket::Init(CWnd *pWnd)
{// ��� ȯ���� ����
	m_pWnd = pWnd;					 // �θ� ������ Ŭ������ ��ü�� ������
	m_pFile = new CSocketFile(this); // ���� ������ ���� ������ �̿��Ͽ� ������
	m_pArchiveIn = new CArchive(m_pFile, CArchive::load);// ���� ������ ���� �����͸� �޾� ��  CArchiveŬ���� ������
	m_pArchiveOut = new CArchive(m_pFile, CArchive::store); // ���� ������ ���� �����͸� ����  CArchiveŬ���� ������
}

void CGameSocket::Send(CData *pData)//�߿�! �۽� ��Ʈ
{													
	if ( m_pArchiveOut != NULL )
	{										
		pData->Serialize(*m_pArchiveOut); // CDataŬ������ Serialize�Լ��� ���ؼ� CArchiveŬ������ ���� �����͸� ����
	}
}

void CGameSocket::Receive(CData *pData)
{
	if ( m_pArchiveIn != NULL )
	{
		pData->Serialize(*m_pArchiveIn);// CDataŬ������ Serialize�Լ��� ���ؼ� CArchiveŬ������ ���� �����͸� ����
	}
}

void CGameSocket::Flush()
{
	if ( m_pArchiveOut != NULL )
	{
		m_pArchiveOut->Flush();// �����͸� ������ CArchiveŬ������ ���۸� ���.
	}
}

void CGameSocket::OnReceive(int nErrorCode) 
{	// �����Ͱ� ���� �߻��ϴ� �޼��� �ڵ鷯 �Լ�
	// �θ� Ŭ������ ���� �޼����� ���� �޼��� �ڵ鷯�� ���� �����͸� ���� �� �ֵ��� ��
	
	m_pWnd->SendMessage(UM_DATARECEIVE);

	CSocket::OnReceive(nErrorCode);
}

void CGameSocket::OnClose(int nErrorCode) 
{	// ������ �����鼭 ���� �޸𸮸� ����
	// TODO: Add your specialized code here and/or call the base class
	if ( m_pFile != NULL )
	{
		delete m_pFile;//���� ������ ����
		m_pFile = NULL;
	}

	if ( m_pArchiveIn != NULL )
	{
		m_pArchiveIn->Abort();
		delete m_pArchiveIn;//��ī�̺� ������(��) ����
		m_pArchiveIn = NULL;
	}

	if ( m_pArchiveOut != NULL )
	{
		m_pArchiveOut->Abort();
		delete m_pArchiveOut;//��ī�̺� ������(�ƿ�) ����
		m_pArchiveOut = NULL;
	}

	CSocket::OnClose(nErrorCode);
}

void CGameSocket::operator <<(CData &data)
{
	Send(&data);// ������ �����ε��� ���� ���� �޼����� ���� �� �ֵ��� ��
	Flush();

}

void CGameSocket::operator >>(CData &data)
{
	Receive(&data); // ������ �����ε��� ���� ���� �޼����� ���� �� �ֵ��� ��
}


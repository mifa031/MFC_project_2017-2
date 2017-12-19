
#include "Data.h"
#include "DefineWords.h"


class CGameSocket : public CSocket
{ // ���ӿ��� ����� ���� ����
public :
	CGameSocket();
	virtual ~CGameSocket();
	
	void Init(CWnd *pWnd);
	void Send(CData *pData);//���� ���� �۽�
	void Receive(CData *pData);//���� ���� ����
	void Flush();//��ī�̺� ���� flush�Լ�
	void operator <<(CData &data);//����(������)
	void operator >>(CData &data);//�۽�(������)


public :

public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	



private :
	CWnd		*m_pWnd;
	CSocketFile *m_pFile;
	CArchive	*m_pArchiveIn;
	CArchive	*m_pArchiveOut;

};




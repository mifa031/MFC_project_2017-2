
#include "Data.h"
#include "DefineWords.h"


class CGameSocket : public CSocket
{ // 게임에서 사용할 소켓 집합
public :
	CGameSocket();
	virtual ~CGameSocket();
	
	void Init(CWnd *pWnd);
	void Send(CData *pData);//게임 소켓 송신
	void Receive(CData *pData);//게임 소켓 수신
	void Flush();//아카이브 비우는 flush함수
	void operator <<(CData &data);//수신(구분자)
	void operator >>(CData &data);//송신(구분자)


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




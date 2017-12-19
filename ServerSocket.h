#include <cstring>
#include <cstringt.h>
#include "DefineWords.h"


class CServerSocket : public CSocket
{// 서버가 Listen 하기 위한 소켓 클래스
public :
	CServerSocket();
	virtual ~CServerSocket();
	
	void Init(CWnd *pWnd, int nPortNum);

public :
	
public:
	virtual void OnAccept(int nErrorCode);

private :
	CWnd *m_pWnd;

};


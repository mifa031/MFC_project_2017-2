#include <cstring>
#include <cstringt.h>
#include "DefineWords.h"


class CServerSocket : public CSocket
{// ������ Listen �ϱ� ���� ���� Ŭ����
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


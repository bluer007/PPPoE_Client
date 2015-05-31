
#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include <windows.h>
#include <tchar.h>
#include <Ras.h>
#include <map> 
#include <raserror.h>
#include "Def.h"
#pragma comment(lib,"rasapi32.lib")

class CAccount
{
public:
	CAccount(TCHAR user[], TCHAR password[], TCHAR entryName[]);
	CAccount();
	~CAccount();

	DWORD connet();
	DWORD disconnet();
	BOOL isConnect();
	INT getCurConnect(tString& connectName);
	INT getAllConnect(TCHAR connectName[][MAX_ENTRY_NAME], INT connectNum);

	std::map<tString, Account> m_accountMap;

	tString m_user;					//�û���
	tString m_password;				//����
	tString m_entryName;			//�����ӿ�
	static VOID WINAPI RAScallback(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError);

private:
	static HRASCONN handle;			//�������
	static BOOL m_isConnect;		//�Ƿ񲦺�
};



#endif /* _ACCOUNT_H_ */
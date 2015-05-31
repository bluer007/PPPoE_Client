
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

	tString m_user;					//用户名
	tString m_password;				//密码
	tString m_entryName;			//上网接口
	static VOID WINAPI RAScallback(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError);

private:
	static HRASCONN handle;			//上网句柄
	static BOOL m_isConnect;		//是否拨号
};



#endif /* _ACCOUNT_H_ */
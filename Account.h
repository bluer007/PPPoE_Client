#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_


#include <windows.h>
#include <tchar.h>
#include <Ras.h>
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
	INT getCurConnect(PTCHAR connectName, INT size);
	INT getAllConnect(TCHAR connectName[][MAX_ENTRY_NAME], INT connectNum);
	//static VOID(WINAPI *RASDIALFUNC)(UINT, RASCONNSTATE, DWORD)

	TCHAR m_user[MAX_USER];					//�û���
	TCHAR m_password[MAX_PASS_WORD];		//����
	TCHAR m_entryName[MAX_ENTRY_NAME];		//�����ӿ�
	static VOID WINAPI RAScallback(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError);



private:
	HRASCONN handle;		//�������
};



#endif /* _ACCOUNT_H_ */
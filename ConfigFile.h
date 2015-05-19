/*
	配置文件类声明CConfigFile
*/


#ifndef _CONFIGFILE_H_
#define _CONFIGFILE_H_

#include <windows.h>
#include "Def.h"



struct Account
{
	TCHAR user[MAX_USER];				//用户名
	TCHAR password[MAX_PASS_WORD];		//密码
	TCHAR entryName[MAX_ENTRY_NAME];	//上网接口名称
};

struct Config
{
	BOOL autoStart;				//是否开机运行
	BOOL autoConnect;			//是否自动连接
	INT accountNum;				//帐号数量
	INT defaultAccount;			//默认帐号编号
};


class CConfigFile
{
public:
	CConfigFile(LPCTSTR fileName = NULL);
	~CConfigFile();
	INT GetConfig(LPCTSTR fileName = NULL);
	INT SetConfig();
	INT WriteConfig(LPCTSTR fileName = NULL);

private:
	INT Encrypt();		//加密	[ɪnˈkrɪpt]
	INT Decrypt();		//解密	[di:'krɪpt]
	TCHAR m_configFile[20];
	Account* m_accountList;
	INT m_accountNum;
	Config* m_Config;
};





#endif _CONFIGFILE_H_
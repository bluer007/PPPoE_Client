/*
	全局常量定义
*/

#ifndef _DEF_H_
#define _DEF_H_

#include <windows.h>
#include <string>

const INT MAX_USER = 30;								//用户名称的最长字符限制
const INT MAX_PASS_WORD = 30;							//密码的最长字符限制
const INT MAX_ENTRY_NAME = 30;							//上网接口名称的最长字符限制
const TCHAR DEFAULT_ENTRY[] = TEXT("计协上网");			//默认上网接口名称
const TCHAR WIN_NAME[] = TEXT("计协电信客户端");			//程序窗口名称
const TCHAR CONFIG_FILE[] = TEXT("pppoe_client.config");	//配置文件的默认名字

typedef std::basic_string<TCHAR> tString;

typedef struct tagAccount
{
	TCHAR user[MAX_USER];				//用户名
	TCHAR password[MAX_PASS_WORD];		//密码
	TCHAR entryName[MAX_ENTRY_NAME];	//上网接口名称
}Account, *PAccount;

typedef struct tagConfig
{
	BOOL autoStart;				//是否开机运行
	BOOL autoConnect;			//是否自动连接
	INT accountNum;				//帐号数量
	INT defaultAccount;			//默认帐号编号
}Config, *PConfig;

#endif /* _DEF_H_ */
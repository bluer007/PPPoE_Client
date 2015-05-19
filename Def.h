/*
	全局常量定义
*/
#ifndef _DEF_H_
#define _DEF_H_

#include <windows.h>


const INT MAX_USER = 30;								//用户名称的最长字符限制
const INT MAX_PASS_WORD = 30;							//密码的最长字符限制
const INT MAX_ENTRY_NAME = 30;							//上网接口名称的最长字符限制
const PTCHAR defaultEntryName = TEXT("计协上网");			//默认上网接口名称
const PTCHAR winName = TEXT("计协电信客户端");				//程序窗口名称
const PTCHAR configFile = TEXT("pppoe_client.config");	//配置文件的默认名字



#endif /* _DEF_H_ */
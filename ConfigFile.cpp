/*
	配置文件类定义CConfigFile
*/

#include "ConfigFile.h"
#include "Account.h"
#include <tchar.h>
#include <windows.h>
#include <map>

extern CAccount account;

CConfigFile::CConfigFile(LPCTSTR fileName /*= NULL*/)
{
	this->m_configFile.assign(fileName? fileName : CONFIG_FILE);
	
	TCHAR tmp_str[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, tmp_str);
	this->m_filePath.assign(tmp_str);
	this->m_filePath.append(_T("\\"));
	this->m_filePath.append(this->m_configFile);

	this->m_Config = new Config();
}

CConfigFile::~CConfigFile()
{
	if (this->m_Config) 
		delete m_Config;
}


INT CConfigFile::GetConfig()
{
	//成功返回获得有效帐号的数目, 故返回非负数
	//失败返回-1
	//Account tmp_account1 = { _T("zjDSLhdjg150428"),_T("20072008"), _T("计协上网")};
	//if (!WritePrivateProfileStruct(_T("Config"), _T("Account1"),
	//	&tmp_account1, sizeof(Account), this->m_filePath.c_str()))
	//{
	//	return -1;
	//}
	//Account tmp_account2 = { _T("zjlanhdxs285457jj"),_T("1234567890"),(TCHAR)(DEFAULT_ENTRY) };
	//if (!WritePrivateProfileStruct(_T("Config"), _T("Account2"),
	//	&tmp_account2, sizeof(Account), this->m_filePath.c_str()))
	//{
	//	return -1;
	//}

	if (!GetPrivateProfileStruct(_T("Config"), _T("Config"), 
		this->m_Config, sizeof(Config), this->m_filePath.c_str()))
	{
		return -1;
	}

	if (this->m_Config->accountNum < 0)
		return -1;

	TCHAR key[] = _T("Account1");
	Account tmp_account = { 0 };
	for (int i = this->m_Config->accountNum; i; i--)
	{
		_stprintf_s(key, 9, _T("Account%d"), i);
		if (!GetPrivateProfileStruct(_T("Config"), key,
			&tmp_account, sizeof(Account), this->m_filePath.c_str()))
			return -1;
		//以用户名做key值
		account.m_accountMap[tmp_account.user] = tmp_account;
	}

	return this->m_Config->accountNum;
}

INT CConfigFile::SetConfig(Config& config)
{
	/*
	BOOL autoStart;				//是否开机运行
	BOOL autoConnect;			//是否自动连接
	INT accountNum;				//帐号数量
	INT defaultAccount;			//默认帐号编号
	*/
	if (!GetPrivateProfileStruct(_T("Config"), _T("Config"),
		&config, sizeof(Config), this->m_filePath.c_str()))
		return FALSE;

	return TRUE;
}

INT CConfigFile::SetAccount(Account& account, int num)
{
	if (num <= 0)return FALSE;
	
	TCHAR key[] = _T("Account1");
	_stprintf_s(key, 9, _T("Account%d"), num);

	if (!WritePrivateProfileStruct(_T("Config"), key,
		&account, sizeof(Account), this->m_filePath.c_str()))
		return FALSE;
	
	return TRUE;
}




/*
//日后扩展使用
INT CConfigFile::Encrypt()		//加密	[ɪnˈkrɪpt]
{

	return TRUE;
}

INT CConfigFile::Decrypt()		//解密	[di:'krɪpt]
{

	return TRUE;
}
*/



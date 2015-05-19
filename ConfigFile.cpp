/*
	配置文件类定义CConfigFile
*/

#include "ConfigFile.h"
#include <tchar.h>
#include <windows.h>

CConfigFile::CConfigFile(LPCTSTR fileName /*= NULL*/)
{
	if(fileName)
		_tcscpy_s(this->m_configFile, _tcslen(this->m_configFile), fileName);
	else
		_tcscpy_s(this->m_configFile, _tcslen(this->m_configFile), configFile);

	this->m_accountList = NULL;
	this->m_Config = new Config();
}

CConfigFile::~CConfigFile()
{
	if (this->m_accountList)
		delete[] m_accountList;
	if (this->m_Config)
		delete m_Config;
}


INT CConfigFile::GetConfig(LPCTSTR fileName /*= NULL*/)
{
	//成功返回获得有效帐号的数目, 故返回非负数
	//失败返回-1
	TCHAR tmp_fileName[20], tmp_path[MAX_PATH];
	_tcscpy_s(tmp_fileName, 20, fileName? fileName: configFile);

	GetCurrentDirectory(MAX_PATH, tmp_path);
	_tcscat_s(tmp_path, MAX_PATH, _T("\\"));
	_tcscat_s(tmp_path, MAX_PATH, tmp_fileName);
	if (!GetPrivateProfileStruct(_T("Config"), _T("Config"), 
		this->m_Config, sizeof(Config), tmp_path))
	{
		return -1;
	}
	
	TCHAR tmp_account[] = _T("Account1");
	if (this->m_Config->accountNum > 0)
	{
		this->m_accountNum = this->m_Config->accountNum;
		this->m_accountList = new Account[this->m_Config->accountNum]();
	}
	for (int i = this->m_Config->accountNum; i; i--)
	{
		_stprintf_s(tmp_account, 9, _T("Account%d"), i);
		if (!GetPrivateProfileStruct(_T("Config"), tmp_account, 
			&this->m_accountList[i - 1], sizeof(Account), tmp_path))
			return -1;
	}

	return this->m_Config->accountNum;
}

INT CConfigFile::SetConfig()
{
	return TRUE;
}

INT CConfigFile::WriteConfig(LPCTSTR fileName /*= NULL*/)
{

	return TRUE;
}


INT CConfigFile::Encrypt()		//加密	[ɪnˈkrɪpt]
{

	return TRUE;
}

INT CConfigFile::Decrypt()		//解密	[di:'krɪpt]
{

	return TRUE;
}




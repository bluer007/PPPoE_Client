/*
	配置文件类声明CConfigFile
*/


#ifndef _CONFIGFILE_H_
#define _CONFIGFILE_H_

#include <windows.h>
#include <string>
#include "Def.h"
using namespace std;

class CConfigFile
{
public:
	CConfigFile(LPCTSTR fileName = NULL);
	~CConfigFile();
	INT GetConfig();
	INT SetConfig(Config& config);
	INT SetAccount(Account& account, int num);
	PConfig m_Config;
	
private:
	tString m_configFile;
	tString m_filePath;


	//INT Encrypt();		//加密	[ɪnˈkrɪpt]
	//INT Decrypt();		//解密	[di:'krɪpt]
};





#endif _CONFIGFILE_H_
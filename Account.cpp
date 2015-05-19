#include "Account.h"

CAccount::CAccount(TCHAR user[], TCHAR password[], TCHAR entryName[])
{
	_tcscpy_s(m_user, (_tcslen(user)+1), user);
	_tcscpy_s(m_password, _tcslen(password)+1, password);
	_tcscpy_s(m_entryName, (_tcslen(entryName) + 1), entryName);

	handle = NULL;
}

CAccount::CAccount()
{
	memset((this->m_user), 0, sizeof(m_user));
	memset((this->m_password), 0, sizeof(m_password));
	memset((this->m_entryName), 0, sizeof(m_entryName));
	handle = NULL;
}

CAccount::~CAccount()
{
	
}

DWORD CAccount::connet()
{
	if (!*(this->m_entryName) || !*(this->m_password) || !*(this->m_user))
		return FALSE;

	RASDIALPARAMS params = { 0 };
	params.dwSize = sizeof(RASDIALPARAMS);
	_tcscpy_s(params.szEntryName, this->m_entryName);	//上网接口
	_tcscpy_s(params.szUserName, this->m_user);			//用户名
	_tcscpy_s(params.szPassword, this->m_password);		//密码

	//开始拨号连接
	DWORD res = RasDial(NULL, NULL, &params, NULL, CAccount::RAScallback, &handle);

	return res;
}

DWORD CAccount::disconnet()
{
	DWORD res = FALSE;
	if (handle)
	{
		res = RasHangUp(handle);
		handle = NULL;
	}
	return res;
}

INT CAccount::getCurConnect(PTCHAR connectName, INT size)
{
	RASCONNSTATUS RasConnectStatus[2];
	RASCONN RasConnect[2];
	RasConnectStatus[0].dwSize = sizeof(RASCONNSTATUS);
	RasConnect[0].dwSize = sizeof(RASCONN);
	DWORD dSize, dNumber;
	dSize = sizeof(RasConnect);
	if (RasEnumConnections(RasConnect, &dSize, &dNumber) == 0)
	{
		//win10_x64_10074版本测试, 只用wifi上网, RasEnumConnections都返回0
		if (RasGetConnectStatus(RasConnect[0].hrasconn, RasConnectStatus) == 0)
			if (RasConnectStatus[0].rasconnstate == RASCS_Connected)
			{
				if (_tcslen(RasConnect[0].szEntryName) >= (UINT)size)
					return FALSE;
				
				_tcscpy_s(connectName, size, RasConnect[0].szEntryName);
				return TRUE;
			}
	}

	return FALSE;
}

INT CAccount::getAllConnect(TCHAR connectName[][MAX_ENTRY_NAME], INT connectNum)
{
	//成功返回0, 因传入的连接不够大返回应传入的连接数目, 其他原因失败返回-1
	INT res = -1;
	DWORD dwCb = 0;
	DWORD dwRet = ERROR_SUCCESS;
	DWORD dwEntries = 0;
	LPRASENTRYNAME lpRasEntryName = NULL;

	// 用lpRasEntryName = NULL 来调用 RasEnumEntries, 其中dwCb是一个传出值, 用来返回成功调用所需的缓冲区的字节数.  
	dwRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &dwCb, &dwEntries);
	// 函数成功返回0  
	if (dwRet == ERROR_BUFFER_TOO_SMALL) 
	{
		// 分配遍历条目所需要的字节输  
		lpRasEntryName = (LPRASENTRYNAME)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);
		// 如果lpRasEntryName指针为NULL, 则说明分配内存失败  
		if (lpRasEntryName == NULL) 
		{
			MessageBox(NULL, _T("getAllConnect()分配内存失败!"), _T("错误"), MB_OK);
			res = -1;
		}

		// 数组中第一个 RASENRTYNAME 结构必须包含结构体的大小  
		lpRasEntryName[0].dwSize = sizeof(RASENTRYNAME);

		// 调用 RasEnumEntries 枚举所有的连接名称  
		dwRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &dwCb, &dwEntries);

		// 如果调用成功, 打印出每个连接的名称  
		if (ERROR_SUCCESS == dwRet) 
		{
			if (dwEntries > connectNum)
			{
				res = dwEntries;			//返回正常调用时候应传入的连接数目
				goto FINAL;
			}
			for (DWORD i = 0; i < dwEntries; i++)
			{
				//复制本机上网接口名称
				_tcscpy_s(connectName[i], MAX_ENTRY_NAME + 1, lpRasEntryName[i].szEntryName);
			}
			res = 0;
		}
		else
		{
			res = -1;
		}
	}
	else 
	{
		// 枚举连接名称出现的问题  
		res = -1;
	}

FINAL:
	// 释放用于存放连接名称的内存  
	if (lpRasEntryName)
	{
		HeapFree(GetProcessHeap(), 0, lpRasEntryName);
		// 赋值空指针  
		lpRasEntryName = NULL;
	}

	return res;
}


VOID WINAPI CAccount::RAScallback(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError)
{

	//RasGetErrorString(a, bb, 50);

	//lresult cdialinfo::onrasdialevent(wparam wp, lparam lp)
	//	rasconnstate rasstate = (rasconnstate)wp;
	//clistbox ＊info = (clistbox ＊)getdlgitem(idc_infolist);
	switch (rasconnstate)
	{
	case RASCS_OpenPort:
		//info→addstring(_t(″打开端口……″));
		break;
	case RASCS_PortOpened:
		//info→addstring(_t(″端口已打开.″));
		break;
	case RASCS_ConnectDevice:
		//info→addstring(_t(″连接设备……″));
		break;
	case RASCS_DeviceConnected:
		//info→addstring(_t(″设备已连接.″));
		break;
	case RASCS_Authenticate:
		//info→addstring(_t(″验证用户及密码″));
		break;
	case RASCS_Authenticated:
		//info→addstring(_t(″通过″));
		break;
	case RASCS_Connected:
		//info－ > addstring(_t(″已连接″));
		break;
	case RASCS_Disconnected:
		//info－ > addstring(_t(″连接已断开″));
		//m_hrasconn = null;
		break;
	}

}
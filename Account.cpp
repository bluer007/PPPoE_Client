#include "Account.h"

CAccount::CAccount(TCHAR user[], TCHAR password[], TCHAR entryName[])
{
	this->m_user.assign(user);
	this->m_password.assign(password);
	this->m_entryName.assign(entryName);
}

HRASCONN CAccount::handle = NULL;			//上网句柄
BOOL CAccount::m_isConnect = FALSE;			//是否拨号

CAccount::CAccount()
{
	this->m_user = _T("");
	this->m_password = _T("");
	this->m_entryName = DEFAULT_ENTRY;		//使用 默认接口名称
}

CAccount::~CAccount()
{
	
}

BOOL CAccount::isConnect()
{
	return this->m_isConnect;
}

DWORD CAccount::connet()
{
	if (!(this->m_entryName.c_str()) || !(this->m_password.c_str()) || !(this->m_user.c_str()))
		return FALSE;

	if (this->m_isConnect)
		this->disconnet();

	RASENTRY rasEntry;
	memset(&rasEntry, 0, sizeof(RASENTRY));

	rasEntry.dwSize = sizeof(RASENTRY);
	rasEntry.dwfOptions = RASEO_RemoteDefaultGateway | RASEO_ShowDialingProgress | RASEO_ModemLights;
	//显示连接进度//连接后在通知区域显示图标;	RASEO_PreviewUserPw | 																
	rasEntry.dwfOptions = ~RASEO_RequireEncryptedPw & rasEntry.dwfOptions;//&~RASEO_PreviewUserPw;
	rasEntry.dwfOptions2 = RASEO2_DontNegotiateMultilink;
	rasEntry.dwfOptions2 = RASEO2_SecureFileAndPrint | RASEO2_SecureClientForMSNet | rasEntry.dwfOptions2;
	rasEntry.dwEncryptionType = ET_Optional;
	rasEntry.dwfNetProtocols = RASNP_Ip;
	rasEntry.dwFramingProtocol = RASFP_Ppp;

#if (WINVER >= 0x600)
	//我擦，我是说为什么编译不过呢，原来是没有加预编译条件，尼玛，能别搞这么难看吗
	rasEntry.dwfNetProtocols = rasEntry.dwfNetProtocols | RASNP_Ipv6;
#endif

	rasEntry.dwType = RASET_Broadband;
	lstrcpy(rasEntry.szDeviceType, RASDT_PPPoE);
	lstrcpy(rasEntry.szDeviceName, _T("WAN 微型端口(PPPOE)"));
	//新建和修改上网接口
	RasSetEntryProperties(NULL, this->m_entryName.c_str(), &rasEntry, sizeof(RASENTRY), NULL, 0);

	//修改上网接口的帐号,密码
	RASCREDENTIALS rascredentials = { 0 };
	rascredentials.dwSize = sizeof(RASCREDENTIALS);
	rascredentials.dwMask = RASCM_UserName | RASCM_Password;
	lstrcpy(rascredentials.szUserName, this->m_user.c_str());
	lstrcpy(rascredentials.szPassword, this->m_password.c_str());
	RasSetCredentials(NULL, this->m_entryName.c_str(), &rascredentials, FALSE);


	RASDIALPARAMS params = { 0 };
	params.dwSize = sizeof(RASDIALPARAMS);
	_tcscpy_s(params.szEntryName, RAS_MaxEntryName + 1, this->m_entryName.c_str());	//上网接口
	_tcscpy_s(params.szUserName, UNLEN + 1, this->m_user.c_str());			//用户名
	_tcscpy_s(params.szPassword, PWLEN + 1, this->m_password.c_str());		//密码

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
		if(res) this->m_isConnect = FALSE;
	}
	return res;
}

INT CAccount::getCurConnect(tString& connectName)
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
				connectName.assign(RasConnect[0].szEntryName);
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
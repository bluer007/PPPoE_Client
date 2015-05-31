#include "Account.h"

CAccount::CAccount(TCHAR user[], TCHAR password[], TCHAR entryName[])
{
	this->m_user.assign(user);
	this->m_password.assign(password);
	this->m_entryName.assign(entryName);
}

HRASCONN CAccount::handle = NULL;			//�������
BOOL CAccount::m_isConnect = FALSE;			//�Ƿ񲦺�

CAccount::CAccount()
{
	this->m_user = _T("");
	this->m_password = _T("");
	this->m_entryName = DEFAULT_ENTRY;		//ʹ�� Ĭ�Ͻӿ�����
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
	//��ʾ���ӽ���//���Ӻ���֪ͨ������ʾͼ��;	RASEO_PreviewUserPw | 																
	rasEntry.dwfOptions = ~RASEO_RequireEncryptedPw & rasEntry.dwfOptions;//&~RASEO_PreviewUserPw;
	rasEntry.dwfOptions2 = RASEO2_DontNegotiateMultilink;
	rasEntry.dwfOptions2 = RASEO2_SecureFileAndPrint | RASEO2_SecureClientForMSNet | rasEntry.dwfOptions2;
	rasEntry.dwEncryptionType = ET_Optional;
	rasEntry.dwfNetProtocols = RASNP_Ip;
	rasEntry.dwFramingProtocol = RASFP_Ppp;

#if (WINVER >= 0x600)
	//�Ҳ�������˵Ϊʲô���벻���أ�ԭ����û�м�Ԥ�������������꣬�ܱ����ô�ѿ���
	rasEntry.dwfNetProtocols = rasEntry.dwfNetProtocols | RASNP_Ipv6;
#endif

	rasEntry.dwType = RASET_Broadband;
	lstrcpy(rasEntry.szDeviceType, RASDT_PPPoE);
	lstrcpy(rasEntry.szDeviceName, _T("WAN ΢�Ͷ˿�(PPPOE)"));
	//�½����޸������ӿ�
	RasSetEntryProperties(NULL, this->m_entryName.c_str(), &rasEntry, sizeof(RASENTRY), NULL, 0);

	//�޸������ӿڵ��ʺ�,����
	RASCREDENTIALS rascredentials = { 0 };
	rascredentials.dwSize = sizeof(RASCREDENTIALS);
	rascredentials.dwMask = RASCM_UserName | RASCM_Password;
	lstrcpy(rascredentials.szUserName, this->m_user.c_str());
	lstrcpy(rascredentials.szPassword, this->m_password.c_str());
	RasSetCredentials(NULL, this->m_entryName.c_str(), &rascredentials, FALSE);


	RASDIALPARAMS params = { 0 };
	params.dwSize = sizeof(RASDIALPARAMS);
	_tcscpy_s(params.szEntryName, RAS_MaxEntryName + 1, this->m_entryName.c_str());	//�����ӿ�
	_tcscpy_s(params.szUserName, UNLEN + 1, this->m_user.c_str());			//�û���
	_tcscpy_s(params.szPassword, PWLEN + 1, this->m_password.c_str());		//����

	//��ʼ��������
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
		//win10_x64_10074�汾����, ֻ��wifi����, RasEnumConnections������0
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
	//�ɹ�����0, ��������Ӳ����󷵻�Ӧ�����������Ŀ, ����ԭ��ʧ�ܷ���-1
	INT res = -1;
	DWORD dwCb = 0;
	DWORD dwRet = ERROR_SUCCESS;
	DWORD dwEntries = 0;
	LPRASENTRYNAME lpRasEntryName = NULL;
	
	// ��lpRasEntryName = NULL ������ RasEnumEntries, ����dwCb��һ������ֵ, �������سɹ���������Ļ��������ֽ���.  
	dwRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &dwCb, &dwEntries);
	// �����ɹ�����0  
	if (dwRet == ERROR_BUFFER_TOO_SMALL) 
	{
		// ���������Ŀ����Ҫ���ֽ���  
		lpRasEntryName = (LPRASENTRYNAME)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);
		// ���lpRasEntryNameָ��ΪNULL, ��˵�������ڴ�ʧ��  
		if (lpRasEntryName == NULL) 
		{
			MessageBox(NULL, _T("getAllConnect()�����ڴ�ʧ��!"), _T("����"), MB_OK);
			res = -1;
		}

		// �����е�һ�� RASENRTYNAME �ṹ��������ṹ��Ĵ�С  
		lpRasEntryName[0].dwSize = sizeof(RASENTRYNAME);

		// ���� RasEnumEntries ö�����е���������  
		dwRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &dwCb, &dwEntries);

		// ������óɹ�, ��ӡ��ÿ�����ӵ�����  
		if (ERROR_SUCCESS == dwRet) 
		{
			if (dwEntries > connectNum)
			{
				res = dwEntries;			//������������ʱ��Ӧ�����������Ŀ
				goto FINAL;
			}
			for (DWORD i = 0; i < dwEntries; i++)
			{
				//���Ʊ��������ӿ�����
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
		// ö���������Ƴ��ֵ�����  
		res = -1;
	}

FINAL:
	// �ͷ����ڴ���������Ƶ��ڴ�  
	if (lpRasEntryName)
	{
		HeapFree(GetProcessHeap(), 0, lpRasEntryName);
		// ��ֵ��ָ��  
		lpRasEntryName = NULL;
	}

	return res;
}


VOID WINAPI CAccount::RAScallback(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError)
{

	//RasGetErrorString(a, bb, 50);

	//lresult cdialinfo::onrasdialevent(wparam wp, lparam lp)
	//	rasconnstate rasstate = (rasconnstate)wp;
	//clistbox ��info = (clistbox ��)getdlgitem(idc_infolist);
	switch (rasconnstate)
	{
	case RASCS_OpenPort:
		//info��addstring(_t(��򿪶˿ڡ�����));
		break;
	case RASCS_PortOpened:
		//info��addstring(_t(��˿��Ѵ�.��));
		break;
	case RASCS_ConnectDevice:
		//info��addstring(_t(�������豸������));
		break;
	case RASCS_DeviceConnected:
		//info��addstring(_t(���豸������.��));
		break;
	case RASCS_Authenticate:
		//info��addstring(_t(����֤�û��������));
		break;
	case RASCS_Authenticated:
		//info��addstring(_t(��ͨ����));
		break;
	case RASCS_Connected:
		//info�� > addstring(_t(�������ӡ�));
		
		break;
	case RASCS_Disconnected:
		//info�� > addstring(_t(�������ѶϿ���));
		//m_hrasconn = null;
		break;
	}

}
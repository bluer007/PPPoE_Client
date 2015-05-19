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
	_tcscpy_s(params.szEntryName, this->m_entryName);	//�����ӿ�
	_tcscpy_s(params.szUserName, this->m_user);			//�û���
	_tcscpy_s(params.szPassword, this->m_password);		//����

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
		//win10_x64_10074�汾����, ֻ��wifi����, RasEnumConnections������0
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
/*
������Ķ���--���Ƴ����߼�
*/


#include "Control.h"

INT CControl::SetAutoRun(BOOL isAutoRun)
{
	//����true��ʾ  �ɹ���������->������ ���� ������->��������
	//ʧ�� ���� ������->������ ���� ��������->�������� ����false

	TCHAR path[MAX_PATH];
	HKEY hSoftKey = NULL;
	INT res = FALSE;

	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 
		0, KEY_ALL_ACCESS, &hSoftKey))
		return FALSE;

	GetModuleFileName(NULL, path, MAX_PATH);
	
	if (isAutoRun)
	{
		if (ERROR_SUCCESS == RegSetValueEx(hSoftKey,
			TEXT("pppoe_client"),
			NULL, REG_SZ, (const BYTE*)path, MAX_PATH))
			res = TRUE;
	}
	else
	{
		if (ERROR_SUCCESS == RegDeleteValue(hSoftKey, TEXT("pppoe_client")))
			res = TRUE;
	}

FINAL:
	if (hSoftKey)RegCloseKey(hSoftKey);
	return res;
}


INT CControl::IsAutoRun()
{
	//����true, ���Ѿ��ǿ������е�����
	//����false, ������ϵͳ���ǿ��������е�����
	//����-1, ������������

	HKEY hkey = NULL;
	INT res = -1;
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
		0, KEY_ALL_ACCESS, &hkey))
		res = -1;

	TCHAR data[MAX_PATH] = {0};
	DWORD size = MAX_PATH;
	//��ѯע���ֵ
	DWORD resQuery = RegQueryValueEx(hkey, TEXT("pppoe_client"), NULL, NULL, (LPBYTE)&data, &size);
	//���û�в�ѯ��, ��false
	if (ERROR_FILE_NOT_FOUND == resQuery)
	{
		res = FALSE;
		goto FINAL;
	}
	if (ERROR_SUCCESS == resQuery)
	{
		TCHAR path[MAX_PATH];
		GetModuleFileName(NULL, path, MAX_PATH);
		if (0 == _tcscmp(path, data))
			res = TRUE;
		else
		{
			res = FALSE;
			goto FINAL;
		}
	}
	else
		res = -1;

FINAL:
	if (hkey)RegCloseKey(hkey);
	return res;
}
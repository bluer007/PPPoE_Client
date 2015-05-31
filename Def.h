/*
	ȫ�ֳ�������
*/

#ifndef _DEF_H_
#define _DEF_H_

#include <windows.h>
#include <string>

const INT MAX_USER = 30;								//�û����Ƶ���ַ�����
const INT MAX_PASS_WORD = 30;							//�������ַ�����
const INT MAX_ENTRY_NAME = 30;							//�����ӿ����Ƶ���ַ�����
const TCHAR DEFAULT_ENTRY[] = TEXT("��Э����");			//Ĭ�������ӿ�����
const TCHAR WIN_NAME[] = TEXT("��Э���ſͻ���");			//���򴰿�����
const TCHAR CONFIG_FILE[] = TEXT("pppoe_client.config");	//�����ļ���Ĭ������

typedef std::basic_string<TCHAR> tString;

typedef struct tagAccount
{
	TCHAR user[MAX_USER];				//�û���
	TCHAR password[MAX_PASS_WORD];		//����
	TCHAR entryName[MAX_ENTRY_NAME];	//�����ӿ�����
}Account, *PAccount;

typedef struct tagConfig
{
	BOOL autoStart;				//�Ƿ񿪻�����
	BOOL autoConnect;			//�Ƿ��Զ�����
	INT accountNum;				//�ʺ�����
	INT defaultAccount;			//Ĭ���ʺű��
}Config, *PConfig;

#endif /* _DEF_H_ */
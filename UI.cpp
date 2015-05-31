#include "UI.h"
#include "Def.h"

INT UI::SetTaskbarIco(BOOL flag)
{
	static NOTIFYICONDATA nid = {0};
	if (FLAG_CREATE_TASKBARICO == flag)
	{
		// ��Ҫ�޸�TaskbarCreated������ϵͳ�������Զ������Ϣ, Ϊ������explorer����ʱ�ָ�ͼ��
		RegisterWindowMessage(TEXT("TaskbarCreated"));

		nid.cbSize = sizeof(NOTIFYICONDATA);
		nid.hWnd = this->m_hWnd;			//�󶨴���,���������Ϣ
		nid.uID = 20150531;					//����ͼ��ID
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nid.uCallbackMessage = WM_MYTASKBAR;	//�Զ�������ͼ����Ϣ
		nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		lstrcpy(nid.szTip, WIN_NAME);		//�ó�����������ʾ��
		//�½�����ͼ��
		return (Shell_NotifyIcon(NIM_ADD, &nid) != 0);
	}
	else if (FLAG_DELLTE_TASKBARICO == flag)
	{
		//ɾ������ͼ��
		return (Shell_NotifyIcon(NIM_DELETE, &nid) != 0);
	}
	else
		return FALSE;

}
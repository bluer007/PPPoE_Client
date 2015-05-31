#include "UI.h"
#include "Def.h"

INT UI::SetTaskbarIco(BOOL flag)
{
	static NOTIFYICONDATA nid = {0};
	if (FLAG_CREATE_TASKBARICO == flag)
	{
		// 不要修改TaskbarCreated，这是系统任务栏自定义的消息, 为了重启explorer进程时恢复图标
		RegisterWindowMessage(TEXT("TaskbarCreated"));

		nid.cbSize = sizeof(NOTIFYICONDATA);
		nid.hWnd = this->m_hWnd;			//绑定窗口,方便接收消息
		nid.uID = 20150531;					//托盘图标ID
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nid.uCallbackMessage = WM_MYTASKBAR;	//自定义托盘图标消息
		nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		lstrcpy(nid.szTip, WIN_NAME);		//用程序名称做提示语
		//新建托盘图标
		return (Shell_NotifyIcon(NIM_ADD, &nid) != 0);
	}
	else if (FLAG_DELLTE_TASKBARICO == flag)
	{
		//删除托盘图标
		return (Shell_NotifyIcon(NIM_DELETE, &nid) != 0);
	}
	else
		return FALSE;

}
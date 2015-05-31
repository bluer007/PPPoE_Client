#pragma once

#include ".\duilib\UIlib.h"
using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "./duilib/DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif

class CDuiMenu : public WindowImplBase
{
protected:
	virtual ~CDuiMenu() {};        // 私有化析构函数，这样此对象只能通过new来生成，而不能直接定义变量。就保证了delete this不会出错
	CDuiString  m_strXMLPath;

public:
	explicit CDuiMenu(LPCTSTR pszXMLPath) : m_strXMLPath(pszXMLPath) {}
	virtual LPCTSTR    GetWindowClassName()const { return _T("CDuiMenu "); }
	virtual CDuiString GetSkinFolder() { return _T(""); }
	virtual CDuiString GetSkinFile() { return m_strXMLPath; }
	virtual void       OnFinalMessage(HWND hWnd) { delete this; }

	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		Close();
		bHandled = FALSE;
		return 0;
	}

	void Init(HWND hWndParent, POINT ptPos)
	{
		Create(hWndParent, _T("MenuWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		::ClientToScreen(hWndParent, &ptPos);
		::SetWindowPos(*this, NULL, ptPos.x, ptPos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL    bHandled = TRUE;

		switch (uMsg)
		{
		case WM_KILLFOCUS:
			lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
			break;
		default:
			bHandled = FALSE;
		}

		if (bHandled || m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
		{
			return lRes;
		}

		return __super::HandleMessage(uMsg, wParam, lParam);
	}
};

enum
{
	FLAG_CREATE_TASKBARICO = 0,		//创建托盘图标标志
	FLAG_DELLTE_TASKBARICO = 1,		//删除托盘图标标志
	WM_MYTASKBAR = WM_USER + 1,		//自定义托盘图标消息
};

class UI : public WindowImplBase
{
public:
	

	INT SetTaskbarIco(BOOL flag);

	virtual LPCTSTR    GetWindowClassName() const { return _T("DUIMainFrame"); }
	virtual CDuiString GetSkinFile() { return _T("UISkin1.xml"); }
	virtual CDuiString GetSkinFolder() { return _T("alberl"); }

	CControlUI* UI::CreateControl(LPCTSTR pstrClassName)
	{
		if (_tcsicmp(pstrClassName, _T("WndPlayPanel2")) == 0)
		{
			CDialogBuilder builder;
			CControlUI* pUI = builder.Create(_T("menu.xml"));
			return pUI;
		}

		return NULL;
	}
	virtual void Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("selectchanged"))
		{
			CDuiMenu *pMenu = new CDuiMenu(_T("menu.xml"));
			POINT pt = { msg.ptMouse.x, msg.ptMouse.y };
			pMenu->Init(*this, pt);
			pMenu->ShowWindow(TRUE);
			CListUI a;
			int b = a.GetCurSel();
		}

		__super::Notify(msg);
	}

private:
	HWND m_hWnd;
};


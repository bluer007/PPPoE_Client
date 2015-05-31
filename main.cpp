//#pragma once
#include "Account.h"
#include "ConfigFile.h"
#include "UI.h"
#include "Def.h"
#include "Control.h"

//RASDIALPARAMS params;
//HRASCONN handle = NULL;

UI* ui = NULL;
CAccount account;
CConfigFile configFile;
CControl control;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	
	control.SetAutoRun(1);
	control.IsAutoRun();
	Account a =
	{
		_T("22222222222222"),
	_T("22222"),
		_T("计协上网"),
	};
	configFile.SetAccount(a, 2);
	Account B =
	{
		_T("111111111111111"),
		_T("11111"),
		_T("计协上网"),//_T("计协上网"),//
	};
	configFile.SetAccount(B, 1);
	configFile.GetConfig();
	INT allConnectNum = account.getAllConnect(NULL, 0);
	TCHAR (*allConnect)[MAX_ENTRY_NAME] = new TCHAR[allConnectNum][MAX_ENTRY_NAME];
	account.getAllConnect(allConnect, allConnectNum);
	delete[] allConnect;
	return 0;
}
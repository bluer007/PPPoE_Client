//#pragma once
#include "Account.h"
#include "UI.h"
#include "Def.h"

//RASDIALPARAMS params;
//HRASCONN handle = NULL;

UI* ui = NULL;
CAccount account;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	INT allConnectNum = account.getAllConnect(NULL, 0);
	TCHAR (*allConnect)[MAX_ENTRY_NAME] = new TCHAR[allConnectNum][MAX_ENTRY_NAME];
	account.getAllConnect(allConnect, allConnectNum);
	delete[] allConnect;
	return 0;
}
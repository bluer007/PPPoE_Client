/*
	控制类的声明--控制程序逻辑
*/

#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <windows.h>
#include "tchar.h"
#include "Def.h"

class CControl
{
public:
	CControl() {};
	~CControl() {};
	INT SetAutoRun(BOOL isAutoRun);
	INT IsAutoRun();

private:
	

};


#endif  /*_CONTROL_H_*/


#pragma once
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif
#include "resource.h"		
class CPlatformApp : public CWinApp
{
public:
	CPlatformApp();
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern CPlatformApp theApp;

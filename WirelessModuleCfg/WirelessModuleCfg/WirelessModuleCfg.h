
// WirelessModuleCfg.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CWirelessModuleCfgApp:
// See WirelessModuleCfg.cpp for the implementation of this class
//

class CWirelessModuleCfgApp : public CWinApp
{
public:
	CWirelessModuleCfgApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
    virtual int ExitInstance();
};

extern CWirelessModuleCfgApp theApp;
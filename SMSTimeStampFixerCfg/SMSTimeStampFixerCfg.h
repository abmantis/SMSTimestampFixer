// SMSTimeStampFixerCfg.h : main header file for the PROJECT_NAME application
//

#pragma once



#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif 

// CSMSTimeStampFixerCfgApp:
// See SMSTimeStampFixerCfg.cpp for the implementation of this class
//

class CSMSTimeStampFixerCfgApp : public CWinApp
{
public:
	CSMSTimeStampFixerCfgApp();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSMSTimeStampFixerCfgApp theApp;

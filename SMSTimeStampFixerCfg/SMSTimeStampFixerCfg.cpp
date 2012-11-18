// SMSTimeStampFixerCfg.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SMSTimeStampFixerCfg.h"
#include "SMSTimeStampFixerCfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSMSTimeStampFixerCfgApp

BEGIN_MESSAGE_MAP(CSMSTimeStampFixerCfgApp, CWinApp)
END_MESSAGE_MAP()


// CSMSTimeStampFixerCfgApp construction
CSMSTimeStampFixerCfgApp::CSMSTimeStampFixerCfgApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSMSTimeStampFixerCfgApp object
CSMSTimeStampFixerCfgApp theApp;

// CSMSTimeStampFixerCfgApp initialization

BOOL CSMSTimeStampFixerCfgApp::InitInstance()
{
    // SHInitExtraControls should be called once during your application's initialization to initialize any
    // of the Windows Mobile specific controls such as CAPEDIT and SIPPREF.
    SHInitExtraControls();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("SMSTimestampFixer"));

	CSMSTimeStampFixerCfgDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal(this);
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

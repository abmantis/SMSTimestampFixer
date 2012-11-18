// SMSTimeStampFixerCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SMSTimeStampFixerCfg.h"
#include "SMSTimeStampFixerCfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define DLL_FILE "\\WINDOWS\\SMSTimestampFixer.dll"
// CSMSTimeStampFixerCfgDlg dialog


BEGIN_MESSAGE_MAP(CSMSTimeStampFixerCfgDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif

	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BT_APPLY, &CSMSTimeStampFixerCfgDlg::OnBnClickedBtApply)
END_MESSAGE_MAP()


CSMSTimeStampFixerCfgDlg::CSMSTimeStampFixerCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSMSTimeStampFixerCfgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_stVars.iShowOriginalTime	= 1;
	m_stVars.iShowAfterTime		= 1;
	m_stVars.iPosition			= SMSBEGINING;
}

void CSMSTimeStampFixerCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control	(pDX, IDC_CBB_AFTER,	m_stControls.cCBBShowAfterTime);
	DDX_Control	(pDX, IDC_CBB_POSITION, m_stControls.cCBBPosition);

	DDX_Check	(pDX, IDC_CHK_SHOWTS,	m_stVars.iShowOriginalTime);
	DDX_CBIndex	(pDX, IDC_CBB_AFTER,	m_stVars.iShowAfterTime);
	DDX_CBIndex	(pDX, IDC_CBB_POSITION,	m_stVars.iPosition);
}

INT_PTR CSMSTimeStampFixerCfgDlg::DoModal(CWinApp *mainApp)
{
	m_MainApp = mainApp;

	return CDialog::DoModal();
}

void CSMSTimeStampFixerCfgDlg::FillComboBoxes()
{

	CString strAux;
	for(int i = 0; i < 60; i++)
	{
		strAux.Format(TEXT("%d"), i);
		m_stControls.cCBBShowAfterTime.AddString(strAux);
		m_stControls.cCBBShowAfterTime.SetItemData(i, i);
	}
	
	
	m_stControls.cCBBPosition.AddString(TEXT("Beginning"));
	m_stControls.cCBBPosition.AddString(TEXT("End"));
	m_stControls.cCBBPosition.SetItemData(0, SMSBEGINING);
	m_stControls.cCBBPosition.SetItemData(1, SMSEND);
}

// CSMSTimeStampFixerCfgDlg message handlers

BOOL CSMSTimeStampFixerCfgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	
	m_stVars.iShowOriginalTime = (m_MainApp->GetProfileInt
		(propRuleClientSection, propShowOriginalTime, 1) == 1) ? 1 : 0;

	m_stVars.iShowAfterTime = m_MainApp->GetProfileInt
		(propRuleClientSection, propShowOriginalTimeAfter, 1);

	m_stVars.iPosition = (m_MainApp->GetProfileInt
		(propRuleClientSection, propOriginalTimePos, 0) == 0) ? SMSBEGINING : SMSEND;

	FillComboBoxes();

	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CSMSTimeStampFixerCfgDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_SMSTIMESTAMPFIXERCFG_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_SMSTIMESTAMPFIXERCFG_DIALOG));
	}
}
#endif


void CSMSTimeStampFixerCfgDlg::OnBnClickedBtStart()
{
	/* get handle to dll */
	HINSTANCE hGetProcIDDLL = LoadLibrary(TEXT(DLL_FILE));

	if(hGetProcIDDLL == NULL)
	{
		AfxMessageBox(TEXT("Could not load the component!"));
		return;
	}

	/* get pointer to the function in the dll*/
	FARPROC lpfnGetProcessID = GetProcAddress(HMODULE (hGetProcIDDLL),TEXT("DllRegisterServer"));

	/*
	  Define the Function in the DLL for reuse. This is just prototyping the dll's function.
	  A mock of it. Use "stdcall" for maximum compatibility.
	*/
	typedef int (__stdcall * pICFUNC)(char *);

	pICFUNC MyFunction;
	MyFunction = pICFUNC(lpfnGetProcessID);

	/* The actual call to the function contained in the dll */
	int intMyReturnVal = MyFunction("DllRegisterServer");

	/* Release the Dll */
	FreeLibrary(hGetProcIDDLL); 
}

void CSMSTimeStampFixerCfgDlg::OnBnClickedBtStop()
{
	/* get handle to dll */
	HINSTANCE hGetProcIDDLL = LoadLibrary(TEXT(DLL_FILE));

	if(hGetProcIDDLL == NULL)
	{
		AfxMessageBox(TEXT("Could not load the component!"));
		return;
	}

	/* get pointer to the function in the dll*/
	FARPROC lpfnGetProcessID = GetProcAddress(HMODULE (hGetProcIDDLL),TEXT("DllUnregisterServer"));

	/*
	  Define the Function in the DLL for reuse. This is just prototyping the dll's function.
	  A mock of it. Use "stdcall" for maximum compatibility.
	*/
	typedef int (__stdcall * pICFUNC)(char *);

	pICFUNC MyFunction;
	MyFunction = pICFUNC(lpfnGetProcessID);

	/* The actual call to the function contained in the dll */
	int intMyReturnVal = MyFunction("DllUnregisterServer");

	/* Release the Dll */
	FreeLibrary(hGetProcIDDLL); 
}

void CSMSTimeStampFixerCfgDlg::OnBnClickedBtApply()
{
	UpdateData(TRUE);

	int iShowAfterTime = m_stControls.cCBBShowAfterTime.GetItemData(m_stVars.iShowAfterTime);
	int iPosition = m_stControls.cCBBPosition.GetItemData(m_stVars.iPosition);	

	if(m_MainApp->WriteProfileInt(propRuleClientSection, propShowOriginalTime, m_stVars.iShowOriginalTime) 
		&& m_MainApp->WriteProfileInt(propRuleClientSection, propShowOriginalTimeAfter, iShowAfterTime)
		&& m_MainApp->WriteProfileInt(propRuleClientSection, propOriginalTimePos, iPosition))
	{
		AfxMessageBox(TEXT("Settings saved"));
	}
	else
		AfxMessageBox(TEXT("Error saving settings"));
}


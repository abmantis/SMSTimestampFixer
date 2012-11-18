// SMSTimeStampFixerCfgDlg.h : header file
//

#pragma once
#include "afxwin.h"

static const CString propRuleClientSection		= TEXT("Rule client");
static const CString propShowOriginalTimeAfter	= TEXT("ShowAfter");
static const CString propShowOriginalTime		= TEXT("ShowOriginal");
static const CString propOriginalTimePos		= TEXT("OriginalPos");

// CSMSTimeStampFixerCfgDlg dialog
class CSMSTimeStampFixerCfgDlg : public CDialog
{
// Construction
public:
	CSMSTimeStampFixerCfgDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SMSTIMESTAMPFIXERCFG_DIALOG };
	enum Positions { SMSBEGINING, SMSEND };
	
	
	

// Implementation
public:
	virtual INT_PTR DoModal(CWinApp *mainApp);


protected:
	HICON m_hIcon;

	void FillComboBoxes();

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	afx_msg void OnBnClickedBtApply();
	afx_msg void OnBnClickedBtStart();
	afx_msg void OnBnClickedBtStop();	


	// Generated message map functions
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()

	


protected:
	struct stVars
	{
		int		iShowOriginalTime;
		int		iShowAfterTime;
		int		iPosition;
	};
	
	struct stControls
	{
		CComboBox cCBBPosition;
		CComboBox cCBBShowAfterTime;
	};
	
	stVars		m_stVars;
	stControls	m_stControls;
	
	CWinApp*	m_MainApp;
};

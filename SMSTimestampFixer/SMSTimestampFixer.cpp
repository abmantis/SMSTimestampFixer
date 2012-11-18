#include "stdafx.h"
#include "SMSTimestampFixer.h"

#include "initguid.h"
int g_cServerLocks = 0;

// {3AD4C10E-673C-494c-98A2-CC2E91A48115}
DEFINE_GUID(CLSID_SMSTimestampFixer, 0x3ad4c10e, 0x673c, 0x494c, 0x98, 0xa2, 0xcc, 0x2e, 0x91, 0xa4, 0x81, 0x15);

/* Add to apps.reg:
[HKEY_CLASSES_ROOT\CLSID\{3AD4C10E-673C-494c-98A2-CC2E91A48115}\InProcServer32]
@="SMSTimestampFixer.dll"
*/

/*
Add this line too:
[HKEY_LOCAL_MACHINE\Software\Microsoft\Inbox\Svc\SMS\Rules]
"{3AD4C10E-673C-494c-98A2-CC2E91A48115}"=dword:1
*/




// **************************************************************************
// Function Name: CFactory 
// Purpose: Initializes CFactory object
// Description: 
//	Constructor for CFactory class. Initializes class members. 
CFactory::CFactory()
{
    m_cRef = 1;
}

// **************************************************************************
// Function Name: ~CFactory
//
// Purpose: Cleans up CFactory object
// Description:  
//	Destructor for CFactory object
CFactory::~CFactory()
{
}

// **************************************************************************
// Function Name: QueryInterface
// Purpose: Obtains caller's desired interface pointer if it is supported

// Arguments:
//	IN IID& iid - Identifier for desired interface
//	OUT LPVOID *ppv - pointer to desired interface pointer

// Return Values: HRESULT 
//	E_NOINTERFACE:  the requested interface is not supported
//	E_INVALIDARG: bad reference for out param

// Description:
//	Standard implementation of COM IUnknown::QueryInterface  
STDMETHODIMP CFactory::QueryInterface(const IID& iid, LPVOID *ppv)
{
    HRESULT hr = E_NOINTERFACE;

    if (!ppv)
    {
        return E_INVALIDARG;
    }
    
    if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
    {
        *ppv = (LPVOID) this;
    }
    else
    {
        *ppv = NULL;
    }

    if (*ppv) 
    {
        ((LPUNKNOWN) *ppv)->AddRef();
        hr = S_OK;
    }
    
    return hr;
}

// **************************************************************************
// Function Name: AddRef
// Purpose: COM reference counting

// Description:  
//	Implements IUnknown::Addref by adding 1 to the object's reference count
ULONG CFactory::AddRef()
{
#ifdef _DEBUG
    AfxMessageBox((TEXT("Factory Reference is now %d\r\n"), m_cRef + 1));
#endif
	return InterlockedIncrement(&m_cRef);
}

// **************************************************************************
// Function Name: Release
// Purpose: COM reference counting

// Return Value: ULONG - reference count after decrementing 

// Description:  
//	Implements IUnknown::Release by subtracting 1 from the object's reference count
ULONG CFactory::Release()
{
    InterlockedDecrement(&m_cRef);
#ifdef _DEBUG
    AfxMessageBox((TEXT("Factory Reference is now %d\r\n"), m_cRef));
#endif    
    int nLocal = m_cRef;

    if (!m_cRef) 
    {
#ifdef _DEBUG
        AfxMessageBox((TEXT("CFactory Deleted!\r\n")));
#endif
		delete this; 
    }

    return nLocal; 
}

// **************************************************************************
// Function Name: CreateInstance
// Purpose: Create a new instance of a COM object and return the specified
//	interface

// Arguments:	IN LPUNKNOWN pUnknownOuter - controlling outer for aggregation
//				IN REFIID iid - interface identifier GUID reference
//				OUT LPVOID *ppv - pointer to newly created interface pointer

// Return Values: HRESULT, S_OK if successful, error otherwise
STDMETHODIMP CFactory::CreateInstance(LPUNKNOWN pUnknownOuter, REFIID iid, LPVOID *ppv)
{
    CMailRuleClient *pClient = NULL;
    HRESULT hr;

    // No aggregation
    if (pUnknownOuter)
    {
        hr = CLASS_E_NOAGGREGATION;
        goto Error;

    }

    // You heard 'em, create a component
    pClient = new CMailRuleClient();
    if (!pClient)
    {
        hr = E_OUTOFMEMORY;
        goto Error;
    }
    
    // Get the requested interface
    hr = pClient->QueryInterface(iid, ppv);

Error:
    // Release the unknown pointer
    if (pClient) 
    {
        pClient->Release();
    }
    
    return hr;
}

// **************************************************************************
// Function Name: LockServer
// Purpose: Increment or decrement the number of lock on a COM server

// Arguments: IN BOOL bLock - increment(TRUE) or decrement(FALSE) the lockcount
// Return Values: HRESULT - S_OK
STDMETHODIMP CFactory::LockServer(BOOL bLock)
{
    if (bLock)
    {
        g_cServerLocks++;
    }
    else
    {
        g_cServerLocks--;
    }

    return S_OK;
}

// **************************************************************************
// Function Name: CMailRuleClient
//
// Purpose: Initialize the CMailRuleClient object
// Description: CMailRuleClient Constructor 
CMailRuleClient::CMailRuleClient()
{
	//AfxMessageBox(L"CMailRuleClient()");
    m_cRef = 1;

	regRuleClientSubKey			= TEXT("Software\\SMSTimestampFixer\\SMSTimeStampFixerCfg\\Rule client");
	regShowOriginalTime			= TEXT("ShowOriginal");
	regShowOriginalTimeAfter	= TEXT("ShowAfter");
	regOriginalTimePos			= TEXT("OriginalPos");


	m_iShowOriginalTime	= 1;
	m_iShowAfterTime	= 1;
	m_iPosition			= SMSBEGINING;

	HKEY hKey;
	DWORD data;
	DWORD dwType;
	DWORD dwBufSize = sizeof(DWORD);

	if( RegOpenKeyEx(RULECLIENT_REGKEY, regRuleClientSubKey, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		dwType = REG_DWORD;
		if( RegQueryValueEx(hKey, regShowOriginalTime		, 0, &dwType, (LPBYTE)&data, &dwBufSize) == ERROR_SUCCESS)
			m_iShowOriginalTime	= data;
		if( RegQueryValueEx(hKey, regShowOriginalTimeAfter	, 0, &dwType, (LPBYTE)&data, &dwBufSize) == ERROR_SUCCESS)
			m_iShowAfterTime	= data;
		if( RegQueryValueEx(hKey, regOriginalTimePos		, 0, &dwType, (LPBYTE)&data, &dwBufSize) == ERROR_SUCCESS)
			m_iPosition			= data;		

		RegCloseKey(hKey);
	}
#ifdef _DEBUG
		AfxMessageBox((TEXT("aaaaaa")));
#endif
	//CString str;
	//str.Format(TEXT("%d, %d, %d"), m_iShowOriginalTime, m_iShowAfterTime, m_iPosition);
	//AfxMessageBox(str);
}

// **************************************************************************
// Function Name: ~CMailRuleClient
//
// Purpose: Cleans up CMailRuleClient object
// Description:  
//	Destructor for CMailRuleClient object
CMailRuleClient::~CMailRuleClient()
{
#ifdef _DEBUG
    AfxMessageBox((TEXT("CMailRuleClient destruction")));
#endif  
}

// **************************************************************************
// Function Name: QueryInterface
// Purpose: Obtains caller's desired interface pointer if it is supported

// Arguments:
//	IN IID& iid - Identifier for desired interface
//	OUT LPVOID *ppv - pointer to desired interface pointer

// Return Values: HRESULT 
//	E_NOINTERFACE:  the requested interface is not supported
//	E_INVALIDARG: bad reference for out param

// Side effects:  

// Description:
//	Standard implementation of COM IUnknown::QueryInterface

HRESULT CMailRuleClient::QueryInterface(REFIID rif, void** ppobj)
{	
	//AfxMessageBox(L"QueryInterface");
    HRESULT hr = E_NOINTERFACE;

    if (!ppobj)
    {
        return E_INVALIDARG;
    }
    
	*ppobj = NULL;
	if ((rif == IID_IUnknown) || (rif == IID_IMailRuleClient))
	{
	 	*ppobj = (LPVOID) this;
 	}

    if (*ppobj) 
    {
	 	((LPUNKNOWN) *ppobj)->AddRef();
		hr = S_OK;
	}
	
	return hr;
}

// **************************************************************************
// Function Name: AddRef
// Purpose: COM reference counting

// Return Values: current ref count (after adding)
// Description:  
//	Implements IUnknown::Addref by adding 1 to the object's reference count
ULONG CMailRuleClient::AddRef()
{
	//AfxMessageBox(L"AddRef");
#ifdef _DEBUG
    AfxMessageBox((TEXT("SMSTimestampFixer reference is now %d\r\n"), m_cRef + 1));
#endif
    return InterlockedIncrement(&m_cRef);
}

// **************************************************************************
// Function Name: Release
// Purpose: COM reference counting

// Return Values: current ref count (after subracting)
// Description:
//	Implements IUnknown::Addref by subtracting 1 from the object's reference count  

ULONG CMailRuleClient::Release()
{
	//AfxMessageBox(L"Release");
    InterlockedDecrement(&m_cRef);
#ifdef _DEBUG
    AfxMessageBox((TEXT("SMSTimestampFixer reference is now %d\r\n"), m_cRef));
#endif

    int nLocal = m_cRef;

    if (!m_cRef) 
    {
#ifdef _DEBUG
        AfxMessageBox((TEXT("SMSTimestampFixer Deleted!\r\n")));
#endif
        delete this; 
    }

    return nLocal; 
}

// **************************************************************************
// Function Name: Initialize
//
// Purpose: determines how the mail rule client will process incoming messages.

// Arguments: IN IMsgStore * pMsgStore - represests message store which contains
//	the incoming messages
//			  OUT MRCACCESS *pmaDesired - desired message store access level	

// Return Values: HRESULT - S_OK

// Description:  
//	This function is called by the system to initialize rule clients. Since we
//	eventually may want to delete messages, we request write access here
HRESULT CMailRuleClient::Initialize(IMsgStore *pMsgStore, MRCACCESS *pmaDesired)
{  
	//AfxMessageBox(L"Initialize");
    *pmaDesired = MRC_ACCESS_WRITE;
    return S_OK;
}

// **************************************************************************
// Function Name: ProcessMessage
//
// Purpose: process incoming messages, which can be moved, modified, or deleted 

// Arguments: IN IMsgStore * pMsgStore - represests message store which contains
//				the incoming messages
//			  IN ULONG cbMsg - The size of lpMsg in bytes
//			  IN LPENTRYID - The ENTRYID of the message
//			  IN ULONG cbDestFolder - The size of lpDestFolder in bytes
//			  IN LPENTRYID lpDestFolder - The ENTRYID of the the folder that 
//				incoming messages are moved to.
//			  OUT ULONG * pulEventType - bit flag that indicates the type of 
//				action the client performed on the message 
//			  OUT MRCHANDLED * pHandled - The type of message handling that 
//				occured during the processing
//			  

// Return Values: HRESULT 
//	This method returns S_OK if the processing was successful, and appropriate 
//	errors if not. 

// Description: 
//	This function is called by the system when an incoming message is received
//  This is where all the plugin-defined processing happens. You can filter 
//	messages based on content, delete or move messages, and report whether the
//  message has been handled or not.

HRESULT CMailRuleClient::ProcessMessage(IMsgStore *pMsgStore, ULONG cbMsg, LPENTRYID lpMsg, 
            ULONG cbDestFolder, LPENTRYID lpDestFolder, ULONG *pulEventType, MRCHANDLED *pHandled)
{
	// TODO: Remove the "goto" statements and do it in a propper way :)
	CString str;

    HRESULT hr = S_OK;
	SizedSPropTagArray(1, sptaSubject) = { 1, PR_SUBJECT}; 
	SPropValue *pspvSubject = NULL;
	SizedSPropTagArray(1, sptaTimestamp) = { 1, PR_MESSAGE_DELIVERY_TIME}; 
    SPropValue *pspvTimestamp = NULL;
	ULONG cValues = 0;
    IMessage *pMsg = NULL;
    HRESULT hrRet = S_OK;

	
	FILETIME serverFTime, clientFTime, localFTime;
	SYSTEMTIME clientSysTime, serverSysTime;
	COleDateTime dtCurTime, dtServerTime;
	COleDateTimeSpan span;
	CString msg;

    // Get the message from the entry ID
    hr = pMsgStore->OpenEntry(cbMsg, lpMsg, NULL, 0, NULL, (LPUNKNOWN *) &pMsg);
    if (FAILED(hr))
    {       
#ifdef _DEBUG
		AfxMessageBox((TEXT("Unable to get the message!\r\n")));
#endif
		goto Exit;
    }
	
	// Get the timestamp
	hr = pMsg->GetProps((SPropTagArray *) &sptaTimestamp, MAPI_UNICODE, &cValues, &pspvTimestamp);
	if (FAILED(hr))
	{        
#ifdef _DEBUG
		AfxMessageBox((TEXT("Unable to get the message timestamp!\r\n")));
#endif
		goto Exit;
	}
	
	serverFTime = pspvTimestamp->Value.ft;	

	//Convert to an COleDateTime
	FileTimeToLocalFileTime ( &serverFTime, &localFTime );
	FileTimeToSystemTime ( &localFTime, &serverSysTime );
	
	dtCurTime = COleDateTime::GetCurrentTime();
	dtServerTime = COleDateTime(serverSysTime);
	
	if(m_iShowOriginalTime != 0)
	{
		span = dtCurTime - dtServerTime;
		double diffMin = span.GetTotalMinutes();

		if( diffMin >= m_iShowAfterTime || diffMin <= m_iShowAfterTime * -1)
		{
			// We need to put the [timestamp] in the SMS body
			// For SMS, the subject is also the message body
			hr = pMsg->GetProps((SPropTagArray *) &sptaSubject, MAPI_UNICODE, &cValues, &pspvSubject);
			if (FAILED(hr))
			{
#ifdef _DEBUG
				AfxMessageBox((TEXT("Unable to get the message body!\r\n")));
#endif
				goto Exit;
			}

			msg = "[";
			//Use date if it was from another day else use only time
			if(span.GetTotalDays() >= 1)
				msg += dtServerTime.Format();
			else
				msg += dtServerTime.Format(VAR_TIMEVALUEONLY).Trim();
			msg += "]";

			if(m_iPosition == SMSBEGINING)
				msg = msg + CString(" ") + pspvSubject->Value.lpszW;
			else
				msg = pspvSubject->Value.lpszW + CString(" ") + msg;

			pspvSubject->Value.lpszW = msg.GetBuffer(1024);
			hr = pMsg->SetProps(1, pspvSubject, NULL);
			if (FAILED(hr))
			{        
#ifdef _DEBUG
				AfxMessageBox((TEXT("Unable to set the message body!\r\n")));
#endif
				goto Exit;
			}

			msg.ReleaseBuffer();
		}
	}

	// Convert the COleDateTime of the current date to a FileTime
	clientSysTime.wDay			= dtCurTime.GetDay();
	clientSysTime.wDayOfWeek	= dtCurTime.GetDayOfWeek();
	clientSysTime.wHour			= dtCurTime.GetHour();
	clientSysTime.wMinute		= dtCurTime.GetMinute();
	clientSysTime.wMonth		= dtCurTime.GetMonth();
	clientSysTime.wSecond		= dtCurTime.GetSecond();
	clientSysTime.wYear			= dtCurTime.GetYear();
	clientSysTime.wMilliseconds	= serverSysTime.wMilliseconds;

	SystemTimeToFileTime(&clientSysTime, &localFTime);
	LocalFileTimeToFileTime(&localFTime, &clientFTime);

	pspvTimestamp->Value.ft = clientFTime;
	
	// Set the current date/time in the SMS timestamp
	hr = pMsg->SetProps(1, pspvTimestamp, NULL);
	if (FAILED(hr))
    {        
#ifdef _DEBUG
		AfxMessageBox((TEXT("Unable to set the message time!\r\n")));
#endif
		goto Exit;
    }


// Clean up
Exit:
    if (pspvTimestamp)
    {
        MAPIFreeBuffer(pspvTimestamp);
    }
    if (pMsg)
    {
        pMsg->Release();
    }
    	
    return hr;
}

//// **************************************************************************
//// Function Name: DllMain
//// Purpose: DLL Entry point
//// Arguments: IN HANDLE hinst - Handle to the DLL
////			  IN DWORD dwReason - flag indicating why entry-point was called
////			  IN LPVOID lpv - specifies further aspects of initialization and
////				cleanup 
//
//// Return Values: TRUE if initialization succeeds, FALSE otherwise
////	Called by system when a thread or process loads/unloads the dll 
//BOOL WINAPI DllMain(HANDLE hinst, DWORD dwReason, LPVOID lpv)
//{
//   
//
//	switch (dwReason)
//    {
//        case DLL_PROCESS_ATTACH:
//            break;
//
//        case DLL_PROCESS_DETACH:
//            break;
//
//        default:
//            break;
//    }
//
//    return TRUE;
//}

// **************************************************************************
// Function Name: DllGetClassObject
// Purpose: Retrieves the class object from the DLL object

// Arguments: IN CLSID& clsid - CLSID for desired class object
//			  IN REFIID iid - ref to interface id, usually IClassFactory
//			  OUT LPVOID *ppv - address of requested interface pointer

// Return Values: HRESULT
STDAPI DllGetClassObject(const CLSID& clsid, REFIID iid, LPVOID *ppv)
{
#ifdef _DEBUG
		AfxMessageBox((TEXT("DllGetClassObject")));
#endif
    HRESULT hr;
    
    // We only understand this rule's class
    if (clsid != CLSID_SMSTimestampFixer)
    {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    CFactory *pFactory = new CFactory;
    if (!pFactory)
    {
        return E_OUTOFMEMORY;
    }

    // Get the requested interface
    hr = pFactory->QueryInterface(iid, ppv);
    pFactory->Release();
    
    return hr;
}

// **************************************************************************
// Function Name: DllCanUnloadNow
//
// Purpose: Notify caller if dll can safely be unloaded
// Return Values: HRESULT, S_OK if safe to unload, S_FALSE otherwise

// Description:  A call to DllCanUnloadNow determines whether the DLL from 
//	which it is exported is still in use. A DLL is no longer in use when it 
//	is not managing any existing objects (the reference count on all of its 
//	objects is zero). 
STDAPI DllCanUnloadNow()
{
	//AfxMessageBox(L"DllCanUnloadNow");
    if (!g_cServerLocks)
    {
     
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}

// **************************************************************************
// Function Name: DllRegisterServer
//
// Purpose: provide DLL with the ability to register its COM objects
// Return Values: HRESULT - S_OK if registration succeeds, E_FAIL otherwise

// Side effects: In order to fully remove the plugin, both registry keys should
//	be removed (see Description below). DllUnregisterServer does this.

// Description:  
//  In addition to standard COM object registration, the function also must
//  register our rule client handler with Inbox.  We are registering our 
//	DLL in two places, HKEY_CLASSES_ROOT\CLSID\, and HKEY_LOCAL_MACHINE\Software
//	\Microsoft\Inbox\Svc\SMS\Rules
STDAPI DllRegisterServer()
{
	//AfxMessageBox(L"DllRegisterServer");
    LRESULT lr;
    HRESULT hr = E_FAIL;
    HKEY hKey = NULL;
    HKEY hSubKey = NULL;
    DWORD dwDisposition;
    TCHAR wszValue[25];

    // Set up registry keys
    // Register with COM:
    //    [HKEY_CLASSES_ROOT\CLSID\{3AD4C10E-673C-494c-98A2-CC2E91A48115}\InProcServer32]
    //    @="SMSTimestampFixer.dll"
	

    lr = RegCreateKeyEx(HKEY_CLASSES_ROOT, TEXT("\\CLSID\\{3AD4C10E-673C-494c-98A2-CC2E91A48115}"),
	                              0, NULL, 0, 0, NULL, 
	                              &hKey, &dwDisposition);
    if (lr != ERROR_SUCCESS)
    {
        goto Exit;
    }

    lr = RegCreateKeyEx(hKey, TEXT("InprocServer32"),
	                              0, NULL, 0, 0, NULL, 
	                              &hSubKey, &dwDisposition);
    if (lr != ERROR_SUCCESS)
    {
        goto Exit;
    }

    lstrcpy(wszValue, TEXT("SMSTimestampFixer.dll"));
    lr = RegSetValueEx(hSubKey, NULL, 0, REG_SZ, (LPBYTE) wszValue, (lstrlen(wszValue) + 1) * sizeof(TCHAR));
    if (lr != ERROR_SUCCESS)
    {
        goto Exit;
    }

    RegCloseKey(hSubKey);
    hSubKey = NULL;
    RegCloseKey(hKey);
    hKey = NULL;

    // Register with Inbox:
    //    [HKEY_LOCAL_MACHINE\Software\Microsoft\Inbox\Svc\SMS\Rules]
    //    {3AD4C10E-673C-494c-98A2-CC2E91A48115}"=dword:1

    lr = RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("\\Software\\Microsoft\\Inbox\\Svc\\SMS\\Rules"),
	                              0, NULL, 0, 0, NULL, 
	                              &hKey, &dwDisposition);
    if (lr != ERROR_SUCCESS)
    {
        goto Exit;
    }

    dwDisposition = 1;
    lr = RegSetValueEx(hKey, TEXT("{3AD4C10E-673C-494c-98A2-CC2E91A48115}"), 0, REG_DWORD, 
                          (LPBYTE) &dwDisposition, sizeof(DWORD));
    if (lr != ERROR_SUCCESS)
    {
        goto Exit;
    }
 
    hr = S_OK;

Exit:
    if (hSubKey)
    {
        RegCloseKey(hSubKey);
    }
    if (hKey)
    {
        RegCloseKey(hKey);
    }

	if( hr == S_OK )
		AfxMessageBox(TEXT("SMSTimestampFixer 1.3.2 successfully registered"));
	else
		AfxMessageBox(TEXT("SMSTimestampFixer 1.3.2 registration was unsuccessful"));

    return hr;
}

// **************************************************************************
// Function Name: DllUnregisterServer
// Purpose: rovide DLL with the ability to un-register its COM objects
// Return Values: HRESULT - S_OK if registration succeeds, E_FAIL otherwise
// Description:  Deletes both the COM registry key and the key used to register
// the rule client with Inbox
STDAPI DllUnregisterServer()
{
	//AfxMessageBox(L"DllUnregisterServer");
    HKEY hKey = NULL;
    HRESULT hr = E_FAIL;
    LRESULT lr;
    DWORD dwDisposition;

    // Delete registry keys
    RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("\\CLSID\\{3AD4C10E-673C-494c-98A2-CC2E91A48115}"));
    
    lr = RegCreateKeyEx(HKEY_LOCAL_MACHINE, TEXT("\\Software\\Microsoft\\Inbox\\Svc\\SMS\\Rules"),
	                              0, NULL, 0, 0, NULL, 
	                              &hKey, &dwDisposition);
    if (lr != ERROR_SUCCESS)
    {
        goto Exit;
    }

    RegDeleteValue(hKey, TEXT("{3AD4C10E-673C-494c-98A2-CC2E91A48115}"));

    hr = S_OK;

Exit:
    if (hKey)
    {
        RegCloseKey(hKey);
    }

	if( hr == S_OK )
		AfxMessageBox(TEXT("SMSTimestampFixer 1.3.1 successfully unregistered"));
	else
		AfxMessageBox(TEXT("SMSTimestampFixer 1.3.1 unregistration was unsuccessful"));

    return hr;
}


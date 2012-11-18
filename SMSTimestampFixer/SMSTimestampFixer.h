// SMSTimestampFixer.h : main header file for the SMSTimestampFixer DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resourceppc.h"

// ************************************************************
// Class CMailRuleClient - Implementation of IMailRuleClient
//
// Inheritance:
//     IMailRuleClient IUnknown (Abstract)
//
// Purpose:
//     This class serves as implementation for the IMailRuleClient
//	   interface and provides our Rule Client functionality.
//	   The Initialize method sets our permissions to interact
//	   with the message store, and the ProcesseMessage method
//	   defines how we handle incoming messages
//
// ************************************************************
#define RULECLIENT_REGKEY						HKEY_CURRENT_USER


class CMailRuleClient : public IMailRuleClient
{
public:
    CMailRuleClient();
    ~CMailRuleClient();
    
    // IUnknown
    STDMETHOD (QueryInterface)(REFIID iid, LPVOID *ppv);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();

    // IMailRuleClient
    MAPIMETHOD(Initialize)(
            IMsgStore *pMsgStore,
            MRCACCESS *pmaDesired
            );
            
    MAPIMETHOD(ProcessMessage)(
            IMsgStore *pMsgStore, 
            ULONG cbMsg,
            LPENTRYID lpMsg,
            ULONG cbDestFolder,
            LPENTRYID lpDestFolder,
            ULONG *pulEventType,
            MRCHANDLED *pHandled
            );

	enum Positions { SMSBEGINING, SMSEND };

private:
    long	m_cRef;

	int		m_iShowOriginalTime;
	int		m_iShowAfterTime;
	int		m_iPosition;

	CString regRuleClientSubKey;
	CString regShowOriginalTime;
	CString regShowOriginalTimeAfter;
	CString regOriginalTimePos;

};


// ************************************************************
// Class CFactory - Class factory for CMailRuleClient objects
//
// Inheritance:
//     IClassFactory IUnknown
//
// Purpose:
//     This class provides a standard COM class factory implementation
//	   for CMailRuleClient
//
// ************************************************************
class CFactory : public IClassFactory
{
public:
    CFactory();
    ~CFactory();
    
    // IUnknown
    STDMETHOD (QueryInterface)(REFIID iid, LPVOID *ppv);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();

    // IClassFactory interfaces
    STDMETHOD (CreateInstance)(IUnknown *pUnknownOuter, const IID& iid, LPVOID *ppv);
    STDMETHOD (LockServer)(BOOL bLock);

private:
    long m_cRef;
};
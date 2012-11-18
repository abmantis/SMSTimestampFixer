// SetupDLL.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "ce_setup.h"
#include "Tlhelp32.h"


DWORD CFindProcess(TCHAR* procName)
{
	// CFindProcess function takes procName string as input and return Process ID of procName if found in the process

	HINSTANCE hProcessSnap = NULL;
	PROCESSENTRY32 pe32 = {0}; 

	//snapshot of currently executing processes
	hProcessSnap = (HINSTANCE)CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPNOHEAPS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE ) return 0;
		pe32.dwSize = sizeof(PROCESSENTRY32);			// Set the size of the structure before using it.
	if (Process32First(hProcessSnap, &pe32))			// Retrieve information about the first process, 
	{
		do
		{
			if (_wcsicmp(pe32.szExeFile, procName)==0)		// Compare current process name with process to be killed name.
			{ 
				CloseToolhelp32Snapshot(hProcessSnap);	// If both are match Close snapshot
				return pe32.th32ProcessID;				// return PID of current process.
			}
		}
		while (Process32Next(hProcessSnap, &pe32));		// goto next process name
	}
	// if no matches found then close snapshot and return zero
	CloseToolhelp32Snapshot(hProcessSnap);
	return 0;
}

void CKillProc(TCHAR* procName){
	
	DWORD dwPID = CFindProcess(procName);
	HANDLE hProcess;
	if (dwPID) // If process ID is non-zero then enter inside to kill the same
	{ 
		hProcess = OpenProcess(PROCESS_ALL_ACCESS,false,dwPID);	// open process object to change access
		TerminateProcess(hProcess,0);							// Kill process
		CloseHandle(hProcess);									// Close handle
	}
}

codeINSTALL_INIT
Install_Init(HWND hwndParent, BOOL fFirstCall, BOOL fPreviouslyInstalled, LPCTSTR pszInstallDir)
{
	CKillProc(TEXT("tmail.exe"));

	// To continue installation, return codeINSTALL_INIT_CONTINUE
	// If you want to cancel installation, 
	// return codeINSTALL_EXIT_UNINSTALL
	return codeINSTALL_INIT_CONTINUE;

}

codeINSTALL_EXIT
Install_Exit(HWND hwndParent, LPCTSTR pszInstallDir, WORD cFailedDirs, WORD cFailedFiles, 
			 WORD cFailedRegKeys, WORD cFailedRegVals, WORD cFailedShortcuts)
{
	// TODO: Add custom installation code here

	// To exit the installation DLL normally, 
	// return codeINSTALL_EXIT_DONE
	// To unistall the application after the function exits,
	// return codeINSTALL_EXIT_UNINSTALL
	return codeINSTALL_EXIT_DONE;
}

codeUNINSTALL_INIT
Uninstall_Init(HWND hwndParent, LPCTSTR pszInstallDir)
{
	CKillProc(TEXT("tmail.exe"));

	// To continue uninstallation, return codeUNINSTALL_INIT_CONTINUE
	// If you want to cancel installation,
	// return codeUNINSTALL_INIT_CANCEL
	return codeUNINSTALL_INIT_CONTINUE;
}


codeUNINSTALL_EXIT
Uninstall_Exit(HWND hwndParent)
{
	// TODO: Add custom uninstallation code here

	return codeUNINSTALL_EXIT_DONE;
}


// SvcProgHelper.h: interface for the SvcProgHelper class.
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2004 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_SVCPROGHELPER_H__A3361E3B_D434_4AD9_9ECF_A0CBE93D0CA5__INCLUDED_)
#define AFX_SVCPROGHELPER_H__A3361E3B_D434_4AD9_9ECF_A0CBE93D0CA5__INCLUDED_

#include <windows.h>
#include <stdio.h>
#include <tchar.h>


namespace GLASS {


template <class T>
class SvcProgHelper  
{
public:
	typedef T	WorkClass;

public:
	static SvcProgHelper& Instance()
	{
		static SvcProgHelper svc;
		return svc;
	}

	~SvcProgHelper() {}

private:
	SvcProgHelper() : bRun_(false) {}

public:
	enum { SZ_SIZE = 1024 };
	
public:
	void Install();
	void Uninstall();
	void Dispatcher();

	void Initialize(LPCTSTR szServiceName, WorkClass work);
	
public:
	static void WINAPI ServiceRun(DWORD argc, LPTSTR *argv);
	static void WINAPI ServiceCtrlHandler(DWORD dwControl);

private:
	WorkClass work_;

	SERVICE_STATUS_HANDLE hServiceHandle_;
	volatile bool bRun_;
	TCHAR SERVICE_NAME[SZ_SIZE + 1];
};


//////////////////////////////////////////////////////////////////////////
//

template <class T>
void SvcProgHelper<T>::Initialize(LPCTSTR szServiceName, WorkClass work)
{
	_tcscpy(SERVICE_NAME, szServiceName);
	work_ = work;
}

template <class T>
void SvcProgHelper<T>::Install()
{
	TCHAR szPath[MAX_PATH + 1];    

    SC_HANDLE scmHandle = OpenSCManager ( NULL, NULL, SC_MANAGER_ALL_ACCESS );

    if (scmHandle == NULL) // Perform error handling.
    {
    }

	GetModuleFileName( GetModuleHandle(NULL), szPath, MAX_PATH );
    
	TCHAR szBinPath[SZ_SIZE + 1] = {0,};
	_tcscpy(szBinPath, _T("\""));
	_tcscat(szBinPath, szPath);
	_tcscat(szBinPath, _T("\""));
    
    SC_HANDLE scHandle = CreateService (
		scmHandle, 
		SvcProgHelper<T>::Instance().SERVICE_NAME, 
        SvcProgHelper<T>::Instance().SERVICE_NAME, 
		SERVICE_ALL_ACCESS, 
        SERVICE_WIN32_OWN_PROCESS, 
        SERVICE_AUTO_START, 
        SERVICE_ERROR_NORMAL, 
        szBinPath, 
		NULL, NULL, NULL, NULL, NULL 
	);

    if (scHandle == NULL) // Process error
    {
    }

    CloseServiceHandle(scHandle);
    CloseServiceHandle(scmHandle);
}

template <class T>
void SvcProgHelper<T>::Uninstall()
{
	SC_HANDLE scmHandle = OpenSCManager (NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (scmHandle == NULL) // Perform error handling.
    {
    }

	SC_HANDLE scHandle = OpenService( 
		scmHandle, 
		SvcProgHelper<T>::Instance().SERVICE_NAME, 
		SERVICE_ALL_ACCESS 
	);
	
    DeleteService( scHandle );
}

template <class T>
void SvcProgHelper<T>::Dispatcher()
{
    SERVICE_TABLE_ENTRY DispatchTable[] = { 
		{ SvcProgHelper<T>::Instance().SERVICE_NAME, ServiceRun }, 
		{ 0, 0 } 
	};

 	if (!StartServiceCtrlDispatcher(DispatchTable) ) 
	{ 
	}
}

template <class T>
void WINAPI SvcProgHelper<T>::ServiceRun( DWORD argc, LPTSTR *argv ) 
{ 
	SERVICE_STATUS ServiceStatus; 
    ServiceStatus.dwServiceType        = SERVICE_WIN32_OWN_PROCESS; 
    ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
    ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP; 
    ServiceStatus.dwWin32ExitCode      = 0; 
    ServiceStatus.dwServiceSpecificExitCode = 0; 
    ServiceStatus.dwCheckPoint         = 0; 
    ServiceStatus.dwWaitHint           = 0; 

	SvcProgHelper<T> &Svc = SvcProgHelper<T>::Instance();

	SERVICE_STATUS_HANDLE &hServiceHandle = Svc.hServiceHandle_;
	char *SERVICE_NAME = Svc.SERVICE_NAME;

	hServiceHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler ); 
 
    if ( hServiceHandle == (SERVICE_STATUS_HANDLE) 0 ) 
    { 
        return; 
    } 
 
    ServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
    ServiceStatus.dwCheckPoint         = 0; 
    ServiceStatus.dwWaitHint           = 0; 
 
    if ( !SetServiceStatus( hServiceHandle, &ServiceStatus ) ) 
    { 
    } 

    Svc.bRun_ = true;
	Svc.work_(Svc.bRun_);

    ServiceStatus.dwCurrentState = SERVICE_STOPPED; 
    
	// Send current status. 
    if (!SetServiceStatus(hServiceHandle, &ServiceStatus)) 
    { 
    } 
 
    return; 
} 
 
template <class T>
void WINAPI SvcProgHelper<T>::ServiceCtrlHandler(DWORD dwControl)
{ 
	SERVICE_STATUS ServiceStatus; 
	ServiceStatus.dwServiceType        = SERVICE_WIN32_OWN_PROCESS; 
    ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
    ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP; 
    ServiceStatus.dwWin32ExitCode      = 0; 
    ServiceStatus.dwServiceSpecificExitCode = 0; 
    ServiceStatus.dwCheckPoint         = 0; 
    ServiceStatus.dwWaitHint           = 0; 

    switch(dwControl) 
    { 
        case SERVICE_CONTROL_PAUSE: 
            ServiceStatus.dwCurrentState = SERVICE_PAUSED; 
            break; 
 
        case SERVICE_CONTROL_CONTINUE: 
            ServiceStatus.dwCurrentState = SERVICE_RUNNING; 
            break; 
 
        case SERVICE_CONTROL_STOP: 
			ServiceStatus.dwWin32ExitCode = 0; 
            ServiceStatus.dwCurrentState  = SERVICE_STOP_PENDING; 
            ServiceStatus.dwCheckPoint    = 0; 
            ServiceStatus.dwWaitHint      = 0; 

 			SvcProgHelper::Instance().bRun_ = false;
			break;

        case SERVICE_CONTROL_INTERROGATE: 
            break; 
 
        default: 
			break;
    } 
 
	SvcProgHelper<T> &Svc = SvcProgHelper<T>::Instance();
	SERVICE_STATUS_HANDLE &hServiceHandle = Svc.hServiceHandle_;

    // Send current status. 
    if ( !SetServiceStatus(hServiceHandle, &ServiceStatus) ) 
    { 
    } 
} 




}


#endif // !defined(AFX_SVCPROGHELPER_H__A3361E3B_D434_4AD9_9ECF_A0CBE93D0CA5__INCLUDED_)

//////////////////////////////////////////////////////////////////////
// Copyright (c) 2004 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
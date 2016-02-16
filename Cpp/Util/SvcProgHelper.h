// SvcProgHelper.h: interface for the SvcProgHelper class.
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2004 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_SVCPROGHELPER_H__A3361E3B_D434_4AD9_9ECF_A0CBE93D0CA5__INCLUDED_)
#define AFX_SVCPROGHELPER_H__A3361E3B_D434_4AD9_9ECF_A0CBE93D0CA5__INCLUDED_

#pragma once 


#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <functional>

namespace GLASS {


	class SvcProgHelper  
	{
	public:
		typedef std::function<void(volatile bool&)> function_type;

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
		void Install(LPCTSTR szServiceName);
		void Uninstall(LPCTSTR szServiceName);

	public:
		void StartService(function_type fn);
	
	private:
		void Run() {
			bRun_ = true;
			fn_(bRun_);
		}

	private:
		static void WINAPI ServiceRun(DWORD argc, LPTSTR *argv);
		static void WINAPI ServiceCtrlHandler(DWORD dwControl);

	private:
		SERVICE_STATUS_HANDLE hServiceHandle_;
		volatile bool bRun_;

		function_type fn_;
	};


	//////////////////////////////////////////////////////////////////////////
	//

	void SvcProgHelper::Install(LPCTSTR szServiceName)
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
			szServiceName,
			szServiceName,
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


	void SvcProgHelper::Uninstall(LPCTSTR szServiceName)
	{
		SC_HANDLE scmHandle = OpenSCManager (NULL, NULL, SC_MANAGER_ALL_ACCESS);

		if (scmHandle == NULL) // Perform error handling.
		{
		}

		SC_HANDLE scHandle = OpenService( 
			scmHandle, 
			szServiceName,
			SERVICE_ALL_ACCESS 
		);
	
		DeleteService( scHandle );
	}


	void SvcProgHelper::StartService(function_type fn)
	{
		fn_ = fn;

		SERVICE_TABLE_ENTRY DispatchTable[] = { 
			{ _T(""), &ServiceRun },
			{ 0, 0 } 
		};

 		if (!StartServiceCtrlDispatcher(DispatchTable) ) 
		{ 
		}
	}


	void WINAPI SvcProgHelper::ServiceRun( DWORD argc, LPTSTR *argv ) 
	{ 
		SERVICE_STATUS ServiceStatus; 
		ServiceStatus.dwServiceType        = SERVICE_WIN32_OWN_PROCESS; 
		ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
		ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP; 
		ServiceStatus.dwWin32ExitCode      = 0; 
		ServiceStatus.dwServiceSpecificExitCode = 0; 
		ServiceStatus.dwCheckPoint         = 0; 
		ServiceStatus.dwWaitHint           = 0; 

		SvcProgHelper &Svc = SvcProgHelper::Instance();

		SERVICE_STATUS_HANDLE &hServiceHandle = Svc.hServiceHandle_;
		//TCHAR *SERVICE_NAME = Svc.SERVICE_NAME;

		hServiceHandle = RegisterServiceCtrlHandler(_T(""), ServiceCtrlHandler ); 
 
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

		// 업무로직
		Svc.Run();
		//

		ServiceStatus.dwCurrentState = SERVICE_STOPPED; 
    
		// Send current status. 
		if (!SetServiceStatus(hServiceHandle, &ServiceStatus)) 
		{ 
		} 
 
		return; 
	} 
 

	void WINAPI SvcProgHelper::ServiceCtrlHandler(DWORD dwControl)
	{ 
		SERVICE_STATUS ServiceStatus; 
		ServiceStatus.dwServiceType        = SERVICE_WIN32_OWN_PROCESS; 
		ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
		ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP; 
		ServiceStatus.dwWin32ExitCode      = 0; 
		ServiceStatus.dwServiceSpecificExitCode = 0; 
		ServiceStatus.dwCheckPoint         = 0; 
		ServiceStatus.dwWaitHint           = 0; 

		// OutputDebugString(_T("SvcTest ServiceCtrlHandler"));

		switch(dwControl) 
		{ 
			case SERVICE_CONTROL_PAUSE: 
				// OutputDebugString(_T("SvcTest ServiceCtrlHandler SERVICE_CONTROL_PAUSE"));
				ServiceStatus.dwCurrentState = SERVICE_PAUSED; 
				break; 
 
			case SERVICE_CONTROL_CONTINUE: 
				// OutputDebugString(_T("SvcTest ServiceCtrlHandler SERVICE_CONTROL_CONTINUE"));
				ServiceStatus.dwCurrentState = SERVICE_RUNNING; 
				break; 
 
			case SERVICE_CONTROL_STOP: 
				// OutputDebugString(_T("SvcTest ServiceCtrlHandler SERVICE_CONTROL_STOP"));
				ServiceStatus.dwWin32ExitCode = 0; 
				ServiceStatus.dwCurrentState  = SERVICE_STOP_PENDING; 
				ServiceStatus.dwCheckPoint    = 0; 
				ServiceStatus.dwWaitHint      = 0; 

 				SvcProgHelper::Instance().bRun_ = false;
				break;

			case SERVICE_CONTROL_INTERROGATE: 
				// OutputDebugString(_T("SvcTest ServiceCtrlHandler SERVICE_CONTROL_INTERROGATE"));
				break; 
 
			default: 
				// OutputDebugString(_T("SvcTest ServiceCtrlHandler default"));
				break;
		} 
 
		SvcProgHelper &Svc = SvcProgHelper::Instance();
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
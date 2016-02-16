// WinThread.h
//
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
#ifndef __WINTHREAD_H__BY_CDECL__
#define __WINTHREAD_H__BY_CDECL__

#include <process.h>
#include <windows.h>


namespace GLASS {


class WinThread
{
public:
	WinThread() : hThread_(0) {}
	virtual ~WinThread() 
	{ 
		Stop(); 
	}
	
public:
	virtual void Run() = 0;
	
	
public:
	void Start() 
	{
		hThread_ = reinterpret_cast<HANDLE>
			(_beginthreadex(NULL, 0, RunImp, this, 0, NULL));
	}
	
	void Stop()
	{
		if (hThread_) {
			TerminateThread(hThread_, 0);
			Release();
		}
	}
	
	void Wait(DWORD dwMilliseconds = INFINITE)
	{
		WaitForSingleObject(hThread_, dwMilliseconds);
		Release();
	}
	
	size_t GetWinThreadID() const 
	{
		return reinterpret_cast<size_t>(hThread_); 
	}
	
protected:
	void Release()
	{
		if (hThread_) {
			CloseHandle(hThread_);
		}
		
		hThread_ = 0;
	}
	
protected:
	static UINT __stdcall RunImp(LPVOID lpvoid)
	{
		WinThread *pThis = reinterpret_cast<WinThread*>(lpvoid);
		pThis->Run();
		
		return 0;
	}
	
private:
	WinThread(const WinThread &);
	WinThread& operator=(const WinThread &);

protected:
	HANDLE hThread_;
};


} // namespace GLASS

#endif	 // __WINTHREAD_H__BY_CDECL__




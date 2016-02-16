
#ifndef __LOADLIBRARY_H__BY_CDECL__
#define __LOADLIBRARY_H__BY_CDECL__


#include <windows.h>

class LoadLibraryManage
{
public:
	explicit LoadLibraryManage(LPCTSTR szPath) 
	{
		hInstance_ = ::LoadLibrary(szPath);
	}

	~LoadLibraryManage()
	{
		FreeLibrary();
	}

	void FreeLibrary()
	{
		if (hInstance_)
			::FreeLibrary(hInstance_);
	}

	void Reset(HINSTANCE h)
	{
		FreeLibrary();
		hInstance_ = h;
	}

	operator bool()
	{
		return hInstance_ != NULL;
	}

	bool operator!()
	{
		return hInstance_ == NULL;
	}	

	operator HINSTANCE()
	{
		return hInstance_;
	}

	template <class T>
	bool operator()(LPCTSTR lp, T &F_)
	{
		F_ = (T)::GetProcAddress(hInstance_, lp);
		return F_ != NULL;
	}

private:
	HINSTANCE hInstance_;
};


#endif	// __LOADLIBRARY_H__BY_CDECL__
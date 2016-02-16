// strmanip.h: interface for the strmanip class.
//
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRMANIP_H__B4CF1076_F9E7_4F9F_A343_A4F0704AF349__INCLUDED_)
#define AFX_STRMANIP_H__B4CF1076_F9E7_4F9F_A343_A4F0704AF349__INCLUDED_

#include <string>
using namespace std;

class strmanip 
{
public:
	static string trim(const string &str);
	static string ltrim(const string &str);
	static string rtrim(const string &str) ;
};


#endif // !defined(AFX_STRMANIP_H__B4CF1076_F9E7_4F9F_A343_A4F0704AF349__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
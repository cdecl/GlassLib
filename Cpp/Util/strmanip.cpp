// strmanip.cpp: implementation of the strmanip class.
//
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "strmanip.h"


string strmanip::trim(const string &str)
{
	if (str.empty()) {
		return str;
	}

	return ltrim(rtrim(str));
}

string strmanip::ltrim(const string &str) 
{
	if (str.empty()) {
		return str;
	}

	string::size_type nPos = str.find_first_not_of(' ');
	if (nPos == string::npos) {
		if (str[0] == ' ') {
			return "";
		}
		
		return str;
	}
	
	return str.substr(nPos);
}


string strmanip::rtrim(const string &str) 
{
	if (str.empty()) {
		return str;
	}

	string::size_type nPos = str.find_last_not_of(' ');
	if (nPos == string::npos) {
		if (str[0] == ' ') {
			return "";
		}
		
		return str;
	}
	
	return str.substr(0, nPos + 1);
}


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
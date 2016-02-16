// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Log_H__E3C452A3_F6E4_11D3_8A1A_00105AC6D631__INCLUDED_)
#define AFX_Log_H__E3C452A3_F6E4_11D3_8A1A_00105AC6D631__INCLUDED_

#include <comdef.h>

class CLog  
{
public:
	CLog(const _bstr_t &bstrLogFile = _T("c:\\LogFile.log"));
	virtual ~CLog();

public:
	void InlineLog(const _bstr_t &bstr, bool bDateFileName = true);
	_bstr_t MakeDateFileName(const _bstr_t &bstrFileName, const _bstr_t &bstrDate);
	_bstr_t SetSource(const _bstr_t &bstrSource);
	_bstr_t SetDescription(const _bstr_t &bstrDescription);
	_bstr_t SetSql(const _bstr_t &bstrSql);
	_bstr_t SetParameter(const _bstr_t &bstrKey, const _bstr_t &bstrValue);
	void WriteLog(bool bDateFileName = true);

private:
	_bstr_t m_bstrDescription;
	_bstr_t m_bstrParameter;
	_bstr_t m_bstrSource;
	_bstr_t m_bstrSql;
	_bstr_t m_bstrFileName;

};

#endif // !defined(AFX_Log_H__E3C452A3_F6E4_11D3_8A1A_00105AC6D631__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
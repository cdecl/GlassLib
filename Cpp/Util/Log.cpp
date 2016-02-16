// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Log.h"
#include "SysTime.h"
#include <stdio.h>
#include <string.h>

#define SZ_SIZE		1024

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLog::CLog(const _bstr_t &bstrLogFile)
{
	m_bstrDescription = _T("");
	m_bstrParameter = _T("");
	m_bstrSource = _T("");
	m_bstrSql = _T("");
	m_bstrFileName = (LPCTSTR)bstrLogFile;
}

CLog::~CLog()
{
}

//////////////////////////////////////////////////////////////////////
// Get Source Name
//////////////////////////////////////////////////////////////////////
_bstr_t CLog::SetSource(const _bstr_t &bstrSource)
{
	m_bstrSource += (LPCTSTR)bstrSource;
	m_bstrSource += _T("\n");

	return bstrSource;
}

//////////////////////////////////////////////////////////////////////
// Get Description
//////////////////////////////////////////////////////////////////////
_bstr_t CLog::SetDescription(const _bstr_t &bstrDescription)
{
	m_bstrDescription += (LPCTSTR)bstrDescription;
	m_bstrDescription += _bstr_t(_T("\n"));

	return bstrDescription;
}

//////////////////////////////////////////////////////////////////////
// Get Sql Text
//////////////////////////////////////////////////////////////////////
_bstr_t CLog::SetSql(const _bstr_t &bstrSql)
{
	m_bstrSql += (LPCTSTR)bstrSql;
	m_bstrSql += _T("\n");
	
	return bstrSql;
}

//////////////////////////////////////////////////////////////////////
// Get Function Parameter Name - Value
//////////////////////////////////////////////////////////////////////
_bstr_t CLog::SetParameter(const _bstr_t &bstrKey, const _bstr_t &bstrValue)
{
	m_bstrParameter += (LPCTSTR)bstrKey;
	m_bstrParameter += _T(" = \"");
	m_bstrParameter += (LPCTSTR)bstrValue;
	m_bstrParameter += _T("\"\n");

	return bstrValue;
}


//////////////////////////////////////////////////////////////////////
// Write File 
//////////////////////////////////////////////////////////////////////
void CLog::WriteLog(bool bDateFileName)
{
	CSysTime sysTime = CSysTime::GetLocalTime();

	_bstr_t bstrSysTime = (LPCTSTR)sysTime.GetTime();
	_bstr_t bstrFileName = m_bstrFileName;

	if (bDateFileName) {
		bstrFileName = (LPCTSTR)MakeDateFileName(bstrFileName, sysTime.GetDate()); 
	}
	
	FILE * fp = NULL;
	if ((fp = fopen((LPCSTR)m_bstrFileName, "a+")) == NULL) {
		return ;
	}

	fprintf(fp, "============================================\n");
	fprintf(fp, "%04d/%02d/%02d %02d시%02d분%02d초\n", 
				sysTime.GetYear(), sysTime.GetMonth(), sysTime.GetDay(), 
				sysTime.GetHour(), sysTime.GetMinute(), sysTime.GetSecond()		
			);
	fprintf(fp, "============================================\n");


	if (m_bstrSource.length() != 0) {
		fprintf(fp, "[Source] \n");
		fprintf(fp, "%s\n", (LPCSTR)m_bstrSource);
	}

	if (m_bstrDescription.length() != 0) {
		fprintf(fp, "[Description] \n");
		fprintf(fp, "%s\n", (LPCSTR)m_bstrDescription);
	}

	if (m_bstrSql.length() != 0) {
		fprintf(fp, "[Sql] \n");
		fprintf(fp, "%s\n", (LPCSTR)m_bstrSql);
	}

	if (m_bstrParameter.length() != 0) {
		fprintf(fp, "[Parameter] : \n");
		fprintf(fp, "%s\n", (LPCSTR)m_bstrParameter);
	}

	fprintf(fp, "\n\n");

	fclose(fp);
}

//////////////////////////////////////////////////////////////////////
// Inline log
void CLog::InlineLog(const _bstr_t &bstr, bool bDateFileName)
{
	CSysTime sysTime;
	sysTime = CSysTime::GetLocalTime();

	_bstr_t bstrSysTime = (LPCTSTR)sysTime.GetTime();
	_bstr_t bstrFileName = m_bstrFileName;

	if (bDateFileName) {
		bstrFileName = (LPCTSTR)MakeDateFileName(bstrFileName, sysTime.GetDate()); 
	}
	
	FILE * fp = NULL;
	if ((fp = fopen((LPCSTR)bstrFileName, "a+")) == NULL) {
		return ;
	}

	fprintf(fp, "============================================\n");
	fprintf(fp, "%04d/%02d/%02d %02d시%02d분%02d초\n", 
				sysTime.GetYear(), sysTime.GetMonth(), sysTime.GetDay(), 
				sysTime.GetHour(), sysTime.GetMinute(), sysTime.GetSecond()		
			);
	fprintf(fp, "============================================\n");

	fprintf(fp, "%s\n", (LPCSTR)bstr);
	fprintf(fp, "\n\n");

	fclose(fp);
}

//////////////////////////////////////////////////////////////////////
// Make FileName Add Date
_bstr_t CLog::MakeDateFileName(const _bstr_t &bstrFileName, const _bstr_t &bstrDate)
{
	const char DOT = '.';
	const char DIR_MARK = '\\';
	const char DIR_MARK2 = '/';

	char szFileName[SZ_SIZE + 1];
	char szRet[SZ_SIZE + 1];

	strcpy(szFileName, (LPCSTR)bstrFileName);
	int nLen = strlen(szFileName);

	int i;
	for (i = nLen - 1; i >= 0; i--) {
		if (szFileName[i] == DIR_MARK && szFileName[i] == DIR_MARK2) {
			i = -1;
			break;
		}

		if (szFileName[i] == DOT) 
			break ;
	}


	_bstr_t bstrFileNameIn;
	if (i < 0) {
		bstrFileNameIn += (_T("_") + bstrDate);
	}
	else {
		szFileName[i] = NULL;
		strcpy(szRet, szFileName);
		strcat(szRet, "_");
		strcat(szRet, (LPCSTR)bstrDate);
		strcat(szRet, ".");
		strcat(szRet, szFileName + i + 1);

		bstrFileNameIn = szRet;
	}

	return bstrFileNameIn;
}


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
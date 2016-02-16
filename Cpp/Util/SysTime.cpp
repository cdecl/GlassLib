// SysTime.cpp: implementation of the CSysTime class.
//
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SysTime.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSysTime::CSysTime()
{
	m_stTime.wYear = (unsigned short)0;
	m_stTime.wMonth = (unsigned short)0;
	m_stTime.wDay = (unsigned short)0;
	m_stTime.wHour = (unsigned short)0;
	m_stTime.wMinute = (unsigned short)0;
	m_stTime.wSecond = (unsigned short)0;
	m_stTime.wDayOfWeek = (unsigned short)0;
	m_stTime.wMilliseconds = (unsigned short)0;
}

CSysTime::CSysTime(CSysTime & sysTime)
{
	m_stTime = sysTime.m_stTime; 
}

//////////////////////////////////////////////////////////////////////
// Get Current Local Time 
CSysTime CSysTime::GetLocalTime()
{
	CSysTime retTime;
	::GetLocalTime(&retTime.m_stTime); 

	retTime.m_stTime.wMilliseconds = (unsigned short)0;

	return retTime;
}


//////////////////////////////////////////////////////////////////////
// GetTime 
// Format : YYYYMMDDHHMMSS
_bstr_t CSysTime::GetTime()
{
	char szTime[15];
	sprintf(szTime, "%04d%02d%02d%02d%02d%02d", 
				m_stTime.wYear, 
				m_stTime.wMonth,  
				m_stTime.wDay,
				m_stTime.wHour,
				m_stTime.wMinute,
				m_stTime.wSecond
	);

	return szTime;
}

//////////////////////////////////////////////////////////////////////
// GetTime 
// Format : YYYYMMDD
_bstr_t CSysTime::GetDate()
{
	char szTime[15];
	sprintf(szTime, "%04d%02d%02d", 
				m_stTime.wYear, 
				m_stTime.wMonth,  
				m_stTime.wDay
	);

	return szTime;
}

//////////////////////////////////////////////////////////////////////
// GetYear 
// Format : YYYY
int CSysTime::GetYear()
{	
	return (int)m_stTime.wYear;
}

//////////////////////////////////////////////////////////////////////
// GetSecond 
// Format : SS
int CSysTime::GetSecond()
{
	return (int)m_stTime.wSecond;
}

//////////////////////////////////////////////////////////////////////
// GetMinute 
// Format : MM
int CSysTime::GetMinute()
{
	return (int)m_stTime.wMinute;
}

//////////////////////////////////////////////////////////////////////
// GetHour 
// Format : HH
int CSysTime::GetHour()
{
	return (int)m_stTime.wHour;
}

//////////////////////////////////////////////////////////////////////
// GetDay 
// Format : DD
int CSysTime::GetDay()
{
	return (int)m_stTime.wDay;
}

//////////////////////////////////////////////////////////////////////
// GetMonth 
// Format : MM
int CSysTime::GetMonth()
{
	return (int)m_stTime.wMonth;
}

//////////////////////////////////////////////////////////////////////
// GetDayOfWeek 
// return : int 
// 0: sunday, 1: MonDay, go on ...
int CSysTime::GetDayOfWeek()
{
	return int(m_stTime.wDayOfWeek); 
}

//////////////////////////////////////////////////////////////////////
// String Formatted 
_bstr_t CSysTime::Format(int nFormatType)
{
	char szTime[15];
	strcpy(szTime, (LPCSTR)this->GetTime());

	szTime[nFormatType] = NULL;
	return szTime;
}

//////////////////////////////////////////////////////////////////////
// Add Date 
void CSysTime::operator+=(int nAddDay)
{
	if (nAddDay == 0)
		return ;

	int nRemainDay = 0;
	int nYear = (int)m_stTime.wYear;
	int nMonth = (int)m_stTime.wMonth;
	int nDay = (int)m_stTime.wDay;
	
	nDay += nAddDay;

	int nMaxDay = GetMaxDayOfMonth(nMonth, nYear);
	while (nDay > nMaxDay) {
		nDay -= GetMaxDayOfMonth(nMonth, nYear);
		if (nMonth == 12) {
			nYear += 1;
			nMonth = 1;
		}
		else {
			nMonth += 1;
		}
		nMaxDay = GetMaxDayOfMonth(nMonth, nYear);
	}

	m_stTime.wYear = (unsigned short)nYear;
	m_stTime.wMonth = (unsigned short)nMonth;
	m_stTime.wDay = (unsigned short)nDay;

	SyncDayOfWeek(nAddDay);
}

//////////////////////////////////////////////////////////////////////
// Sub Date 
void CSysTime::operator-=(int nSubDay)
{
	if (nSubDay == 0)
		return ;

	int nYear = (int)m_stTime.wYear;
	int nMonth = (int)m_stTime.wMonth;
	int nDay = (int)m_stTime.wDay;
	
	nDay -= nSubDay;

	int nMaxDay = GetMaxDayOfMonth(nMonth, nYear);
	while (nDay <= 0 ) {
		if (nMonth == 1L) {
			nYear -= 1;
			nMonth = 12;
		}
		else {
			nMonth -= 1;
		}

		nDay = GetMaxDayOfMonth(nMonth, nYear) + nDay;		
		nMaxDay = GetMaxDayOfMonth(nMonth, nYear);
	}

	m_stTime.wYear = (unsigned short)nYear;
	m_stTime.wMonth = (unsigned short)nMonth;
	m_stTime.wDay = (unsigned short)nDay;

	SyncDayOfWeek((nSubDay * -1));
}

CSysTime CSysTime::operator+(int nAddDay)
{
	int nYear = (int)m_stTime.wYear;
	int nMonth = (int)m_stTime.wMonth;
	int nDay = (int)m_stTime.wDay;
	int nDayOfWeek = (int)m_stTime.wDayOfWeek;
	
	nDay += nAddDay;

	int nMaxDay = GetMaxDayOfMonth(nMonth, nYear);
	while (nDay > nMaxDay) {
		nDay -= GetMaxDayOfMonth(nMonth, nYear);
		if (nMonth == 12) {
			nYear += 1;
			nMonth = 1;
		}
		else {
			nMonth += 1;
		}
		nMaxDay = GetMaxDayOfMonth(nMonth, nYear);
	}

	CSysTime sysTime;
	sysTime.m_stTime.wYear = (unsigned int)nYear;
	sysTime.m_stTime.wMonth = (unsigned int)nMonth;
	sysTime.m_stTime.wDay = (unsigned int)nDay;
	sysTime.SyncDayOfWeek(nAddDay);
	return sysTime;
}

//////////////////////////////////////////////////////////////////////
// Sub Date 
CSysTime CSysTime::operator-(int nSubDay)
{
	int nRemainDay = 0;
	int nYear = (int)m_stTime.wYear;
	int nMonth = (int)m_stTime.wMonth;
	int nDay = (int)m_stTime.wDay;
	int nDayOfWeek = (int)m_stTime.wDayOfWeek;
	
	nDay -= nSubDay;

	int nMaxDay = GetMaxDayOfMonth(nMonth, nYear);
	while (nDay <= 0 ) {
		if (nMonth == 1) {
			nYear -= 1;
			nMonth = 12;
		}
		else {
			nMonth -= 1;
		}

		nDay = GetMaxDayOfMonth(nMonth, nYear) + nDay;		
		nMaxDay = GetMaxDayOfMonth(nMonth, nYear);
	}

	CSysTime sysTime;
	sysTime.m_stTime.wYear = (unsigned int)nYear;
	sysTime.m_stTime.wMonth = (unsigned int)nMonth;
	sysTime.m_stTime.wDay = (unsigned int)nDay;
	sysTime.SyncDayOfWeek(nSubDay * -1);
	return sysTime;
}

//////////////////////////////////////////////////////////////////////
// format : YYYYMMDDHHMMSS
CSysTime& CSysTime::operator=(CSysTime &sysTime)
{
	m_stTime = sysTime.m_stTime; 
	return sysTime;
}

//////////////////////////////////////////////////////////////////////
// format : YYYYMMDDHHMMSS
bool CSysTime::operator==(CSysTime &sysTime)
{
	if (GetTime() == sysTime.GetTime())
		return true;
	else 
		return false;
}

//////////////////////////////////////////////////////////////////////
// format : YYYYMMDDHHMMSS
bool CSysTime::operator<=(CSysTime &sysTime)
{
	if (GetTime() <= sysTime.GetTime())
		return true;
	else 
		return false;

}

//////////////////////////////////////////////////////////////////////
// format : YYYYMMDDHHMMSS
bool CSysTime::operator>=(CSysTime &sysTime)
{
	if (GetTime() >= sysTime.GetTime())
		return true;
	else 
		return false;

}

//////////////////////////////////////////////////////////////////////
// format : YYYYMMDDHHMMSS
bool CSysTime::operator<(CSysTime &sysTime)
{
	if (GetTime() < sysTime.GetTime())
		return true;
	else 
		return false;

}

//////////////////////////////////////////////////////////////////////
// format : YYYYMMDDHHMMSS
bool CSysTime::operator>(CSysTime &sysTime)
{
	if (GetTime() > sysTime.GetTime())
		return true;
	else 
		return false;

}

//////////////////////////////////////////////////////////////////////
// format : YYYYMMDDHHMMSS
bool CSysTime::operator!=(CSysTime &sysTime)
{
	if (GetTime() != sysTime.GetTime())
		return true;
	else 
		return false;

}

//////////////////////////////////////////////////////////////////////
// Leap Year : true
// Common Year : false 
int CSysTime::GetMaxDayOfMonth(int nMonth, int nYear)
{
	if (nMonth == 1 || nMonth == 3 || nMonth == 5 || nMonth == 7 ||
						nMonth == 8 || nMonth == 10 || nMonth == 12) {
		return 31;
	}
	else if (nMonth == 4 || nMonth == 6 || nMonth == 9 || nMonth == 11) {
		return 30;
	}
	else if (nMonth == 2) {
		return IsLeapYear(nYear) ? 29 : 28 ;	
	}
	else {
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////
// Leap Year : true
// Common Year : false 
bool CSysTime::IsLeapYear(int nYear)
{
	bool bRetVal = false;
	int nRem = 0;
	div_t divt;

	divt = div(nYear, 4);

	if (!divt.rem) 
		bRetVal = true;
	else 
		return false;
	
	divt = div(nYear, 100);
	if (!divt.rem) 
		bRetVal = false;
	else 
		return true;
	
	divt = div(nYear, 400);
	if (!divt.rem) 
		bRetVal = true;

	return bRetVal;
}

//////////////////////////////////////////////////////////////////////
// Calculate DayOfWeek
void CSysTime::SyncDayOfWeek(int nDay)
{
	int nDayOfWeek = (int)m_stTime.wDayOfWeek;
	div_t divt;

	if (nDay > 0L) {
		divt = div(nDay, 7);
		nDayOfWeek += divt.rem; 

		if (nDayOfWeek > 6) 
			nDayOfWeek -= 7;
	} // if 
	else {
		divt = div(abs(nDay), 7);
		nDayOfWeek -= divt.rem; 

		if (nDayOfWeek < 0) 
			nDayOfWeek += 7;
	} // else
	
	m_stTime.wDayOfWeek = (unsigned short)nDayOfWeek;

}


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
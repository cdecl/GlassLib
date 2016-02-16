// SysTime.h: interface for the CSysTime class.
//
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_SYSTIME_H__5E34DA66_ED9D_11D3_8A0D_00105AC6D631__INCLUDED_)
#define AFX_SYSTIME_H__5E34DA66_ED9D_11D3_8A0D_00105AC6D631__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <comdef.h>


///////////////////////
// constant define
enum {
	STIME_YEAR = 4,
	STIME_MONTH = 6,
	STIME_DAY = 8,
	STIME_HOUR = 10,
	STIME_MINUTE = 12, 
	STIME_SECOND = 14
};


class CSysTime  
{

public:
	CSysTime();
	CSysTime(CSysTime &sysTime);
	virtual ~CSysTime() {};

public:
	_bstr_t Format(int nFormatType = STIME_SECOND);
	bool operator<=(CSysTime &sysTime);
	bool operator>=(CSysTime &sysTime);
	bool operator<(CSysTime &sysTime);
	bool operator>(CSysTime &sysTime);
	bool operator!=(CSysTime &sysTime);
	bool operator==(CSysTime &sysTime);
	CSysTime& operator=(CSysTime &sysTime);
	void operator+=(int nAddDay);
	void operator-=(int nSubDay);
	CSysTime operator+(int nAddDay);
	CSysTime operator-(int nSubDay);
	int GetDayOfWeek();
	int GetSecond();
	int GetMinute();
	int GetHour();
	int GetDay();
	int GetMonth();
	int GetYear();
	_bstr_t GetTime();
	_bstr_t GetDate();

public:
	static CSysTime GetLocalTime();	// static function

private:
	void SyncDayOfWeek(int nDay);
	int GetMaxDayOfMonth(int nMonth, int nYear);
	bool IsLeapYear(int nYear);

private:
	SYSTEMTIME m_stTime;

};

#endif // !defined(AFX_SYSTIME_H__5E34DA66_ED9D_11D3_8A0D_00105AC6D631__INCLUDED_)

//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
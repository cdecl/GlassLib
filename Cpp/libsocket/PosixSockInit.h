//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#ifndef __POSIXSOCKINIT_H__BY_CDECL__
#define __POSIXSOCKINIT_H__BY_CDECL__

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>  
#include <netinet/in.h> 
#include <netdb.h>		// hostent

#include <pthread.h>
#include <string>

typedef int	SOCKET;

#define SOCKET_ERROR	(-1)
#define INVALID_SOCKET	(SOCKET)(~0)

#define SD_RECEIVE      0x00
#define SD_SEND         0x01
#define SD_BOTH         0x02


//////////////////////////////////////////////////////////////////////
// Exception 클래스 
//////////////////////////////////////////////////////////////////////
class WinSockException 
{
public:
	explicit WinSockException(const std::string str = "Unknown Exception", int n = 0) : str_(str), n_(n) {}
	WinSockException(const WinSockException &wse) : str_(wse.str_), n_(wse.n_) {}
	virtual ~WinSockException() {}

public:
	WinSockException& operator=(const WinSockException &wse) 
	{
		if (this == &wse) {
			return *this;
		}

		str_ = wse.str_;
		n_ = wse.n_;
		return *this;
	}

	std::string GetMessage() const
	{
		return str_;
	}

	int GetErrCode() const
	{
		return n_;
	}

private:
	std::string str_;
	int n_;
};



//////////////////////////////////////////////////////////////////////
// CriticalSection 관리 클래스 
//////////////////////////////////////////////////////////////////////
class CriticalSection
{
public:
	CriticalSection() 
	{
		pthread_mutex_init(&cs_, NULL);
	}

	~CriticalSection() 
	{
		pthread_mutex_destroy(&cs_);
	}

	pthread_mutex_t* operator&()
	{
		return &cs_;
	}

private:
	CriticalSection(const CriticalSection&);
	CriticalSection& operator=(const CriticalSection&);

private:
	pthread_mutex_t cs_;
};


//////////////////////////////////////////////////////////////////////
// CriticalSection 락 관리 클래스 
// 리소스의 획득은 초기화 이다 
//////////////////////////////////////////////////////////////////////
class CriticalSectionLock
{
public:
	//////////////////////////////////////////////////////////////////
	// EnterCriticalSection
	CriticalSectionLock(pthread_mutex_t* pcs) : pcs_(pcs) 
	{ 
		pthread_mutex_lock(pcs_); 
	}

	//////////////////////////////////////////////////////////////////
	// LeaveCriticalSection
	~CriticalSectionLock() 
	{ 
		pthread_mutex_unlock(pcs_); 
	}

private:
	pthread_mutex_t* pcs_;
};



#endif



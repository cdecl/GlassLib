// Socket Library 
// Version : 1.0
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSOCKET_H__8F398A4F_8D72_4EBB_BE58_910571565F2B__INCLUDED_)
#define AFX_SERVERSOCKET_H__8F398A4F_8D72_4EBB_BE58_910571565F2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerSocket.h : header file
//
#include <queue>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CServerSocket command target
class CServer;

class CServerSocket : public CSocket
{

// Attributes
public:
	CServer *m_pServer;

// Operations
public:
	CServerSocket(CServer *pServer);
	virtual ~CServerSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSocket)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CServerSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:

public:
	BOOL IsValidSocket();

private:
	CSocketFile *m_pSocketFile;

public:
	void CreateArchive();
	CArchive *m_pArchiveLoad;
	CArchive *m_pArchiveStore;
	
private:
	void Release();



};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSOCKET_H__8F398A4F_8D72_4EBB_BE58_910571565F2B__INCLUDED_)

//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
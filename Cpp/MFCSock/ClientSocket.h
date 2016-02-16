// Socket Library 
// Version : 1.0
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTSOCKET_H__768A9063_6021_4DE7_B088_16C74E689CF1__INCLUDED_)
#define AFX_CLIENTSOCKET_H__768A9063_6021_4DE7_B088_16C74E689CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//

#include <algorithm>
#include <queue>
using namespace std;

#include <afxmt.h>

class CClient;
/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target

class CClientSocket : public CSocket
{
public:
	
// Attributes
public:
	CClient *m_pClient;

// Operations
public:
	CClientSocket(CClient *pClient);
	virtual ~CClientSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:

public:
	BOOL IsValidSocket();
	void Release();
	
private:
	CSocketFile *m_pSocketFile;

public:
	void CreateArchive();
	CArchive *m_pArchiveLoad;
	CArchive *m_pArchiveStore;
	
private:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__768A9063_6021_4DE7_B088_16C74E689CF1__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
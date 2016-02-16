// Socket Library 
// Version : 1.0
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTENSOCKET_H__74BF052A_BAD3_4DA4_BC8E_643C98DC86EF__INCLUDED_)
#define AFX_LISTENSOCKET_H__74BF052A_BAD3_4DA4_BC8E_643C98DC86EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListenSocket.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CListenSocket command target

class CServer;

class CListenSocket : public CSocket
{
// Attributes
public:
	CServer * m_pServer;

// Operations
public:
	CListenSocket(CServer *pServer);
	virtual ~CListenSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CListenSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:

public:
	BOOL IsValidSocket();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENSOCKET_H__74BF052A_BAD3_4DA4_BC8E_643C98DC86EF__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
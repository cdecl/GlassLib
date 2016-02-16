// ListenSocket.cpp : implementation file
//
// Socket Library 
// Version : 1.0
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ListenSocket.h"
#include "Server.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenSocket

CListenSocket::CListenSocket(CServer *pServer)
{
	m_pServer = pServer;
}

CListenSocket::~CListenSocket()
{
	m_pServer = NULL;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenSocket member functions

void CListenSocket::OnAccept(int nErrorCode) 
{
	// Success
	if (!nErrorCode) {
		m_pServer->OnAccept(this);
	}
	else {
		// Error 
	}
	
	CSocket::OnAccept(nErrorCode);
}

BOOL CListenSocket::IsValidSocket()
{
	return (m_hSocket == INVALID_SOCKET) ? FALSE : TRUE;
}


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
// ServerSocket.cpp : implementation file
//
// Socket Library 
// Version : 1.0
// Programmed by cdecl(byung-kyu kim)
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerSocket.h"
#include "Server.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSocket

CServerSocket::CServerSocket(CServer *pServer) 
{
	m_pServer = pServer;

	m_pSocketFile = NULL;
	m_pArchiveLoad = NULL;
	m_pArchiveStore = NULL;
}

CServerSocket::~CServerSocket()
{
	m_pServer = NULL;
	Release();
}

void CServerSocket::CreateArchive()
{
	if (!m_pSocketFile) {
		m_pSocketFile = new CSocketFile(this);
	}

	if (!m_pArchiveLoad) {
		m_pArchiveLoad = new CArchive(m_pSocketFile, CArchive::load);
	}

	if (!m_pArchiveStore) {
		m_pArchiveStore = new CArchive(m_pSocketFile, CArchive::store);
	}
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerSocket, CSocket)
	//{{AFX_MSG_MAP(CServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerSocket member functions


void CServerSocket::Release()
{
	// SocketFile을 해제하기 전에 Archive를 먼저 해제해야함 
	if (m_pArchiveLoad) {
		delete m_pArchiveLoad;
		m_pArchiveLoad = NULL;
	}

	if (m_pArchiveStore) {
		delete m_pArchiveStore;
		m_pArchiveStore = NULL;
	}

	if (m_pSocketFile) {
		delete m_pSocketFile;
		m_pSocketFile = NULL;
	}
}


void CServerSocket::OnClose(int nErrorCode) 
{
	Release();

	// Success
	if (!nErrorCode) {
		m_pServer->OnClose(this); 	
	}
	else {

	}
	
	CSocket::OnClose(nErrorCode);
}

void CServerSocket::OnReceive(int nErrorCode) 
{
	// Success
	if (!nErrorCode) {
		m_pServer->OnReceive(this); 	
	}
	else {

	}
	
	CSocket::OnReceive(nErrorCode);
}

BOOL CServerSocket::IsValidSocket()
{
	return (m_hSocket == INVALID_SOCKET) ? FALSE : TRUE;
}


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
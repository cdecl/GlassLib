// ClientSocket.cpp : implementation file
//
// Socket Library 
// Version : 1.0
// Programmed by cdecl(byung-kyu kim)
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ClientSocket.h"

#include "Client.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket(CClient *pClient)
{
	m_pClient = pClient; 

	m_pSocketFile = NULL;
	m_pArchiveLoad = NULL;
	m_pArchiveStore = NULL;
}

CClientSocket::~CClientSocket()
{
	Release();
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::OnClose(int nErrorCode) 
{
	if (!nErrorCode) {
		m_pClient->OnClose(this);
	}
	
	CSocket::OnClose(nErrorCode);
}

void CClientSocket::OnReceive(int nErrorCode) 
{
	if (!nErrorCode) {
		m_pClient->OnRecieve(this);	
	}
	
	CSocket::OnReceive(nErrorCode);
}


BOOL CClientSocket::IsValidSocket()
{
	return (m_hSocket == INVALID_SOCKET) ? FALSE : TRUE;
}


void CClientSocket::CreateArchive()
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

void CClientSocket::Release()
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


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
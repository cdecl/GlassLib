// Client.h: interface for the CClient class.
//
// Socket Library 
// Version : 1.0
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENT_H__89C212E9_E857_487A_BB35_3631FA437B40__INCLUDED_)
#define AFX_CLIENT_H__89C212E9_E857_487A_BB35_3631FA437B40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define UM_SOCKET_RECV_C	WM_USER	+ 200
#define UM_SOCKET_CLOSE_C	WM_USER	+ 201


#include "ClientSocket.h"

class CClient  
{
private:
	CClientSocket *m_pSocket;
	BOOL m_bResponse;
	CString m_strPacket;
	CWnd *m_pWnd;

private:
	CString GetPacket(CClientSocket *pSocket);
	

public:
	BOOL WaitingRecieve(UINT uElapse = 5000);
	BOOL Send(const CString &strMsg);
	void Release();
	BOOL Connect(const CString &strAddress, UINT uPort);
	void OnClose(CClientSocket *pSocket);
	void OnRecieve(CClientSocket *pSocket);
	CString GetRecvMessage();
	BOOL IsConnecting();

public:
	void Processing(CClientSocket *pSocket, const CString &strMessage);
	CClient(CWnd *pWnd = NULL);
	virtual ~CClient();

private:
	static UINT ProcessingReceive(LPVOID lpvParam);

};

#endif // !defined(AFX_CLIENT_H__89C212E9_E857_487A_BB35_3631FA437B40__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
// Server.h: interface for the CServer class.
//
// Socket Library 
// Version : 1.0
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVER_H__B4288A2D_B36E_405A_9BAF_B8FF84A874F7__INCLUDED_)
#define AFX_SERVER_H__B4288A2D_B36E_405A_9BAF_B8FF84A874F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//////////////////////////////////////////////////////////////////////
// User Window Message Define 
#define UM_SOCKET_ACCEPT	WM_USER + 100
#define UM_SOCKET_CLOSE		WM_USER + 101
#define UM_SOCKET_RECV		WM_USER + 102

#include <list>
using namespace std;

#include "ListenSocket.h"
#include "ServerSocket.h"
#include <afxmt.h>


class CServer  
{
public:
	CServer(CWnd *pWnd = NULL);
	virtual ~CServer();

private:
	list<CServerSocket *> m_listSocket;
	CWnd *m_pWnd;
	CListenSocket *m_pListen;

	volatile BOOL m_bListening;

	CCriticalSection m_CSectionIn;
	CCriticalSection m_CSectionOut;

	CWinThread *m_pThread;

private:
	CString GetPacket(CServerSocket *pSocket);
	int Send(CServerSocket *pSocket, CString strMsg);
	
public:
	void OnClose(CServerSocket *pSocket);
	void OnAccept(CListenSocket *pSocket);
	void OnReceive(CServerSocket *pSocket);

	BOOL IsListening();
	BOOL Listen(UINT uPort);
	void Release();
	void Processing(CServerSocket *pSocket, const CString &strPacket);
	int GetSessionCount();
	void PushQueuePacket(CServerSocket *pSocket);	

public:
	static UINT ProcessingReceive(LPVOID lpvParam);

public:
	struct TPacket {
		CServer *pServer;
		CServerSocket *pSocket;
		CString strMessage;
	};

	typedef queue<TPacket> QueuePacket;
	QueuePacket m_q;	
};

#endif // !defined(AFX_SERVER_H__B4288A2D_B36E_405A_9BAF_B8FF84A874F7__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
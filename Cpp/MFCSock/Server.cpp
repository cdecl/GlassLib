// Server.cpp: implementation of the CServer class.
//
// Socket Library 
// Version : 1.0
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Server.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServer::CServer(CWnd *pWnd)
{
	::CoInitialize(NULL);

	m_pListen = NULL;
	m_bListening = FALSE;
	m_pWnd = pWnd;

	m_pThread = NULL;
}

CServer::~CServer()
{
	Release();
	m_pWnd = NULL;
	m_pThread = NULL;

	::CoUninitialize();
}

///////////////////////////////////////////////////////////////////////
// Garbage Collection
void CServer::Release()
{
	list<CServerSocket *>::iterator it = m_listSocket.begin();
	
	for (; it != m_listSocket.end(); it++) {
		(*it)->Close();
		delete (*it);
	}
	m_listSocket.clear();

	if (m_pListen) {
		m_pListen->Close();
		delete m_pListen;
		m_pListen = NULL;
	}

	m_bListening = FALSE;

	WaitForSingleObject(m_pThread->m_hThread, INFINITE);
	m_pThread = NULL;
}

///////////////////////////////////////////////////////////////////////
// Socket Accept Message
void CServer::OnAccept(CListenSocket *pSocket)
{
	CServerSocket *pServerSocket = new CServerSocket(this);
	CString strPeerName;
	UINT uPort;

	if (!pSocket->Accept(*pServerSocket)) {
		delete pServerSocket;
		AfxMessageBox(_T("Failed Accept !!"));
		return ;
	}
	pServerSocket->CreateArchive();

	pServerSocket->GetPeerName(strPeerName, uPort);
	m_listSocket.push_back(pServerSocket);

	if (m_pWnd) {
		::SendMessage(m_pWnd->m_hWnd, UM_SOCKET_ACCEPT, pServerSocket->m_hSocket, NULL);
	}
}

///////////////////////////////////////////////////////////////////////
// Socket Listen
BOOL CServer::Listen(UINT uPort)
{
	if (!IsListening()) {
		m_pListen = new CListenSocket(this);
		m_pListen->Create(uPort);

		if (m_pListen->Listen()) {
			m_bListening = TRUE;
			
			m_pThread = AfxBeginThread(ProcessingReceive, reinterpret_cast<LPVOID>(this));
			return m_bListening;
		}
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////
// Socket Close Message : Memory Free 
void CServer::OnClose(CServerSocket *pSocket)
{
	try {
		list<CServerSocket *>::iterator it = find(m_listSocket.begin(), m_listSocket.end(), pSocket);

		if (it != m_listSocket.end()) {
			(*it)->Close();
			delete (*it);
			m_listSocket.erase(it);
		}

		if (m_pWnd) 
			::SendMessage(m_pWnd->m_hWnd, UM_SOCKET_CLOSE, pSocket->m_hSocket, NULL);
	}
	catch (exception e) {

	}
	catch (...) {

	}
}

///////////////////////////////////////////////////////////////////////
// Socket Receive Message
void CServer::OnReceive(CServerSocket *pSocket)
{
	PushQueuePacket(pSocket);	// Decrypt

	if (m_pWnd) {
		::SendMessage(m_pWnd->m_hWnd, UM_SOCKET_RECV, pSocket->m_hSocket, NULL);
	}
}


///////////////////////////////////////////////////////////////////////
// Get Packet (using Archive)
void CServer::PushQueuePacket(CServerSocket *pSocket)
{
	CString strMessage;
	TPacket Packet;
	int nPacketLen = 0;

	do {
		Packet.pSocket = pSocket;
		Packet.pServer = this;
		Packet.strMessage = GetPacket(pSocket);

		m_q.push(Packet);
		//AfxBeginThread(ProcessingReceive, reinterpret_cast<LPVOID>(pSocket));

	} while (!pSocket->m_pArchiveLoad->IsBufferEmpty());
}

UINT CServer::ProcessingReceive(LPVOID lpvParam)
{
	::CoInitialize(NULL);
	
	CServer *pThis = reinterpret_cast<CServer *>(lpvParam);
	
	TPacket tpacket;
	while(pThis->m_bListening) {

		pThis->m_CSectionIn.Lock();						// Locking 
		if (!pThis->m_q.empty()) {
			tpacket = pThis->m_q.front();
			pThis->m_q.pop();

			pThis->Processing(tpacket.pSocket, tpacket.strMessage);		// 업무로직 처리 
		}
		pThis->m_CSectionIn.Unlock();					// UnLoking
		
		Sleep(1);
	}
	
	::CoUninitialize();
	return 0L;
}



void CServer::Processing(CServerSocket *pSocket, const CString &strPacket)
{
	Send(pSocket, strPacket);
}


///////////////////////////////////////////////////////////////////////
// Get Socket Buffer Packet (using Archive) 
CString CServer::GetPacket(CServerSocket *pSocket)
{
	char c;
	CString strMessage;

	do {
		// Packet 를 가져 온다 
		pSocket->m_pArchiveLoad->Read(&c, 1); 
		strMessage += c;

	} while (!pSocket->m_pArchiveLoad->IsBufferEmpty());

	return strMessage;
}

///////////////////////////////////////////////////////////////////////
// Listening status 
BOOL CServer::IsListening()
{
	return m_bListening;
}

///////////////////////////////////////////////////////////////////////
// Send Message
int CServer::Send(CServerSocket *pSocket, CString strMsg)
{
	if (pSocket) {
		m_CSectionOut.Lock(); 

		pSocket->m_pArchiveStore->Write(reinterpret_cast<const void *>((LPCSTR)strMsg), strMsg.GetLength());
		pSocket->m_pArchiveStore->Flush();		

		m_CSectionOut.Unlock();
	}

	return strMsg.GetLength() + sizeof(long);
}


///////////////////////////////////////////////////////////////////////
// Session Count
int CServer::GetSessionCount()
{
	return m_listSocket.size();
}


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
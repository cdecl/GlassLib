// Client.cpp: implementation of the CClient class.
//
// Socket Library 
// Version : 1.0
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Client.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace CUSTOMERSLib {
	const TCHAR STX[]	= _T("IVIP");
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClient::CClient(CWnd *pWnd)
{
	m_pSocket = NULL;
	m_bResponse = FALSE;

	m_pWnd = pWnd;
}

CClient::~CClient()
{
	Release();
}

//////////////////////////////////////////////////////////////////////
// Memory Free 
void CClient::Release()
{
	if (m_pSocket) {
		delete m_pSocket;
		m_pSocket = NULL;
	}

	m_bResponse = FALSE;
}

//////////////////////////////////////////////////////////////////////
// Close Message
void CClient::OnClose(CClientSocket *pSocket)
{
	Release();

	if (m_pWnd) {
		//::SendMessage(m_pWnd->m_hWnd, UM_SOCKET_CLOSE_C, pSocket->m_hSocket, NULL);
	}
}


//////////////////////////////////////////////////////////////////////
// Get Packet 
CString CClient::GetPacket(CClientSocket *pSocket)
{
	CString strPacket;
	char c;

	do {
		pSocket->m_pArchiveLoad->Read(&c, 1);
		strPacket += c;

	} while (!pSocket->m_pArchiveLoad->IsBufferEmpty());	
	
	return strPacket;
}

//////////////////////////////////////////////////////////////////////
// Socket Connect 
BOOL CClient::Connect(const CString &strAddress, UINT uPort)
{
	m_pSocket = new CClientSocket(this);

	if (!m_pSocket->Create()) {
		Release();
		return FALSE;
	}

	// 동기 소켓이기 때문 가능 
	if (!m_pSocket->Connect((LPCTSTR)strAddress, uPort)) {
		Release();
		return FALSE;
	}

	m_pSocket->CreateArchive(); 
	//AfxBeginThread(ProcessingReceive, reinterpret_cast<LPVOID>(m_pSocket));

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Is Connecting 
BOOL CClient::IsConnecting() 
{
	if (m_pSocket) {
		return static_cast<CClientSocket *>(m_pSocket)->IsValidSocket();
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// Message Send 
BOOL CClient::Send(const CString &strMsg)
{
	if (!m_pSocket) {
		return FALSE;
	}

	if (m_pSocket->IsValidSocket()) { 
		m_pSocket->m_pArchiveStore->Write(static_cast<LPCSTR>(strMsg), strMsg.GetLength());
		m_pSocket->m_pArchiveStore->Flush();
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Waiting Recive : Time out or Message Recieve
BOOL CClient::WaitingRecieve(UINT uElapse)
{
	MSG msg;
	UINT uTimerID = ::SetTimer(NULL, NULL, uElapse, NULL);

	while (!m_bResponse) {

		if (GetMessage(&msg, NULL, NULL, NULL)) {
			::TranslateMessage(&msg);

			if ((msg.message == WM_TIMER) && (msg.wParam == uTimerID)) {
				if (m_pSocket) {
					m_pSocket->CancelBlockingCall();		// Cancel Blokinging Call ??
				}
				break;
			} 
			::DispatchMessage(&msg);
		} 
	} 

	m_bResponse = FALSE;
	::KillTimer(NULL, uTimerID);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Recieve Message reutrn 
CString CClient::GetRecvMessage()
{
	return m_strPacket;
}

//////////////////////////////////////////////////////////////////////
// Recieve Message
void CClient::OnRecieve(CClientSocket *pSocket)
{
	m_bResponse = TRUE;

	static const int SZ_SIZE = 4096;

	CString strPacket;
	DWORD wBuffSize = 0L;
	UINT uRecvSize = 0;

	do {
		strPacket += GetPacket(pSocket);
		
	} while (!pSocket->m_pArchiveLoad->IsBufferEmpty());

	m_strPacket = strPacket;

	if (m_pWnd) {
		//::SendMessage(m_pWnd->m_hWnd, UM_SOCKET_RECV_C, pSocket->m_hSocket, NULL);
	}
}

//////////////////////////////////////////////////////////////////////
// Socket Class Queue에 있는것은 가져와 업무로직 합수를 Call 한다 
UINT CClient::ProcessingReceive(LPVOID lpvParam)
{

	return 0L;
}

void CClient::Processing(CClientSocket *pSocket, const CString &strMessage)
{
	//AfxMessageBox(strMessage);
}


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
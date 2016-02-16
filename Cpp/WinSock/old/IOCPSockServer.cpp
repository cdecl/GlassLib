// IOCPSockServer.cpp: implementation of the IOCPServer class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2003 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "IOCPSockServer.h"
using namespace GLASS;

//////////////////////////////////////////////////////////////////////
// PerSockContext class implememtation
//////////////////////////////////////////////////////////////////////

int PerSockContext::GetPeerName(sockaddr *pName, int *pNameLen) const
{
	return getpeername(sock_, pName, pNameLen);
}


int PerSockContext::GetPeerName(std::string &strPeerAddr, int &nPort) const
{
	// MFC CAsyncSocket 구현 
	sockaddr_in sockAddr = {0};

	int nSockAddrLen = sizeof(sockAddr);
	int nResult = GetPeerName((sockaddr*)&sockAddr, &nSockAddrLen);

	if (!nResult)
	{
		nPort = ntohs(sockAddr.sin_port);
		strPeerAddr = inet_ntoa(sockAddr.sin_addr);
	}

	return nResult;
}

int PerSockContext::GetSockName(sockaddr *pName, int *pNameLen) const
{
	return getsockname(sock_, pName, pNameLen);
}


int PerSockContext::GetSockName(std::string &strSockAddr, int &nPort) const
{
	// MFC CAsyncSocket 구현 
	sockaddr_in sockAddr = {0};

	int nSockAddrLen = sizeof(sockAddr);
	int nResult = GetSockName((sockaddr*)&sockAddr, &nSockAddrLen);

	if (!nResult)
	{
		nPort = ntohs(sockAddr.sin_port);
		strSockAddr = inet_ntoa(sockAddr.sin_addr);
	}

	return nResult;
}


int PerSockContext::GetSockOpt(int nOptionName, char *lpOptionValue, int *lpOptionLen, int nLevel) const
{
	return getsockopt(sock_, nLevel, nOptionName, lpOptionValue, lpOptionLen);
}

int PerSockContext::SetSockOpt(int nOptionName, const char *lpOptionValue, int nOptionLen, int nLevel)
{
	return setsockopt(sock_, nLevel, nOptionName, lpOptionValue, nOptionLen);
}

SOCKET PerSockContext::GetSocket() const 
{
	return sock_;
}

bool PerSockContext::IsValidSocket() const 
{
	return sock_ != INVALID_SOCKET;
}



//////////////////////////////////////////////////////////////////
// closesocket 호출 
void PerSockContext::CloseSocket(bool bGraceful)
{
	if (sock_ != INVALID_SOCKET) {
		if (!bGraceful) {
			LINGER  lingerStruct;
	
		    lingerStruct.l_onoff = 1;
			lingerStruct.l_linger = 0;

			SetSockOpt(SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct));
		}

		shutdown(sock_, SD_SEND);	// 우아한 종료 
		closesocket(sock_);
		sock_ = INVALID_SOCKET;
	}
}


bool PerSockContext::SendPostRemain(PerSockBuff* psb)
{
	size_t nSendSize = psb->GetRemainSize();
	if (!nSendSize) { return true; }

	DWORD dwNoOfByteSend = 0;
	psb->SetStatus(PerSockBuff::SOCK_SEND);

	WSABUF wsabuf = {0};
	wsabuf.buf = psb->GetBuff() + psb->GetSentSize();
	wsabuf.len = nSendSize;

	int nRet = WSASend(sock_, &wsabuf, 1, &dwNoOfByteSend, 0, &psb->overlapped, NULL); 
	if (SOCKET_ERROR == nRet && (ERROR_IO_PENDING != WSAGetLastError())) {
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////
// 비동기 Send Method
bool PerSockContext::SendPost(PerSockBuff* psb)
{
	DWORD dwNoOfByteSend = 0;
	psb->SetStatus(PerSockBuff::SOCK_SEND);

	int nRet = WSASend(sock_, psb->GetWsaBuf(), 1, &dwNoOfByteSend, 0, &psb->overlapped, NULL); 
	if (SOCKET_ERROR == nRet && (ERROR_IO_PENDING != WSAGetLastError())) {
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////
// 메세지 Recv 대기를 신청
bool PerSockContext::RecvWait()
{
	DWORD dwNoOfByteRecv = 0, dwFlag = 0;
	RecvBuff.SetStatus(PerSockBuff::SOCK_RECV);

	RecvBuff.SetTotalSize(RecvBuff.GetBuffSize());

	int nRet = WSARecv(sock_, RecvBuff.GetWsaBuf(), 1, &dwNoOfByteRecv, &dwFlag, &RecvBuff.overlapped, NULL);
	if (SOCKET_ERROR == nRet) {
		if (WSA_IO_PENDING != WSAGetLastError()) {
			return false;
		}
	}
	return true;
}




//////////////////////////////////////////////////////////////////
// PerSockBuffList class 
//////////////////////////////////////////////////////////////////
PerSockBuffList::PerSockBuffList() : bSendBuffUse_(false)
{ 
}

PerSockBuffList::~PerSockBuffList() 
{ 
	Release();
}

void PerSockBuffList::Release() 
{
	GLASS::CriticalSectionLock Lock(&CS_);

	std::for_each(SendBuffList_.begin(), SendBuffList_.end(), FreeBuffNodeFunc); 
	SendBuffList_.clear();
	bSendBuffUse_ = false;
}

PerSockBuff* PerSockBuffList::AllocPerSockBuff() 
{
	PerSockBuff *psb = NULL;

	if (bSendBuffUse_) {
		psb = new PerSockBuff;

		GLASS::CriticalSectionLock Lock(&CS_);
		SendBuffList_.push_back(psb);
	}
	else {
		psb = &SendBuff_;
		bSendBuffUse_ = true;
	}

	return psb;
}

void PerSockBuffList::FreePerSockBuff(WSAOVERLAPPED* lpOverlapped) 
{
	if (bSendBuffUse_ && (lpOverlapped == &SendBuff_.overlapped)) {
		bSendBuffUse_ = false;
	}
	else {
		GLASS::CriticalSectionLock Lock(&CS_);

		PerSockBuffListTypeIter itFind = 
				std::find_if(SendBuffList_.begin(), SendBuffList_.end(), std::bind2nd(FindBuff(), lpOverlapped));

		if (itFind != SendBuffList_.end()) {
			PerSockBuff *psb = *itFind;
			SendBuffList_.erase(itFind);
			delete psb;
		}
	}
}

PerSockBuff* PerSockBuffList::FindPerSockBuff(WSAOVERLAPPED* lpOverlapped) 
{
	if (bSendBuffUse_ && (lpOverlapped == &SendBuff_.overlapped)) {
		return &SendBuff_;
	}
	else {
		PerSockBuffListTypeIter itFind = 
				std::find_if(SendBuffList_.begin(), SendBuffList_.end(), std::bind2nd(FindBuff(), lpOverlapped));

		if (itFind != SendBuffList_.end()) {
			return *itFind;
		}
	}
	
	return NULL;
}

void PerSockBuffList::FreeBuffNodeFunc(PerSockBuff *psb) 
{
	delete psb;
}


int PerSockBuffList::GetListCount() const
{
	return SendBuffList_.size() + (bSendBuffUse_ ? 1 : 0);
}
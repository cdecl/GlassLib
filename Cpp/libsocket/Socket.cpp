// Socket.cpp
//////////////////////////////////////////////////////////////////////
// Copyright (c) 2004 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "Socket.h"
using namespace std;
using namespace GLASS;


//////////////////////////////////////////////////////////////////////////

Socket::Socket(SOCKET s) : hSocket_(s)
{
}

Socket::Socket(const Socket &pSocket) : hSocket_(pSocket.hSocket_)
{
}

Socket::~Socket() 
{
	Release();
}


Socket& Socket::operator=(const Socket &sock) 
{
	hSocket_ = sock.hSocket_;
	return *this;
}


bool Socket::operator==(const Socket &sock) const
{
	return GetHandle() == sock.GetHandle();
}


void Socket::Create() 
{
	hSocket_ = socket(PF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == hSocket_) {
		
	}
}

void Socket::Listen(unsigned int uPort) 
{
	sockaddr_in server_addr = {0};
	server_addr.sin_family = AF_INET;              
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(uPort); 

	if (SOCKET_ERROR == bind(hSocket_, reinterpret_cast<sockaddr *>(&server_addr), sizeof(sockaddr_in))) {
	}

	listen(hSocket_, 5);
}

bool Socket::Connect(const std::string &strIP, int uPort)
{
	hSocket_ = socket(PF_INET, SOCK_STREAM, 0);
	if(hSocket_ == INVALID_SOCKET) {
		return false;
	}

	sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(static_cast<short>(uPort));

	hostent *pHost = gethostbyname(strIP.c_str());
	memcpy(&addr.sin_addr, pHost->h_addr_list[0], sizeof(addr.sin_addr));

	int nSuccess = connect(hSocket_, (sockaddr*)&addr, sizeof(sockaddr_in));
	if (nSuccess == SOCKET_ERROR) {
		CloseSocket();
		return false;
	}

	return true;
}

SOCKET Socket::Accept()
{
	sockaddr_in client_addr = {0,};
	socklen_t nSocketAddrSize = sizeof(sockaddr);
	
	return accept(hSocket_, reinterpret_cast<sockaddr *>(&client_addr), &nSocketAddrSize);
}

int Socket::Send(const std::string &str) 
{
	return send(hSocket_, str.c_str(), str.size(), 0);
}

void Socket::CloseSocket()
{
	if (hSocket_ != INVALID_SOCKET) {
		shutdown(hSocket_, SD_SEND);

#ifdef _WIN32
		closesocket(hSocket_);
#else 
		close(hSocket_);
#endif
		hSocket_ = INVALID_SOCKET;
	}
}

void Socket::Release() 
{
	CloseSocket();
}

int Socket::Recv() 
{
	char szBuff[BUFF_SIZE];
	int nLen = recv(hSocket_, szBuff, BUFF_SIZE, 0);

	strBuff_ = std::string(szBuff, nLen);
	return nLen;
}

std::string& Socket::GetRecvBuff()
{
	return strBuff_;
}

SOCKET Socket::GetHandle() const 
{
	return hSocket_;
}

bool Socket::IsValidSocket() const 
{
	return hSocket_ != INVALID_SOCKET;
}

int Socket::GetPeerName(sockaddr *pName, socklen_t *pNameLen) const
{
	return getpeername(hSocket_, pName, pNameLen);
}

int Socket::GetPeerName(std::string &strPeerAddr, int &nPort) const
{
	// MFC CAsyncSocket 구현 
	sockaddr_in sockAddr = {0};

	socklen_t nSockAddrLen = sizeof(sockAddr);
	int nResult = GetPeerName((sockaddr*)&sockAddr, &nSockAddrLen);

	if (!nResult)
	{
		nPort = ntohs(sockAddr.sin_port);
		strPeerAddr = inet_ntoa(sockAddr.sin_addr);
	}

	return nResult;
}


int Socket::GetSockName(sockaddr *pName, socklen_t *pNameLen) const
{
	return getsockname(hSocket_, pName, pNameLen);
}


int Socket::GetSockName(std::string &strSockAddr, int &nPort) const
{
	// MFC CAsyncSocket 구현 
	sockaddr_in sockAddr = {0};

	socklen_t nSockAddrLen = sizeof(sockAddr);
	int nResult = GetSockName((sockaddr*)&sockAddr, &nSockAddrLen);

	if (!nResult)
	{
		nPort = ntohs(sockAddr.sin_port);
		strSockAddr = inet_ntoa(sockAddr.sin_addr);
	}

	return nResult;
}


int Socket::GetSockOpt(int nOptionName, char *lpOptionValue, socklen_t *lpOptionLen, int nLevel) const
{
	return getsockopt(hSocket_, nLevel, nOptionName, lpOptionValue, lpOptionLen);
}

int Socket::SetSockOpt(int nOptionName, const char *lpOptionValue, socklen_t nOptionLen, int nLevel)
{
	return setsockopt(hSocket_, nLevel, nOptionName, lpOptionValue, nOptionLen);
}


//////////////////////////////////////////////////////////////////////////
//

ClientSocket::ClientSocket(SOCKET s) : Socket(s)
{
}

ClientSocket::ClientSocket(const ClientSocket &pSocket) : Socket(pSocket)
{
}

ClientSocket::~ClientSocket() 
{
	CancelSelect();
}


bool ClientSocket::Connect(const std::string &strIP, int uPort)
{
	bool bResult = Socket::Connect(strIP, uPort);
	Select();

	return bResult;
}


void ClientSocket::Select() 
{
	SelectInfo *psi = new SelectInfo;
	psi->first = this;
	psi->second = 0;

#ifdef _WIN32
	unsigned int nThread = 0;
	SelectThread_.first = (HANDLE)_beginthreadex(NULL, 0, WorkerSelect, static_cast<void*>(psi), 0, &nThread);
	SelectThread_.second = nThread;
#else 
	pthread_create(&SelectThread_.first, NULL, WorkerSelect, static_cast<void*>(psi));
#endif
}

void ClientSocket::CancelSelect()
{
	if (SelectThread_.first) {
#ifdef _WIN32
		::PostThreadMessage(SelectThread_.second, WM_QUIT, 0, 0);
		::WaitForSingleObject(SelectThread_.first, INFINITE);
		::CloseHandle(SelectThread_.first);
		SelectThread_.first = NULL;
#else 
		pthread_cancel(SelectThread_.first); 
		pthread_join(SelectThread_.first, NULL);
		SelectThread_.first = 0;
#endif			
		SelectThread_.second = 0;
	}
}

void ClientSocket::WorkerSelectImp(int nWorkThreadCount) 
{
	fd_set read_fds;
	
	while (true) {
#ifdef _WIN32
		MSG msg;
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
		}
#endif
		if (!IsValidSocket()) {
			break;
		}
		
		FD_ZERO(&read_fds);			
		FD_SET(hSocket_, &read_fds);
		int nMaxDesc = hSocket_;

		timeval tv = {0};
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		int nSelCount = select(nMaxDesc + 1, &read_fds, NULL, NULL, &tv);

		if (SOCKET_ERROR == nSelCount) {
			break;
		}
		else if (0 == nSelCount) {
			continue;
		}

		if (FD_ISSET(hSocket_, &read_fds)) {
			int nLen = Recv();

			if (!nLen) {
				FD_CLR(hSocket_, &read_fds);
				OnClose(this);
				break;
			}
			else {
				if (IsValidSocket()) {
					OnRecv(this);
				}
			}
		}
	}	// while
}



#ifdef _WIN32
unsigned int __stdcall ClientSocket::WorkerSelect(void *pParam) 
#else 
void* ClientSocket::WorkerSelect(void *pParam) 
#endif
{
	SelectInfo *psi = reinterpret_cast<SelectInfo*>(pParam);
	ClientSocket *pThis = psi->first;
	int nThreadCount = psi->second;
	delete psi;

	pThis->WorkerSelectImp(nThreadCount);
	return 0;
}


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2004 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////





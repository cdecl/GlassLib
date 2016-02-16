//////////////////////////////////////////////////////////////////////
// Copyright (c) 2004 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////
#ifndef _GLASS_SCOKET_H__BY_CDECL_
#define _GLASS_SCOKET_H__BY_CDECL_

#ifdef _WIN32
 #include "WinSockInit.h"
#else
 #include "PosixSockInit.h"
#endif

#include <algorithm>
#include <string>
#include <list>
#include <functional>
#include <vector>
#include <deque>


namespace GLASS 
{


class Socket 
{
public:
	enum { BUFF_SIZE = 8192 };

public:
	Socket(SOCKET s = INVALID_SOCKET);
	Socket(const Socket &pSocket);
	virtual ~Socket();

public:
	Socket& operator=(const Socket &sock);
	bool operator==(const Socket &sock) const;

public:
	virtual void Create();
	virtual void Listen(unsigned int uPort);
	virtual bool Connect(const std::string &strIP, int uPort);
	virtual SOCKET Accept();
	virtual int Send(const std::string &str);
	virtual void CloseSocket();
	virtual void Release();
	virtual int Recv();
	virtual std::string& GetRecvBuff();
	virtual SOCKET GetHandle() const;
	virtual bool IsValidSocket() const;

public:
	virtual int GetPeerName(sockaddr *pName, socklen_t *pNameLen) const;
	virtual int GetPeerName(std::string &strPeerAddr, int &nPort) const;
	virtual int GetSockName(sockaddr *pName, socklen_t *pNameLen) const;
	virtual int GetSockName(std::string &strSockAddr, int &nPort) const;
	virtual int GetSockOpt(int nOptionName, char *lpOptionValue, socklen_t *lpOptionLen, int nLevel = SOL_SOCKET) const;
	virtual int SetSockOpt(int nOptionName, const char *lpOptionValue, socklen_t nOptionLen, int nLevel = SOL_SOCKET);

public:
	virtual void OnRecv(Socket *pSocket)
	{
	}

	virtual void OnClose(Socket *pSocket)
	{
	}

protected:
	SOCKET hSocket_;
	std::string strBuff_;
};




//////////////////////////////////////////////////////////////////////////
// Multiplexing Client Socket
class ClientSocket : public Socket
{
public:

#ifdef _WIN32
	typedef std::pair<HANDLE, unsigned int>			ThreadInfo;
#else
	typedef std::pair<pthread_t, unsigned int>		ThreadInfo;
#endif 

	typedef std::pair<ClientSocket*, int>	SelectInfo;

public:
	ClientSocket(SOCKET s = INVALID_SOCKET);
	ClientSocket(const ClientSocket &pSocket);
	virtual ~ClientSocket() ;

public:
	virtual void OnRecv(Socket *pSocket)
	{
	}

	virtual void OnClose(Socket *pSocket)
	{
	}

public:
	virtual bool Connect(const std::string &strIP, int uPort);

protected:
	virtual void Select();
	virtual void CancelSelect();
	virtual void WorkerSelectImp(int nWorkThreadCount);

protected:

#ifdef _WIN32
	static unsigned int __stdcall WorkerSelect(void *pParam); 
#else 
	static void* WorkerSelect(void *pParam); 
#endif

protected:
	ThreadInfo SelectThread_;
};



//////////////////////////////////////////////////////////////////////////
// Multiplexing Server Socket
template <class ClientSocketType = Socket>
class ServerSocket : public Socket
{
public:

#ifdef _WIN32
	typedef std::pair<HANDLE, unsigned int>			ThreadInfo;
#else
	typedef std::pair<pthread_t, unsigned int>		ThreadInfo;
#endif 

	typedef std::deque<ClientSocketType *>	MessageQueue;
	typedef typename MessageQueue::iterator	MessageQueueIter;

	typedef std::pair<ServerSocket*, int>	SelectInfo;

	typedef std::vector<ThreadInfo>			ThreadList;
	typedef ThreadList::iterator			ThreadListIter;

	typedef std::list<ClientSocketType *>	SockList;
	typedef typename SockList::iterator		SockListIter;
	

public:
	ServerSocket(SOCKET s = INVALID_SOCKET);
	ServerSocket(const ServerSocket &pSocket);
	virtual ~ServerSocket();

public:
	virtual void OnRecv(Socket *pSocket)
	{
	}

	virtual void OnClose(Socket *pSocket)
	{
	}


public:
	virtual void Listen(unsigned int uPort);
	virtual void Listen(unsigned int uPort, int nWorkThreadCount);
	virtual void Release();

protected:
	static ClientSocketType* AllocSocket(SOCKET hSocket);
	static void FreeSocket(ClientSocketType *pSocket);

protected:
	struct NullSocket : public std::unary_function<const ClientSocketType*, bool> 
	{
		bool operator()(const ClientSocketType *pSocket) const {
			return pSocket == NULL;
		}
	};

	struct FS_SET_ALL : public std::binary_function<ClientSocketType*, fd_set*, int>
	{
		int operator()(const ClientSocketType *pSocket, fd_set *fds) const {
			FD_SET(pSocket->GetHandle(), fds);
			return 0;
		}
	};

	struct CompareSocket : public std::binary_function<ClientSocketType*, ClientSocketType*, bool>
	{
		bool operator()(ClientSocketType *pSA, ClientSocketType *pSB) const {
			return pSA->GetHandle() < pSB->GetHandle();
		}
	};

public:
	int GetClientCount() const;

protected:
	void CreateWorkerThread(int nWorkThreadCount);
	void ExitWorkerThread();
	void WaitWorkerThread();
	void CleanWorkerThread();
	void AddSocketList(ClientSocketType *pSocket);
	void RemoveSocketList();
	void CleanQueueMessage(ClientSocketType *pSocket);
	void PushQueueMessage(ClientSocketType *pSocket);
	ClientSocketType* PopQueueMessage();

protected:
	virtual void Select(int nWorkThreadCount = 1);
	virtual void CancelSelect();
	virtual void WorkerSelectImp(int nWorkThreadCount);

protected:

//////////////////////////////////////////////////////////////////////////
// WorkThread 
#ifdef _WIN32
	static unsigned int __stdcall WorkerProcess(void *pParam);
#else 
	static void* WorkerProcess(void *pParam);
#endif


#ifdef _WIN32
	static unsigned int __stdcall WorkerSelect(void *pParam); 
#else 
	static void* WorkerSelect(void *pParam);
#endif
	
public:
	static void Sleep(int nMilliSecond);

protected:
	SockList listSocket_;
	ThreadList vThread_;

	ThreadInfo SelectThread_;

protected:
	MessageQueue mqueue_;
	CriticalSection mqcs_;
};



//////////////////////////////////////////////////////////////////////////
//

template <class ClientSocketType>
ServerSocket<ClientSocketType>::ServerSocket(SOCKET s) : Socket(s)
{
}

template <class ClientSocketType>
ServerSocket<ClientSocketType>::ServerSocket(const ServerSocket &pSocket) : Socket(pSocket)
{
}

template <class ClientSocketType>
ServerSocket<ClientSocketType>::~ServerSocket() 
{
	Release();
}


template <class ClientSocketType>
void ServerSocket<ClientSocketType>::Listen(unsigned int uPort) 
{
	Listen(uPort, 1);
}

template <class ClientSocketType>
void ServerSocket<ClientSocketType>::Listen(unsigned int uPort, int nWorkThreadCount) 
{
	Socket::Listen(uPort);
	Select(nWorkThreadCount);
}

template <class ClientSocketType>
void ServerSocket<ClientSocketType>::Release() 
{
	CancelSelect();
	Socket::Release();
}


template <class ClientSocketType>
ClientSocketType* ServerSocket<ClientSocketType>::AllocSocket(SOCKET hSocket)
{
	return new ClientSocketType(hSocket);
}

template <class ClientSocketType>
void ServerSocket<ClientSocketType>::FreeSocket(ClientSocketType *pSocket)
{
	delete pSocket;
}


template <class ClientSocketType>
int ServerSocket<ClientSocketType>::GetClientCount() const 
{
	return listSocket_.size();
}



template <class ClientSocketType>
void ServerSocket<ClientSocketType>::CreateWorkerThread(int nWorkThreadCount)
{
	vThread_.clear();
	vThread_.resize(nWorkThreadCount);

	ThreadListIter it = vThread_.begin();
	for (; it != vThread_.end(); ++it) {
#ifdef _WIN32
		it->first = (HANDLE)_beginthreadex(NULL, 0, WorkerProcess, static_cast<void*>(this), 0, &it->second);
#else 
		pthread_create(&it->first, NULL, WorkerProcess, static_cast<void*>(this));
#endif
	}
}


template <class ClientSocketType>
void ServerSocket<ClientSocketType>::ExitWorkerThread()
{
	ThreadListIter it = vThread_.begin();
	
	for (; it != vThread_.end(); ++it) {
#ifdef _WIN32
		unsigned int uThreadID = it->second;
		::PostThreadMessage(uThreadID, WM_QUIT, 0, 0);
#else 
		pthread_cancel(it->first); 
#endif
	}
}

template <class ClientSocketType>
void ServerSocket<ClientSocketType>::WaitWorkerThread()
{
#ifdef _WIN32
	::WaitForMultipleObjects(vThread_.size(), reinterpret_cast<CONST HANDLE*>(&*vThread_.begin()), TRUE, INFINITE);
#else 
	ThreadListIter it = vThread_.begin();
	for (; it != vThread_.end(); ++it) {
		pthread_join(it->first, NULL);
	}
#endif
}

template <class ClientSocketType>
void ServerSocket<ClientSocketType>::CleanWorkerThread()
{
	ExitWorkerThread();
	WaitWorkerThread();

#ifdef _WIN32
	ThreadListIter it = vThread_.begin();
	for (; it != vThread_.end(); ++it) {
		HANDLE hThread = it->first;
		CloseHandle(reinterpret_cast<HANDLE>(hThread));
	}
#endif
}

template <class ClientSocketType>
void ServerSocket<ClientSocketType>::AddSocketList(ClientSocketType *pSocket)
{
	listSocket_.push_back(pSocket);
}

template <class ClientSocketType>
void ServerSocket<ClientSocketType>::RemoveSocketList()
{
	SockListIter it = listSocket_.begin();
	for	(; it != listSocket_.end(); ++it) {
		if (!(*it)->IsValidSocket()) {
			CleanQueueMessage((*it));
			FreeSocket(*it);
			*it = NULL;
		}
	}
	listSocket_.remove_if(NullSocket());
}

template <class ClientSocketType>
void ServerSocket<ClientSocketType>::CleanQueueMessage(ClientSocketType *pSocket)
{
	CriticalSectionLock lock(&mqcs_); 

	MessageQueueIter itRemove = std::remove(mqueue_.begin(), mqueue_.end(), pSocket);
	mqueue_.erase(itRemove, mqueue_.end());
}

template <class ClientSocketType>
void ServerSocket<ClientSocketType>::PushQueueMessage(ClientSocketType *pSocket)
{
	CriticalSectionLock lock(&mqcs_); 

	mqueue_.push_back(pSocket);
}

template <class ClientSocketType>
ClientSocketType* ServerSocket<ClientSocketType>::PopQueueMessage()
{
	CriticalSectionLock lock(&mqcs_);

	ClientSocketType *pSocket = NULL;

	if (!mqueue_.empty()) {
		pSocket = mqueue_.front();
		mqueue_.pop_front();
	}

	return pSocket;
}


template <class ClientSocketType>
void ServerSocket<ClientSocketType>::Select(int nWorkThreadCount) 
{
	SelectInfo *psi = new SelectInfo;
	psi->first = this;
	psi->second = nWorkThreadCount;

#ifdef _WIN32
	unsigned int nThread = 0;
	SelectThread_.first = (HANDLE)_beginthreadex(NULL, 0, WorkerSelect, static_cast<void*>(psi), 0, &nThread);
	SelectThread_.second = nThread;
#else 
	pthread_create(&SelectThread_.first, NULL, WorkerSelect, static_cast<void*>(psi));
#endif
}


template <class ClientSocketType>
void ServerSocket<ClientSocketType>::CancelSelect()
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


template <class ClientSocketType>
void ServerSocket<ClientSocketType>::WorkerSelectImp(int nWorkThreadCount) 
{
	CreateWorkerThread(nWorkThreadCount);

	fd_set read_fds;
	
	while (IsValidSocket()) {
#ifdef _WIN32
		MSG msg;
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
		}
#endif
		
		FD_ZERO(&read_fds);			
		FD_SET(hSocket_, &read_fds);
		std::for_each(listSocket_.begin(), listSocket_.end(), bind2nd(FS_SET_ALL(), &read_fds));

		int nMaxDesc = hSocket_;
#ifdef _WIN32
#else
		SockListIter itMax = std::max_element(listSocket_.begin(), listSocket_.end(), CompareSocket());
		if (itMax != listSocket_.end()) {
			nMaxDesc = max((*itMax)->GetHandle(), hSocket_);	
		}
#endif

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
			SOCKET hSockClient = Accept();
			
			if (hSockClient != INVALID_SOCKET) {
				ClientSocketType *pClientSock = AllocSocket(hSockClient);

				AddSocketList(pClientSock);
				FD_SET(pClientSock->GetHandle(), &read_fds);
			}

			--nSelCount;
		}

		SockListIter it = listSocket_.begin();

		for	(; it != listSocket_.end() && nSelCount > 0; ++it) {
			ClientSocketType *pSocket = *it;

			if (FD_ISSET(pSocket->GetHandle(), &read_fds)) {
				int nLen = pSocket->Recv();
				
				if (!nLen) {
					FD_CLR(pSocket->GetHandle(), &read_fds);
					OnClose(pSocket);
				}
				else {
					PushQueueMessage(pSocket);
				}

				--nSelCount;
			}
		}	

		RemoveSocketList();
	}

	CleanWorkerThread();
}




//////////////////////////////////////////////////////////////////////////
// WorkThread
template <class ClientSocketType>
#ifdef _WIN32
unsigned int __stdcall ServerSocket<ClientSocketType>::WorkerProcess(void *pParam) 
#else 
void* ServerSocket<ClientSocketType>::WorkerProcess(void *pParam) 
#endif
{
	ServerSocket *pThis = reinterpret_cast<ServerSocket *>(pParam);
	
	while (true) {
#ifdef _WIN32
		MSG msg;
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
		}
#endif
		ClientSocketType *pSocket = pThis->PopQueueMessage();

		if (pSocket) {
			if (pSocket->IsValidSocket()) {
				pThis->OnRecv(pSocket);
			}
			continue;
		}

		ServerSocket::Sleep(1);
	}

	return 0;
}



template <class ClientSocketType>
#ifdef _WIN32
unsigned int __stdcall ServerSocket<ClientSocketType>::WorkerSelect(void *pParam) 
#else 
void* ServerSocket<ClientSocketType>::WorkerSelect(void *pParam) 
#endif
{
	SelectInfo *psi = reinterpret_cast<SelectInfo*>(pParam);
	ServerSocket *pThis = psi->first;
	int nThreadCount = psi->second;
	delete psi;

	pThis->WorkerSelectImp(nThreadCount);
	return 0;
}


template <class ClientSocketType>
void ServerSocket<ClientSocketType>::Sleep(int nMilliSecond)
{
#ifdef _WIN32
	::Sleep(nMilliSecond);
#else 
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = nMilliSecond * 1000;
	select( 0, NULL, NULL, NULL, &tv );
#endif
}



} // namespace GLASS;


#endif 


//////////////////////////////////////////////////////////////////////
// Copyright (c) 2004 by cdecl (byung-kyu kim)
// EMail : cdecl@interpark.com
//////////////////////////////////////////////////////////////////////


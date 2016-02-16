#include "StdAfx.h"
#include "AsioLib.h"



namespace GLASS {
namespace ASIO {

Session::Session(asio::io_service& io_service, TCPServer &s)
	: socket_(io_service), server_(s)
{
	TRACE(_T("Session()\n"));
}

Session::~Session()
{
	/*
	if (socket_.is_open()) {
		asio::socket_base::linger option(true, 0);
		socket_.set_option(option);
		socket_.shutdown(asio::ip::tcp::socket::shutdown_send);
		socket_.close();

		TRACE(_T("socket_.close();\n"));
	}
	*/
	TRACE(_T("~Session()\n"));
	
}

tcp::socket& Session::Socket()
{
	return socket_;
}

void Session::Start()
{
	ReadWait();
}

void Session::ReadWait()
{
	socket_.async_read_some(
		asio::buffer(rdata_, BUFF_SIZE), 
		boost::bind(&Session::HandleRead, this, asio::placeholders::error, asio::placeholders::bytes_transferred)
	);
}

void Session::Write(std::string str) 
{
	socket_.async_write_some(
		asio::buffer(str.c_str(), str.size()),
		boost::bind(&Session::HandleWrite, this, asio::placeholders::error)
	);


}


void Session::HandleRead(const asio::error_code& error, size_t bytes_transferred)
{
	if (!error)	{
		std::string str(rdata_, bytes_transferred);
		Write(str);

		ReadWait();
	}
	else {
		server_.DeleteSocket(this);
		//delete this;
	}

}


void Session::HandleWrite(const asio::error_code& error)
{
	if (!error) {
		TCHAR sz[128];
		//_stprintf(sz, _T("bytes_transferred : %u"), bytes_transferred);
		//TRACE(sz);
	}
	else {
		//delete this;
		server_.DeleteSocket(this);
	}
}


/////////////////////////////////////
// 

TCPServer::TCPServer()
{
}

TCPServer::~TCPServer()
{
	Stop();
}

void TCPServer::Release()
{
	sessions_.clear();
	Join();

	spAcceptor_.reset();
	spio_.reset();
}

void TCPServer::Listen(short port, size_t nThread)
{
	if (!spio_) {
		spio_.reset(new asio::io_service());
		spAcceptor_.reset(new tcp::acceptor(*spio_, tcp::endpoint(tcp::v4(), port)));

		for (int i = 0; i < nThread; ++i) {
			threads_.push_back(
				ThreadPtr(new asio::thread(boost::bind(&asio::io_service::run, &*spio_)))
			);
		}

		AcceptWait();
	}
}

void TCPServer::Join()
{
	Threads::iterator it = threads_.begin();
	Threads::iterator itEnd = threads_.end();

	for (; it != itEnd; ++it) {
		(*it)->join();

	}

	threads_.clear();
}

bool TCPServer::IsRun()
{
	return !(!spio_);
}

void TCPServer::Stop()
{
	if (spio_) {
		spio_->stop();
		Join();
	}

	Release();
}

size_t TCPServer::GetSessionCount()
{
	return sessions_.size();
}


void TCPServer::AcceptWait()
{
	SessionPtr spSession(new Session(*spio_, *this));
	sessions_.push_back(spSession);

	spAcceptor_->async_accept(spSession->Socket(),
		boost::bind(&TCPServer::HandleAccept, this, spSession, asio::placeholders::error)
	);

}

void TCPServer::HandleAccept(SessionPtr spSession, const asio::error_code& error)
{
	if (!error) {
		spSession->Start();
		AcceptWait();
	}
	else {
	}
}



bool TCPServer::FindSession(SessionPtr sp, Session *p)
{
	return sp.get() == p;
}


void TCPServer::DeleteSocket(Session *s) 
{
	Sessions::iterator it = std::find_if(
								sessions_.begin(), 
								sessions_.end(), 
								boost::bind(&TCPServer::FindSession, _1, s)
							);

	if (it != sessions_.end()) {
		//TRACE(_T("find_if \n"));
		sessions_.erase(it);
	}
}

}
}
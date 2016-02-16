#pragma once


#include <list>
#include <vector>

//#include <asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>



namespace GLASS {
namespace ASIO {


using asio::ip::tcp;

class TCPServer;


class Session
{
public:
	Session(asio::io_service& io_service, TCPServer &s);
	virtual ~Session();

	tcp::socket& Socket();
	void Start();

	void ReadWait();
	void Write(std::string str);

	void HandleRead(const asio::error_code& error, size_t bytes_transferred);
	void HandleWrite(const asio::error_code& error);

private:
	tcp::socket socket_;

	enum { BUFF_SIZE = 1024 };
	char rdata_[BUFF_SIZE];
	char sdata_[BUFF_SIZE];

	TCPServer &server_;
};



class TCPServer
{
public:
	typedef boost::shared_ptr<Session> SessionPtr;
	typedef std::list<SessionPtr> Sessions;

	typedef boost::shared_ptr<asio::io_service> IOServicePtr;
	typedef boost::shared_ptr<tcp::acceptor> AcceptorPtr;
	typedef boost::shared_ptr<asio::thread> ThreadPtr;
	typedef std::vector<ThreadPtr> Threads;

public:
	TCPServer();
	virtual ~TCPServer();

	void Listen(short port, size_t nThread = 1);
	void Join();
	bool IsRun();
	void Stop();
	size_t GetSessionCount();
	void AcceptWait();
	void HandleAccept(SessionPtr spSession, const asio::error_code& error);

	void DeleteSocket(Session *s);

public:
	static bool FindSession(SessionPtr sp, Session *p);

protected:
	void Release();

private:
	IOServicePtr spio_;
	AcceptorPtr spAcceptor_;
	Threads threads_; 

	Sessions sessions_;

};

} // GLASS
} // ASIO
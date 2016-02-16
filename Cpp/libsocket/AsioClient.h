

#pragma once

using boost::asio::ip::tcp;



struct IsEOF_Basic
{
	bool operator()(const std::string &)
	{
		return true;
	}
};


template <class Ty = IsEOF_Basic>
class ClientSocket 
{
public:
	ClientSocket() : socket_(io_service_), timeout_(io_service_), nResponse_(0) {}

	~ClientSocket() 
	{
		Close();
	}

	bool Connect(const string &sIP, const string &sPort) 
	{
		bool bResult = false;

		tcp::resolver resolver(io_service_);
		tcp::resolver::query query(tcp::v4(), sIP, sPort);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end)
		{
			socket_.close();
			socket_.connect(*endpoint_iterator++, error);
		}

		if (!error) {
			bResult = true;
		}
		
		return bResult;
	}

	void Send(const string &s, const int nTimeout = 5)
	{
		copy(s.begin(), s.end(), szBuff);

		socket_.async_write_some(boost::asio::buffer(szBuff, s.length()),
			boost::bind(&ClientSocket::handle_write, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

		timeout_.expires_from_now(boost::posix_time::seconds(nTimeout));
		timeout_.async_wait(boost::bind(&ClientSocket::Timeout, this));

		handle_read_wait();
	}

	int WaitForResponse()
	{
		socket_.io_service().reset();

		while (socket_.io_service().run_one()) 
		{
			if (nResponse_) break;
		}

		return nResponse_;
	}

	void Close()
	{
		socket_.close();
	}

	std::string GetReadBuffer()
	{
		return sReadBuff_;
	}

	enum { TIMEOUT = -1, READY = 0, SUCCESS = 1 };

private:
	void Timeout()
	{
		nResponse_ = TIMEOUT;
	}

	void handle_read_wait() 
	{
		socket_.async_read_some(boost::asio::buffer(szBuff, BUFF_SIZE),
			boost::bind(&ClientSocket::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

		nResponse_ = READY;
	}

	void handle_read(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error) {
			Close();
			return;
		}

		sReadBuff_.append(szBuff, szBuff + bytes_transferred);

		if (IsEOF(sReadBuff_)) {
			nResponse_ = SUCCESS;
		}
		else {
			handle_read_wait();
		}
	}

	void handle_write(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error) {
			Close();
			return;
		}
	}

private:
	boost::asio::io_service io_service_;

	boost::asio::deadline_timer timeout_;
	tcp::socket socket_;
	std::string sReadBuff_;
	volatile int nResponse_;

	Ty IsEOF;
	enum { BUFF_SIZE = 8192};
	char szBuff[BUFF_SIZE];
};

// server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;

#define TCP asio::ip::tcp

#define ASYNCHRONOUS  0 //异步
#define IOSTREAM      1 //IO流操作
#define UDPSOCKET	  1

class server
{
private:
	asio::io_service &ios;
	TCP::acceptor acceptor;
	typedef std::shared_ptr<TCP::socket> sock_pt;
public:
	server(asio::io_service& io) :ios(io), acceptor(ios, TCP::endpoint(TCP::v4(), 6688))
	{
		start();
	}
	void start()
	{
		sock_pt sock(new TCP::socket(ios));

		acceptor.async_accept(*sock,
			boost::bind(&server::accept_handler, this, boost::asio::placeholders::error, sock,9));
	}

	void accept_handler(const boost::system::error_code& ec, sock_pt sock,int x)
	{
		if (ec)
		{
			return;
		}

		cout << "client:";
		cout << sock->remote_endpoint().address() << endl;
		sock->async_write_some(boost::asio::buffer("hello asio"),
			boost::bind(&server::write_handler, this, boost::asio::placeholders::error));
		start();
	}

	void write_handler(const boost::system::error_code&)
	{
		cout << "send msg complete." << endl;
	}

};

int _tmain(int argc, _TCHAR* argv[])
#if UDPSOCKET
{
	std::cout << "udp server start." << std::endl;
	asio::io_service ios;
	asio::ip::udp::socket sock(ios, asio::ip::udp::endpoint(asio::ip::udp::v4(), 6699));
	for (;;)
	{
		char buf[1];
		asio::ip::udp::endpoint ep;

		boost::system::error_code ec;
		sock.receive_from(asio::buffer(buf), ep, 0, ec);

		if (ec && ec != boost::system::errc::message_size)
		{
			throw boost::system::system_error(ec);
		}

		std::cout << "send to " << ep.address() << std::endl;
		sock.send_to(asio::buffer("hello asio udp"), ep);
	}
}
#else
#if IOSTREAM
{
	asio::io_service ios;
	TCP::endpoint ep(TCP::v4(), 6688);
	TCP::acceptor acceptor(ios, ep);

	while (true)
	{
		TCP::iostream tcp_stream;
		acceptor.accept(*tcp_stream.rdbuf());
		tcp_stream << "hello tcp stream";
	}
}
#else
try
{
	cout << "server start." << endl;
	asio::io_service ios;
#if ASYNCHRONOUS
	server serv(ios);
	ios.run();
#else
	TCP::acceptor acceptor(ios, TCP::endpoint(TCP::v4(), 6688));
	cout << acceptor.local_endpoint().address() << endl;

	while (true)
	{
		TCP::socket sock(ios);
		acceptor.accept(sock);

		cout << "client:";
		cout << sock.remote_endpoint().address() << endl;

		sock.write_some(asio::buffer("hello asio"));
	}
#endif
}
catch (std::exception& e)
{
	cout << e.what() << endl;
}
#endif
#endif

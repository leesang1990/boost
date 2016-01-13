// client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include <boost/bind.hpp>
#include "..\boostlearn\asio.hpp"

#define ASYNCHRONOUS  0 //异步
#define IOSTREAM      1 //IO流操作
#define UDPSOCKET	  1

void client(boost::asio::io_service &ios)
try
{
	std::cout << "client start." << std::endl;

	TCP::socket sock(ios);
	TCP::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 6688);

	sock.connect(ep);

	std::vector<char> str(100, 0);
	sock.read_some(boost::asio::buffer(str));
	std::cout << "recive from " << sock.remote_endpoint().address();
	std::cout << &str[0] << std::endl;
}
catch (std::exception& e)
{
	std::cout << e.what() << std::endl;
}

class Cclient
{
private:
	boost::asio::io_service& ios;
	TCP::endpoint ep;
	typedef std::shared_ptr<TCP::socket> sock_pt;
public:
	Cclient(boost::asio::io_service& io) :ios(io),
		ep(boost::asio::ip::address::from_string("127.0.0.1"), 6688)
	{
		start();
	}

	void time_expired(const boost::system::error_code&, TCP::socket *sock)
	{
		std::cout << "time expired" << std::endl;
		sock->close();
	}

	void start()
	{
		sock_pt sock(new TCP::socket(ios));
		sock->async_connect(ep, boost::bind(&Cclient::conn_handler, this, boost::asio::placeholders::error, sock));

		//超时处理
		boost::asio::deadline_timer t(ios, boost::posix_time::seconds(5));
		t.async_wait(boost::bind(&Cclient::time_expired,this,boost::asio::placeholders::error, &*sock));
	}

	void conn_handler(const boost::system::error_code& ec, sock_pt sock)
	{
		if (ec)
		{
			return;
		}

		std::cout << "recive from " << sock->remote_endpoint().address()<<std::endl;
		std::shared_ptr<std::vector<char> > str(new std::vector<char>(100, 0));

		sock->async_read_some(boost::asio::buffer(*str),
			boost::bind(&Cclient::read_handler, this, boost::asio::placeholders::error, str));
		start();
	}

	void read_handler(const boost::system::error_code& ec, std::shared_ptr<std::vector<char> > str)
	{
		if (ec)
		{
			return;
		}
		std::cout << &(*str)[0] << std::endl;
	}
};

int _tmain(int argc, _TCHAR* argv[])
#if UDPSOCKET
{
	std::cout << "client start." << std::endl;
	boost::asio::io_service ios;

	UDP::endpoint send_ep(IP::address::from_string("127.0.0.1"), 6699);

	UDP::socket sock(ios);
	sock.open(UDP::v4());

	char buf[1];
	sock.send_to(boost::asio::buffer(buf), send_ep);

	std::vector<char> v(100, 0);
	UDP::endpoint recv_ep;

	sock.receive_from(boost::asio::buffer(v),recv_ep);
	std::cout << "recv from " << recv_ep.address() << " ";
	std::cout << &v[0] << std::endl;

	system("pause");
}
#else
#if  ASYNCHRONOUS
{
	//1.Synchronize
	boost::asio::io_service ios;
	a_timer at(ios,5,boost::bind(client,std::ref(ios)));
	ios.run();
}
#else
#if IOSTREAM
{
	for (int i = 0; i < 5; ++i)
	{
		TCP::iostream tcp_steam("127.0.0.1", "6688");
		std::string str;
		getline(tcp_steam, str);
		std::cout << str << std::endl;
	}
	system("pause");
}
#else
try
{
	std::cout << "client start." << std::endl;
	boost::asio::io_service ios;
	Cclient cl(ios);
	ios.run();
}
catch (std::exception& e)
{
	std::cout << e.what() << std::endl;
}
#endif
#endif
#endif


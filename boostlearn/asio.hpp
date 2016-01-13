#ifndef LEE_ASIO
#define LEE_ASIO

// #ifdef _MSC_VER
// #define _WIN32_WINNT 0X0501
// #endif
// #define BOOST_REGEX_NO_LIB
// #define BOOST_DATE_TIME_SOURCE
// #define BOOST_SYSTEM_NO_LIB

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

//Synchronize Õ¨≤Ω asynchronous “Ï≤Ω

#define IP  boost::asio::ip
#define TCP IP::tcp
#define UDP IP::udp

void print(boost::system::error_code e)
{
	std::cout << "hello asio" << std::endl;
}

void asio_usage()
{
	boost::asio::io_service ios;
	boost::asio::deadline_timer t(ios, boost::posix_time::seconds(2));
	//std::cout << t.expires_at() << std::endl;
	//t.wait();
	//std::cout << "hello asio" << std::endl;
	t.async_wait(print);

	std::cout << "it show before t expired." << std::endl;
	ios.run();
	
}

class a_timer
{
private:
	int count, count_max;
	std::function<void()> f;
	boost::asio::deadline_timer t;
public:
	template<typename F>
	a_timer(boost::asio::io_service& ios, int x, F func) :f(func), count_max(x), count(0), t(ios, boost::posix_time::millisec(500))
	{
		t.async_wait(boost::bind(&a_timer::call_func, this, boost::asio::placeholders::error));
	}

	void call_func(const boost::system::error_code&)
	{
		if (count >= count_max)
		{
			return;
		}
		++count;
		f();
		t.expires_at(t.expires_at() + boost::posix_time::millisec(500));
		t.async_wait(boost::bind(&a_timer::call_func, this, boost::asio::placeholders::error));
	}
};

void print1(){ std::cout << "hello asio" << std::endl; }
void print2(){ std::cout << "hello boost" << std::endl; }

void ip_usage()
{
	boost::asio::ip::address addr;
	addr = addr.from_string("127.0.0.1");
	assert(addr.is_v4());
	std::cout << addr.to_string() << std::endl;
	addr = addr.from_string("ab::12:34:56");
	assert(addr.is_v6());

	boost::asio::ip::address addr1;
	addr1 = addr1.from_string("127.0.0.1");
	boost::asio::ip::tcp::endpoint ep(addr, 6688);
	assert(ep.address() == addr);
	assert(ep.port() == 6688);
}

#include <boost/lexical_cast.hpp>

void resolv_connect(TCP::socket &sock, const char* name, int port)
{
	TCP::resolver rlv(sock.get_io_service());
	TCP::resolver::query qry(name, boost::lexical_cast<std::string>(port));

	TCP::resolver::iterator iter = rlv.resolve(qry);
	TCP::resolver::iterator end;
	boost::system::error_code ec = boost::asio::error::host_not_found;

	for (; ec && iter != end; ++iter)
	{
		sock.close();
		sock.connect(*iter, ec);
	}

	if (ec)
	{
		std::cout << "cant't connect" << std::endl;
		throw boost::system::system_error(ec);
	}
	std::cout << "connect success." << std::endl;
}

void resolver_usage()
try
{
	boost::asio::io_service ios;
	TCP::socket sock(ios);
	resolv_connect(sock, "www.boost.org", 80);

	ios.run();
}
catch (std::exception& e)
{
	std::cout << e.what() << std::endl;
}

void time_exipred(boost::system::error_code ec, asio::serial_port *sp)
{
	std::cout << "time exiored" << std::endl;
	sp->close();
}

void read_handler(boost::system::error_code ec)
{
	std::cout << ec.message() << std::endl;
}

#define SP asio::serial_port
void serial_port_usage()
{
	asio::io_service ios;
	SP sp(ios, "COM1");

	sp.set_option(SP::baud_rate(9600));
	sp.set_option(SP::flow_control(SP::flow_control::none));
	sp.set_option(SP::parity(SP::parity::none));
	sp.set_option(SP::stop_bits(SP::stop_bits::one));
	sp.set_option(SP::character_size(8));

	size_t len = sp.write_some(asio::buffer("hello serial"));
	std::cout << len << std::endl;

	std::vector<char> v(100);
	sp.async_read_some(asio::buffer(v),boost::bind(read_handler,asio::placeholders::error));

	asio::deadline_timer t(ios,boost::posix_time::seconds(2));
	t.async_wait(boost::bind(time_exipred,asio::placeholders::error,&sp));

	//t.async_wait(boost::bind(&SP::cancel,boost::ref(sp)));
	//t.async_wait(boost::bind(&SP::close, boost::ref(sp)));
	ios.run();
}

void asio_test()
{
#if 0
	asio_usage();

	io_service ios;

	a_timer at1(ios, 5, print1);
	a_timer at2(ios, 5, print2);
	ios.run();

	ip_usage();

	resolver_usage();
#endif

	serial_port_usage();
}

#endif // LEE_ASIO
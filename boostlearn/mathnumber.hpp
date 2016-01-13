#ifndef LEE_MATHNUMBER
#define LEE_MATHNUMBER

#include <iostream>
#include <boost/assign.hpp>

using namespace std;
using namespace boost;
using namespace boost::assign;

#include <boost/cstdint.hpp>
#include <limits>

void integer_usage()
{
	uint8_t u8;
	int_fast16_t i16;
	int_least32_t i32;
	uintmax_t um;

	u8  = 255;
	i16 = 32000;
	i32 = i16;
	um  = u8 + i16 + i32;

	cout << typeid(u8).name() << ":" << sizeof(u8) << "v = " << (short)u8 << endl;
	cout << typeid(i16).name() << ":" << sizeof(i16) << "v = " << i16 << endl;
	cout << typeid(i32).name() << ":" << sizeof(i32) << "v = " << i32 << endl;
	cout << typeid(um).name() << ":" << sizeof(um) << "v = " << um << endl;

	cout << (short)numeric_limits<int8_t>::max() << endl;
	cout << numeric_limits<uint_least16_t>::max() << endl;
	cout << numeric_limits<int_least32_t>::max() << endl;
	cout << numeric_limits<uintmax_t>::max() << endl;
}

#include <boost/integer.hpp>

void integer_usage1()
{
	int_fast_t<char>::fast a;
	cout << typeid(a).name() << endl;

	int_fast_t<int>::fast b;
	cout << typeid(b).name() << endl;

	int_fast_t<uint16_t>::fast c;
	cout << typeid(c).name() << endl;
}

#include <boost/format.hpp>

void inter_usage2()
{
	format fmt("type:%s,size = %dbit\n");

	uint_t<15>::fast a;
	cout << fmt % typeid(a).name() % (sizeof(a) * 8);

	int_max_value_t<32700>::fast b;
	cout << fmt % typeid(b).name() % (sizeof(b) * 8);

	int_min_value_t<-33000>::fast c;
	cout << fmt % typeid(c).name() % (sizeof(c) * 8);

	uint_value_t<33000>::fast d;
	cout << fmt % typeid(d).name() % (sizeof(d) * 8);

}

#include <boost/rational.hpp>

void rational_usage()
{
	rational<int> a;
	rational<int> b(20);
	rational<int> c(31415, 10000);

	rational<int> r;
	r = 0x31;
	r.assign(7, 8);
}

void rational_operate()
{
	rational<int> a(3), b(65534), c(22, 7);
	b += a;
	c -= a;
	if (c >= 0)
	{
		c = c * b;
		++a;
	}
	assert(a == 4);
	
	rational<int> d(10);
	if (d)
	{
		d -= 10;
	}
	assert(!d);

	rational<int> e(2718, 1000);
	cout << rational_cast<int>(e) << endl;
	cout << rational_cast<double>(e) << endl;

	//rational<int> rrr;
	//cin >> rrr;
	//cout << ++rrr;

	rational<int> f(22, 7);
	cout << f.numerator() << f.denominator() << "=" << rational_cast<double>(f);

	rational<int> g(1444, 1000), pi(314,100);
	cout << abs(g) << endl;
	cout << pow(rational_cast<double>(a), 2) << endl;
	cout << cos(rational_cast<double>(pi)) << endl;

	rational<int> h(22, 7), i;
	try
	{
		h / i;
	}
	catch (bad_rational& e)
	{
		cout << typeid(e).name() << e.what() << endl;
	}

	int j = 37, k = 62;
	format fmt("gcd(%1%,%2%) = %3%.lcm(%1%,%2%) = %4%\n");
	cout << fmt % j % k % gcd(j, k) % lcm(j, k);
}

#include <boost/crc.hpp>

void crc_usage()
{
	crc_32_type crc32;

	cout << hex;
	cout << crc32.checksum() << endl;
	crc32.process_byte('a');
	cout << crc32.checksum() << endl;
	crc32.process_bytes("1234567890",10);
	cout << crc32() << endl;

	char szCh[] = "1234567890";
	crc32.reset();
	crc32.process_block(szCh, szCh + 10);
	cout << crc32.checksum() << endl;

	string str = "1234567890";
	crc32.reset();
	cout << std::for_each(str.begin(), str.end(), crc32)() << endl;
	cout << crc32() << endl;

	crc<16, 0x8005, 0, 0, true, true>("1234567890",10);
}

#include <boost/random.hpp>
#include <ctime>
#include <boost/serialization/singleton.hpp> 

using boost::serialization::singleton;

template<typename T>
class rng_wraper
{
private:
	T rng;
public:
	rng_wraper() :rng((typename T::result_type)time(0)){}
	typename T::result_type operator()()
	{
		return rng();
	}
};

void random_usage()
{
	mt19937 rng(time(0));

	for (int i = 0; i < 10;++i)
	{
		cout << rng()%6 << "," << endl;
	}

// 	typedef singleton<rng_wraper<mt19937>> rng_t;
// 	auto rngg = rng_t::get_const_instance();
// 
// 	for (int i = 0; i < 10; ++i)
// 	{
// 		cout << rngg() << ","<< endl;
// 	}
// 
// 	rand48 fng;
// 	for (int i = 0; i < 10;++i)
// 	{
// 		cout << fng()*1.0 / numeric_limits<rand48::result_type>::max()<<endl;
// 	}
}

void random_distribution()
{
	mt19937 rng(time(0));

	uniform_int<> ui(0, 255);
	for (int i = 0; i < 10;++i)
	{
		cout << ui(rng) << ",";
	}
	cout << endl;

	uniform_01<mt19937&> u01(rng);
	for (int i = 0; i < 10; ++i)
	{
		cout << u01() << ",";
	}
	cout << endl;

	normal_distribution<> nd(1, 2);
	int count = 0;
	for (int i = 0; i < 10;++i)
	{
		if (abs(nd(u01) - 1) <= 2.0)
		{
			++count;
		}
		cout << 1.0*count / 10000 << endl;
	}

	//mt19937 mtrng((int32_t)std::time(0));
	//uniform_smallint<> us(1, 100);

	//variate_generator<mt19937&, uniform_smallint<> > gen(mtrng, us);
	variate_generator<mt19937&, uniform_smallint<> > gen(mt19937((int32_t)std::time(0)), uniform_smallint<>(1, 100));
	for (int i = 0; i < 10; ++i)
	{
		cout << gen() << endl;
	}
}

template<typename Rng>
void rand_bytes(unsigned char *buf, int buf_len)
{
	typedef variate_generator<Rng, uniform_smallint<> > var_gen;
	static var_gen gen(Rng((typename Rng::result_type)std::time(0)), uniform_smallint<>(1, 255));

	for (int i = 0; i < buf_len;++i)
	{
		buf[i] = gen();
		cout << (short)buf[i]<<endl;
	}
}

void random_datablock()
{
	unsigned char *buf = new unsigned char[12];
	rand_bytes<mt19937>(buf, 12);

	mt19937 rng(time(0));
	generate_n(buf, 12, std::tr1::ref(rng));

	for (int i = 0; i < 12;++i)
	{
		cout << (short)buf[i]<<endl;
	}
}

//real random impl

void mathnumber_test()
{
	//integer_usage();
	//integer_usage1();
	//inter_usage2();
	//rational_operate();
	//crc_usage();
	random_usage();
	//random_distribution();
	//random_datablock();
}

#endif //LEE_MATHNUMBER
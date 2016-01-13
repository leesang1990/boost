#ifndef LEE_TOOL
#define LEE_TOOL

#include <iostream>

using namespace std;
using namespace boost;

#include <boost/utility.hpp>

class do_not_copy :boost::noncopyable{};
void noncopyable_usage()
{
	do_not_copy d1;
// 	do_not_copy d2(d1);
// 	do_not_copy d3;
// 	d3 = d1;
}

vector<string> func()
{
	vector<string> v(10);
	return v;
}

// #include <boost/typeof/typeof.hpp>
// #include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()
namespace ex
{
	struct demo_class2{ int a, b; };
}
//BOOST_TYPEOF_REGISTER_TYPE(ex::demo_class2)

void auto_usage()
{
	auto x = 2.0 * 3;
	auto y = 2 + 3;

	auto a = new double[20];
	auto p = make_pair(1, "string");
	auto v = func();

	auto k = make_pair("test", ex::demo_class2());
}

#include <boost/optional.hpp>
#include <boost/utility/in_place_factory.hpp>

optional<double> calc(int x)
{
	return optional<double>(x != 0, 1.0 / x);
}

optional<double> sqrt_op(double x)
{
	return optional<double>(x > 0,sqrt(x));
}

void optional_usage()
{
	optional<int> op0;
	optional<int> op1(none);
	assert(op0 == op1);
	assert(op1.get_value_or(253) == 253);

	optional<string> ops("test");
	cout << *ops << endl;

	vector<int> v(10);
	optional<vector<int>& > opv(v);
	assert(opv);

	opv->push_back(5);
	assert(opv->size() == 11);

	opv = none;
	assert(!opv);

	optional<double> d = calc(10);
	if (d)
	{
		cout << *d << endl;
	}
	d = sqrt_op(-10);
	if (!d)
	{
		cout << "no result" << endl;
	}

	auto x = make_optional(5);
	assert(*x == 5);

	auto y = make_optional<double>((*x > 10), 1.0);
	assert(!y);

	optional<string> opss(in_place("test in_place_factory"));
	cout << *opss;

	optional<vector<int> > opp(in_place(10, 3));
	assert(opp->size() == 10);
	assert((*opp)[0] == 3);
}

#include <boost/assign.hpp>
using namespace boost::assign;

void assign_usage()
{
	vector<int> v;
	v += 1, 2, 3, 4, 5, 6 * 6;

	set<string> s;
	s += "cpp","java","c#","python";

	map<int,string> m;
	m += make_pair(1, "one"), make_pair(2, "two");

	vector<int> v1;
	push_back(v1)(1)(2)(3)(4)(5);

	list<string> l;
	push_front(l)("cpp")("java")("c#")("python");

	set<double> s1;
	insert(s1)(3.14)(0.618)(1.732);

	map<int, string> m1;
	insert(m1)(1, "one")(2, "two");
}

void assign_list_of_usage()
{
	vector<int> v = list_of(1)(2)(3)(4)(5);
	deque<string> d = (list_of("power")("bomb"), "phazon", "suit");
	set<int> s = (list_of(10),20,30,40,50);
	map<int, string> m = list_of(make_pair(1, "one"))(make_pair(2, "two"));

	map<int, int> m1 = map_list_of(1, 2)(3, 4)(5, 6);

	vector<int> v2 = list_of(1).repeat(3, 2)(3)(4)(5);

	multiset<int> ms;
	insert(ms).repeat_fun(5, &rand).repeat(2, 1), 10;

	deque<int> d1;
	push_front(d1).range(v2.begin(), v2.begin() + 5);

}

//standard container vector,string,deque,list,set,multiset,map,multimap
//container adapter stack,queue,priority_queue

#include <stack>
#include <queue>
//unstandard container
#include <hash_map>
//#include <slist>

void assign_unstandard_adapter()
{
	stack<int> stk = (list_of(1), 2, 3).to_adapter();
	while (!stk.empty())
	{
		cout << stk.top() << " ";
		stk.pop();
	}
	cout<<endl;

	queue<string> q = (list_of("china")("us")("uk")).repeat(2,"russia").to_adapter();
	while (!q.empty())
	{
		cout << q.front() << " ";
		q.pop();
	}
	cout << endl;

	priority_queue<double> pq = (list_of(1.414), 1.732, 2.236).to_adapter();
	while (!pq.empty())
	{
		cout << pq.top() << " ";
		pq.pop();
	}

	//slist<int> sl;
	//push_front(sl),1,2,3,4,5;

	hash_map<string, int> hm = map_list_of("one", 1)("two", 2);

	vector<vector<int > > vv = list_of(list_of(1)(2))(list_of(3)(4));
	vv += list_of(5)(6), list_of(7)(8);

	int a = 1,b = 2,c = 3;
	vector<int> v = ref_list_of<3>(a)(b)(c);
	assert(v.size() == 3);
}

namespace tool
{
	class point
	{
		int x, y, z;
	public:
		explicit point(int a = 0, int b = 0, int c = 0) :x(a), y(b), z(c){}
		void print() const
		{
			cout << x << "," << y << "," << z << endl;
		}
		void swap(point &p)
		{
			std::swap(x, p.x);
			std::swap(y, p.y);
			std::swap(z, p.z);
			cout << "inner swap" << endl;
		}
	};
}

//namespace std
//{
//	template<>
	void swap(tool::point &x, tool::point &y)
	{
		x.swap(y);
	}
//}

#include <boost/swap.hpp>
void swap_usage()
{
	int a1[10];
	int a2[10];

	std::fill_n(a1, 10, 5);
	std::fill_n(a2, 10, 20);

	boost::swap(a1,a2);

	tool::point a(1, 2, 3), b(4, 5, 6);
	cout << "std::swap" << endl;
	std::swap(a, b);
	cout << "boost::swap" << endl;
	boost::swap(a, b);
}

#include <boost/logic/tribool.hpp>
namespace tmp_ns
{
	BOOST_TRIBOOL_THIRD_STATE(unknown)
}


void tribool_usage()
{
	tribool tb(true);
	tribool tb2(!tb);

	if (tb)
	{
		cout << "true" << endl;
	}

	tb2 = indeterminate;
	assert(indeterminate(tb2));
	cout << tb2 << endl;
	if (tb2 == indeterminate)
	{
		cout << "indeterminate" << endl;
	}
	cout<<(tb2||true)<<endl;
	cout<<(tb2&&true)<<endl;

	

	tribool tb3(tmp_ns::unknown);
	assert(tmp_ns::unknown(tb3));
	assert(tmp_ns::unknown(tb3 || false));
}

#include <utility>

namespace tool
{
	class demo_class
	{
	public:
		demo_class(int n) :x(n){}
		int x;
		friend bool operator<(const demo_class& l, const demo_class& r)
		{
			return l.x < r.x;
		}
	};
};

void operator_usage()
{
	tool::demo_class a(10), b(20);
	using namespace std::rel_ops;

	cout << (a < b) << endl;
	cout << (b >= a) << endl;
}

#include <boost/operators.hpp>
namespace tool
{
	//class point1:boost::less_than_comparable<point1>,
	//boost::equality_comparable<point1>
	class point1:boost::totally_ordered<point1,additive<point1>>
	{
		int x, y, z;
	public:
		explicit point1(int a = 0, int b = 0, int c = 0) : x(a), y(b), z(c){}
		void print()const
		{
			cout << x << ", " << y << "," << z << endl;
		}

		friend bool operator<(const point1& l, const point1& r)
		{
			return (l.x*l.x + l.y*l.y + l.z*l.z < r.x*r.x + r.y*r.y + r.z * r.z);
		}

		friend bool operator==(const point1& l, const point1& r)
		{
			return r.x == l.x && r.y == l.y && r.z == l.z;
		}

		point1& operator+=(const point1& r)
		{
			x += r.x;
			y += r.y;
			z += r.z;
			return *this;
		}

		point1& operator-=(const point1& r)
		{
			x -= r.x;
			y -= r.y;
			z -= r.z;
			return *this;
		}
	};
};

void boost_ops_usage()
{
	tool::point1 p0, p1(1, 2, 3), p2(5, 6, 7), p3(3, 2, 1);

	assert( p0 < p1 && p1 < p2);
	assert(p2 > p0);
	assert(p1 <= p3);
	assert(!(p1 < p3)&&!(p1 > p3));

	tool::point1 p4(p1);
	assert(p1 == p4);
	assert(p1 != p3);

	using namespace boost::assign;
	vector<tool::point1> v = (list_of(p0),p1,p2,p3);

	auto pos = std::find(v.begin(),v.end(),tool::point1(1,2,3));
	pos->print();

	(p1 + p2).print();
	(p3 - p1).print();

	assert((p2 - p2) == p0);
}

template<typename T>
class my_smart_ptr :public boost::dereferenceable < my_smart_ptr<T>, T* >
{
	T *p;
public:
	my_smart_ptr(T *x) :p(x){}
	~my_smart_ptr(){ delete p;}
	T& operator*()const
	{	return *p;	}
};

template<typename T>
class my_smart_array :public boost::indexable < my_smart_array<T>, int, T& >
{
	T* p;
public:
	typedef my_smart_array<T> this_type;
	typedef T* iter_type;
	my_smart_array(T *x) :p(x){}
	~my_smart_array(){ delete[] p; }
	friend iter_type operator+(const this_type& a, int n)
	{
		return a.p + n;
	}
};

void dereferenceable_indexable_usage()
{
	my_smart_ptr<string> p(new string("123"));
	assert(p->size() == 3);

	my_smart_array<double> ma(new double[10]);
	ma[0] = 1.0;
	*(ma + 1) = 2.0;
	cout << ma[1] << endl;
}

#include <boost/exception/all.hpp>

struct  my_exception:virtual std::exception,virtual boost::exception
{

};

typedef boost::error_info<struct tag_err_no, int> err_no;
typedef boost::error_info<struct tag_err_str, string> err_str;

void exception_usage()
{
	try
	{
		try
		{
			throw my_exception() << err_no(10);
		}
		catch (my_exception& e)
		{
			//获得异常内存储的信息
			cout << *get_error_info<err_no>(e) << endl;
			cout << e.what() << endl;
			e << err_str("other info"); //向异常追加信息
			throw;						//再次抛出异常
		}
	}
	catch (my_exception& e)
	{
		cout << *get_error_info<err_str>(e) << endl;
	}

}

void exception_usage1()
{
	//errinfo_api_function,errinfo_at_line,errinfo_errno,error_file_handle,\
	  errinfo_file_name,errinfo_file_open_mode,errinfo_type_info_name

	try
	{
		throw my_exception() << errinfo_api_function(__FUNCTION__)
			<< errinfo_errno(101);
	}
	catch (boost::exception& e)
	{
		cout << *get_error_info<errinfo_api_function>(e);
		cout << *get_error_info<errinfo_errno>(e);
	}
}

struct my_err{};
void exception_usage2()
{
	try
	{
		throw enable_error_info(my_err()) << errinfo_errno(10);
	}
	catch (boost::exception& e)
	{
		cout << *get_error_info<errinfo_errno>(e) << endl;
	}

	try
	{
		throw enable_error_info(std::runtime_error("run time")) 
			<< errinfo_at_line(__LINE__);
	}
	catch (boost::exception& e)
	{
		cout << *get_error_info<errinfo_at_line>(e) << endl;
	}
}

void exception_usage3()
{
	try
	{
		throw enable_error_info(my_err())
			<< errinfo_errno(101)
			<< errinfo_api_function("fopen");
	}
	catch (boost::exception& e)
	{
		cout << diagnostic_information(e) << endl;
	}

	try
	{
		BOOST_THROW_EXCEPTION(std::logic_error("logic"));
	}
	catch (boost::exception& e)
	{
		cout << diagnostic_information(e) << endl;
	}

	typedef boost::tuple<errinfo_api_function, errinfo_errno> err_group;
	try
	{
		throw enable_error_info(std::out_of_range("out"))
			<< err_group("syslogd", 874);
	}
	catch (boost::exception& e)
	{
		cout << current_exception_diagnostic_information() << endl;
	}
}

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace boost::uuids;

void uuid_usage()
{
	uuid u;
	assert(uuid::static_size() == 16);
	assert(u.size() == 16);

	vector<unsigned char> v(16,7);
	std::copy(v.begin(),v.end(),u.begin());
	assert(u.data[0] == u.data[1] && u.data[15] == 7);

	cout << u << endl;
	std::fill_n(u.data + 10,6,8);
	cout << u << endl;

	uuid ui;
	std::fill_n(ui.begin(), ui.size(), 0xab);
	assert(!ui.is_nil());
	assert(ui.variant() == ui.variant_rfc_4122);
	assert(ui.version() == ui.version_unknown);
	cout << ui << endl;

	std::memset(ui.data,0,16);
	assert(ui.is_nil());

	uuid u1, u2;
	std::fill_n(u1.begin(),u1.size(),0xab);
	std::fill_n(u2.begin(), u2.size(), 0x10);
	assert(u1 != u2 && u1 > u2);

	u2.data[0] = 0xff;
	assert(u1 < u2);

	std::memset(u1.data, 0, 16);
	std::memset(u2.data, 0, 16);
	assert(u1 == u2);

}

class uuid_t :public uuid
{
private:
	static random_generator rgen;
	static string_generator sgen;
public:
	uuid_t():uuid(rgen()){}
	uuid_t(int):uuid(nil_uuid()){}
	uuid_t(const char* str) :uuid(sgen(str)){}
	uuid_t(const uuid& u, const char* str) :
		uuid(name_generator(u)(str)){}
	explicit uuid_t(const uuid& u) :uuid(u){}
	operator uuid()
	{
		return static_cast<uuid&>(*this);
	}
	operator uuid()const
	{
		return static_cast<const uuid&>(*this);
	}
};

random_generator uuid_t::rgen;
string_generator uuid_t::sgen;

#include <boost/lexical_cast.hpp>
void uuid_generator_usage()
{
	//nil_generator
	uuid u = nil_generator()();
	assert(u.is_nil());

	u = nil_uuid();
	assert(u.is_nil());

	//string_generator
	string_generator sgen;
	uuid u1 = sgen("0123456789abcdef0123456789abcdef");
	cout << u1 << endl;
	uuid u2 = sgen("01234567-89ab-cdef-0123-456789abcdef");
	cout << u2 << endl;
	uuid u3 = sgen(L"{01234567-89ab-cdef-0123-456789abcdef}");
	cout << u3 << endl;

	//name_generator 
	uuid www_xxx_com = string_generator()("{0123456789abcdef0123456789abcdef}");
	name_generator ngen(www_xxx_com);

	uuid u4 = ngen("mario");
	assert(!u4.is_nil() && u4.version() == uuid::version_name_based_sha1);
	uuid u5 = ngen("link");
	cout << u5 << endl;

	//basic_random_generator
	random_generator rgen;

	uuid u6 = rgen();
	assert(u.version() != uuid::version_random_number_based);
	cout << u6 << endl;

	uuid_t ut1, ut2;
	cout << ut1 << endl;
	cout << ut2 << endl;

	uuid_t ut3("{01234567-89ab-cdef-0123-456789abcdef}");
	cout << ut3 << endl;

	//uuid to string
	uuid u7 = string_generator()("01234567-89ab-cdef-0123-456789abcdef");
	stringstream ss;
	ss << u7;
	string str;
	ss >> str;
	cout << str << endl;

	string str1 = lexical_cast<string>(u7);
	cout << str1 << endl;
}

#include <boost/uuid/sha1.hpp>
using namespace boost::uuids::detail;

void sha1_usage()
{
	sha1 sha;

	char *szMsg = "a short message";
	sha.process_byte(0x10);
	sha.process_bytes(szMsg, strlen(szMsg));
	sha.process_block(szMsg, szMsg + strlen(szMsg));

	unsigned int digest[5];
	sha.get_digest(digest);
	for (int i = 0; i < 5; ++i)
	{
		cout << hex << digest[i];
	}
}

#include <boost/config/suffix.hpp>
void config_usage()
{
	cout << BOOST_STRINGIZE(__LINE__) << endl;
	cout << std::boolalpha << (string("22") == BOOST_STRINGIZE(__LINE__)) << endl;
	int x = 255;
	cout << BOOST_STRINGIZE(x) << endl;
}

struct static_int
{	
	//BOOST_STATIC_CONSTANT(int,v1 = 10)
	//BOOST_STATIC_CONSTANT(int,v2 = 20)
	static const int v1 = 10;
	enum{ v2 = 20};
	int a[v2];
};

void static_constant_usage()
{
	cout << static_int::v1 << endl;
	cout << static_int::v2 << endl;
}

#include <boost/config/warning_disable.hpp> //diaable warning C4996

#include <boost/utility/binary.hpp> // or #include <boost/utility.hpp>

void utility_binary_usage()
{
	cout << hex << showbase;
	cout << BOOST_BINARY(0110) << endl;
	cout << BOOST_BINARY(0110 1101) << endl;
	cout << BOOST_BINARY(10110110 01) << endl;
	cout << bitset<5>( BOOST_BINARY(0110)) << endl;

	cout << BOOST_BINARY_UL(101 1001) << endl;
	long long x = BOOST_BINARY_LL(1101);
	cout << x << endl;

	cout << BOOST_CURRENT_FUNCTION <<endl;
}

void tool_test()
{
#if 0
	noncopyable_usage();
	optional_usage();
	assign_usage();
	assign_list_of_usage();
	assign_unstandard_adapter();
	swap_usage();
	tribool_usage();
	operator_usage();
	boost_ops_usage();
	dereferenceable_indexable_usage();
	exception_usage();
	exception_usage1();
	exception_usage2();
	exception_usage3();
	uuid_usage();
	uuid_generator_usage();
	config_usage();
	static_constant_usage();
#endif							
	utility_binary_usage();
}

#endif //LEE_TOOL
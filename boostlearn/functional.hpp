#ifndef LEE_FUNCTIONAL
#define LEE_FUNCTIONAL

#include <iostream>
#include <boost/assign.hpp>
#include <boost/utility/result_of.hpp>

using namespace std;
using namespace boost;
using namespace boost::assign;

template<typename T,typename T1>
typename boost::result_of<T(T1)>::type call_func(T t, T1 t1)
{
	return t(t1);
}

void result_of_usage()
{
	typedef double(*Func)(double d);
	Func func = sqrt;
	boost::result_of<Func(double)>::type x = func(5.0);
	cout << typeid(x).name();

	auto y = call_func(func, 5.0);
	cout << typeid(y).name();
}


#include <algorithm>
#include <set>


struct square
{
	typedef void result_type;
	void operator()(int &x)
	{
		x = x * x;
	}
};

void ref_usage()
{


	vector<int> v = (list_of(1),2,3,4,5);
	for_each(v.begin(), v.end(), square());

	int x = 10;
	boost::reference_wrapper<int> rw(x);
	assert(x == rw);

	(int &)rw = 100;
	assert(x == 100);

	boost::reference_wrapper<int> rw2(rw);
	assert(rw2.get() == 100);

	string str;
	boost::reference_wrapper<string> rws(str);
	*rws.get_pointer() = "test reference_wrapper";
	cout << rws.get().size() << endl;

	double d = 2.71818;
	auto rwd = boost::cref(d);
	cout << typeid(rwd).name() << endl;

	string str1;
	auto rws1 = std::ref(str1);
	cout << typeid(rws1).name() << endl;

	cout << std::sqrt(std::ref(d)) << endl;

	set<int> s;
	auto rwset = boost::ref(s);
	boost::unwrap_ref(rwset).insert(12);

	string str2("text");
	auto rws2 = boost::cref(str2);
	cout << boost::unwrap_ref(rws2) << endl;
}

class big_class
{
private:
	int x;
public:
	big_class() :x(0){}
	void print()
	{
		cout << "big class" << ++x << endl;
	}

};

template<typename T>
void printrw(T a)
{
	for (int i = 0; i < 2;++i)
	{
		boost::unwrap_ref(a).print();
	}
}

void ref_usage1()
{
	big_class c;
	auto rwc = boost::ref(c);
	c.print();

	printrw(c);
	printrw(rwc);
	printrw(c);
	c.print();

	typedef double(*pfunc)(double);
	pfunc pf = sqrt;
	cout << boost::ref(pf)(5.0);

	square sq;
	int x = 5;
	//boost::ref(sq)(x);
}

#include <boost/bind.hpp>

int f(int a, int b)
{
	return a + b;
}

int g(int a, int b, int c)
{
	return a + b *c;
}

typedef int (*f_type)(int, int);
typedef int (*g_type)(int, int,int);

struct demo
{
	int f(int a, int b)
	{
		return a + b;
	}
};

struct point
{
	int x, y;
	point(int a = 0, int b = 2) :x(a), y(b){}
	void print()
	{
		cout << "(" << x << "," << y << ")\n";
	}
};

#include <vector>
#include <functional>
#include <algorithm>

void bind_usage()
{
	cout << bind(f, 1, 2)() << endl;
	cout << bind(g, 1, 2,3)() << endl;

	cout << bind(f, _1, 9)(4)<<endl;
	cout << bind(f, _1, _2)(4, 9) << endl;
	cout << bind(f, _2, _1)(4, 9) << endl;
	cout << bind(f, _1, _1)(4, 4) << endl;
	cout << bind(g, _1, 8, _2)(4, 9) << endl;
	cout << bind(g, _3, _2, _2)(1, 2, 3) << endl;

	f_type pf = f;
	g_type pg = g;
	int x = 1, y = 2, z = 3;
	cout << bind(pf, _1, 9)(x) << endl;
	cout << bind(pg, _3, _2, _2)(x, y, z) << endl;

	demo a, &ra = a;
	demo *p = &a;
	
	cout << bind(&demo::f, a, _1, 20)(10) << endl;
	cout << bind(&demo::f, ra, _2, _1)(10, 20) << endl;
	cout << bind(&demo::f, p, _1, _2)(10, 20) << endl;

	vector<point> v(10);
	vector<int> v2(10);

	for_each(v.begin(), v.end(), bind(&point::print,_1));
	transform(v.begin(), v.end(), v2.begin(), bind(&point::x, _1));

	BOOST_FOREACH(int x, v2)
		cout << x << ",";
	cout << endl;

	typedef pair<int, string> pair_t;
	pair_t pp(123, "string");

	cout << bind(&pair_t::first, pp)() << endl;
	cout << bind(&pair_t::second, pp)() << endl;

	int numbers[] = { 20, 40, 50, 10, 30 };
	std::sort(numbers, numbers + 5, std::greater<int>());
	for (int i = 0; i < 5; i++)
		std::cout << numbers[i] << ' ';
	std::cout << '\n';

	auto ret = bind(std::greater<int>(), _1, 10);
	cout << ret(9) << endl;
	auto ret2 = bind(plus<int>(), _1, _2)(1,2);
	auto ret3 = bind(modulus<int>(), _1, 3)(4);

	int xx = 10;
	cout << bind(g, _1, boost::cref(x), boost::ref(x))(10) << endl;
	f_type af = f;

	cout << bind<int>(boost::cref(af), _1, _2)(10, 20) << endl;

	auto r = std::ref(xx);
	{
		//int *y = new int(20);
		std::shared_ptr<int> y = make_shared<int>(20);
		r = std::ref(*y);
		cout << r << endl;
		cout << bind(g, r, 1, 1)() << endl;
		//delete y;

	}
	cout << bind(g, r, 1, 1)() << endl;

	//boost::arg<1> &_x = _1;
	//boost::arg<2> &_y = _2;
	boost::arg<1> &arg1 = _1;
	boost::arg<2> &arg2 = _2;

	BOOST_AUTO(&_x, _1);
	BOOST_AUTO(&_y, _2);

	assert(typeid(_x) == typeid(arg1));
	cout << bind(f, _x, _y)(10, 20)<<endl;
	cout << bind(g, arg1, 2, arg2)(1, 2) << endl;

	typedef rational<int> ri;

	vector<ri> vv = list_of(ri(1,2))(ri(3,4))(ri(5,6));

	remove_if(vv.begin(), vv.end(), bind(&ri::numerator, _1) == 1);
	assert(vv[0].numerator() == 3);

	assert( find_if(vv.begin(), vv.end(), bind(&ri::numerator, _1) == 1) == vv.end() );

	auto pos = find_if(vv.begin(), vv.end(), bind(&ri::numerator, _1) > 3 && bind(&ri::denominator, _1) < 8);
	cout << *pos << endl;
}

#include <boost/function.hpp>

struct demo_classs
{
	int add(int a, int b)
	{
		return a + b;
	}
	int operator()(int x) const
	{
		return x*x;
	}
};

template<typename T>
struct summary
{
	typedef void result_type;
	T sum;
	summary(T v = T()) :sum(v){}
	void operator()(T const &x)
	{
		sum += x;
	}
};

void function_usage()
{
	boost::function<int(int, int)> func;
	assert(!func);

	func = f;
	if (func)
	{
		cout << func(10, 20) << endl;
	}
	func = 0;
	assert(func.empty());

	boost::function<int(demo_classs&, int, int)> func1;
	func1 = bind(&demo_classs::add, _1, _2, _3);
	demo_classs sc;
	cout << func1(sc, 10, 20) << endl;

	boost::function<int(int, int)> func2;
	func2 = bind(&demo_classs::add,&sc,_1,_2);
	cout << func2(10, 20) << endl;

	boost::function<int(int)> func3;
	func3 = boost::cref(sc);
	cout << func3(10)<<endl;

	vector<int> v = (list_of(1), 3, 5, 7, 9);
	summary<int> s;
	boost::function<void(int const&)> func4(boost::ref(s));
	std::for_each(v.begin(), v.end(), func4);
	cout << s.sum << endl;
}

class demo_class_cb
{
private:
	typedef boost::function<void(int)> func_t;
	func_t func;
	int n;
public:
	demo_class_cb(int i) :n(i){}

	template<typename CallBack>
	void accept(CallBack f)
	{
		func = f;
	}

	void run()
	{
		func(n);
	}
};

void call_back_func(int i)
{
	cout << "call_back_func:";
	cout << i * 2 << endl;
}

class call_back_obj
{
private:
	int x;
public:
	call_back_obj(int i) :x(i){}
	void operator()(int i)
	{
		cout << "call_back_obj:";
		cout << i*x++ << endl;
	}
};

class call_back_factory
{
public:
	void call_back_func1(int i)
	{
		cout << "call_back_func1:";
		cout << i * 2 << endl;
	}

	void call_back_func2(int i,int j)
	{
		cout << "call_back_func2:";
		cout << i * j * 2 << endl;
	}
};

void function_callback()
{
	demo_class_cb dc(10);
	dc.accept(call_back_func);
	dc.run();

	demo_class_cb dcc(20);
	call_back_obj cbo(2);
	dcc.accept(boost::ref(cbo));
	dcc.run();
	dcc.run();

	demo_class_cb dccc(10);
	call_back_factory cbf;
	dccc.accept(bind(&call_back_factory::call_back_func1, cbf, _1));
	dccc.run();

	dccc.accept(bind(&call_back_factory::call_back_func2, cbf, _1,5));
	dccc.run();
}

#include <boost/signals2.hpp>
using namespace boost::signals2;

void slots1()
{cout << "slot1 called" << endl;}

void slots2()
{cout << "slot2 called" << endl;}
 
template<int N>
struct slots
{
	void operator()()
	{
		cout << "slots" << N << "called" << endl;
	}

};

template<int N>
struct slots_ex
{
	int operator()(int x)
	{
		cout << "slot" << N << " called" << endl;
		return x*N;
	}

// 	template<int N>
// 	bool operator==(const slots_ex<N>&, const slots_ex<N>&)
// 	{
// 		return true;
// 	}

};

template<typename T>
class combiner
{
	T v;
public:
	typedef std::pair<T, T> result_type;
	combiner(T t = T()) :v(t){}
	template<typename InputIterator>
	result_type operator()(InputIterator begin, InputIterator end) const
	{
		if (begin == end)
		{
			return result_type();
		}
		vector<T> vec(begin, end);
		T sum = std::accumulate(vec.begin(), vec.end(), v);
		T max = *std::max_element(vec.begin(), vec.end());
		return result_type(sum, max);
	}

};

void signals2_usage()
{
	signal<void()> sig,sig2;

	sig.connect(&slots1);
	sig.connect(&slots2,at_front);
	sig();

	sig2.connect(slots<1>(), at_back);
	sig2.connect(slots<100>(), at_front);
	sig2.connect(5, slots<51>(), at_back);
	sig2.connect(5, slots<55>(), at_front);
	sig2.connect(3, slots<30>(), at_front);
	sig2.connect(3, slots<33>(), at_back);
	sig2.connect(10, slots<10>());
	sig2();

	signal<int(int)> sig_ex;

	sig_ex.connect(slots_ex<10>());
	sig_ex.connect(slots_ex<20>());
	sig_ex.connect(slots_ex<50>());

	cout << *sig_ex(2);

	signal<int(int), combiner<int> > sig_com(combiner<int>(100));

	sig_com.connect(slots_ex<10>());
	sig_com.connect(slots_ex<20>());
	sig_com.connect(slots_ex<30>(),at_front);

	auto ret = sig_com(2);
	cout << ret.first << "," << ret.second<<endl;

// 	signal<int(int)> sig_ex2;
// 	assert(sig_ex2.empty());
// 
// 	sig_ex2.connect(0, slots_ex<10>());
// 	sig_ex2.connect(0, slots_ex<20>());
// 	sig_ex2.connect(1, slots_ex<30>());
// 	sig_ex2.disconnect(0);
// 	assert(sig_ex2.num_slots() == 1);
// 	sig_ex2.disconnect(slots_ex<30>());
// 	assert(sig_ex2.empty());

	signal<int(int)> sig_ex3;

	connection c1 = sig_ex3.connect(0, slots_ex<10>());
	connection c2 = sig_ex3.connect(0, slots_ex<20>());
	connection c3 = sig_ex3.connect(1, slots_ex<30>());

	c1.disconnect();
	assert(sig_ex3.num_slots() == 2);
	assert(!c1.connected());
	assert(c2.connected());

	signal<int(int)> sig_ex4;

	sig_ex4.connect(0, slots_ex<10>());
	assert(sig_ex4.num_slots() == 1);
	{
		scoped_connection sc = sig_ex4.connect(0, slots_ex<20>());
		assert(sig_ex4.num_slots() == 2);
	}
	assert(sig_ex4.num_slots() == 1);

	signal<int(int)> sig_ex5;
	connection c4 = sig_ex5.connect(0, slots_ex<10>());
	connection c5 = sig_ex5.connect(0, slots_ex<20>());
	assert(sig_ex5.num_slots() == 2);
	sig_ex5(2);

	cout << "beign blocking.." << endl;
	{
		shared_connection_block block(c4);
		assert(sig_ex5.num_slots() == 2);
		assert(c4.blocked());
		sig_ex5(2);
	}
	cout << "end blocking.." << endl;
	assert(!c4.blocked());
	sig_ex5(2);
}	

#include<boost/make_shared.hpp>

void signal_autoManage()
{
	signal<int(int)> sig;
	sig.connect(slots_ex<10>());
	auto p = new slots_ex < 20 > ;

	sig.connect(boost::ref(*p));
	delete p;				//指向插槽的指针被销毁
	sig(1);					//信号调用发生未定义行为

	typedef signal<int(int)> signal_t;

	signal_t sig_t;
	sig_t.connect(slots_ex<10>());
	//boost::shared_ptr<slots_ex<20> > sp(new slots_ex<20>);
	auto sp = boost::make_shared<slots_ex<20>>();

	sig_t.connect(signal_t::slot_type(boost::ref(*sp)).track(sp));
	sp.reset();
	assert(sig_t.num_slots() == 1);
	sig_t(1);
}

void signal_autoManage2()
{
	typedef signal<int(int)> signal_t;
	typedef signal_t::slot_type slot_t;
	signal_t sig;

	auto p1 = boost::make_shared<slots_ex<10> >();
	auto p2 = boost::make_shared<slots_ex<20> >();

	boost::function<int(int)> func = boost::ref(*p1);

	sig.connect(slot_t(func).track(p1));
	sig.connect(slot_t(&slots_ex<20>::operator (), p2.get(), _1).track(p2));

	p1.reset();
	p2.reset();
	assert(sig.num_slots() == 0);
	sig(1);
}

class ring
{
public:
	typedef signal<void()> signal_t;
	typedef signal_t::slot_type slot_t;

	connection connect(const slot_t& s)
	{
		return alarm.connect(s);
	}

	void press()
	{
		cout << "Ring alarm..." << endl;
		alarm();
	}
private:
	signal_t alarm;
};

#include <boost/random.hpp>
#include <ctime>

typedef variate_generator<rand48, uniform_smallint<> > bool_rand;
bool_rand g_rand(rand48(time(0)), uniform_smallint<>(0, 100));

extern char const nursel1[] = "Mary";
extern char const nursel2[] = "Kate";

template<char const *name>
class nurse
{
private:
	bool_rand &rand;
public:
	nurse() :rand(g_rand){}
	void action()
	{
		cout << name;
		if (rand() > 30)
		{
			cout << " wakeup and open door." << endl;
		}
		else
		{
			cout << " is sleeping..." << endl;
		}
	}
};

extern char const baby1[] = "Tom";
extern char const baby2[] = "Jerry";

template<char const *name>
class baby
{
private:
	bool_rand &rand;
public:
	baby() :rand(g_rand){}
	void action()
	{
		cout << "Baby "<<name;
		if (rand() > 50)
		{
			cout << " wakeup and crying loudly..." << endl;
		}
		else
		{
			cout << " is sleeping sweetly..." << endl;
		}
	}
};

class guest
{
public:
	void press(ring &r)
	{
		cout << "A guest press the ring." << endl;
		r.press();
	}
};

void Observer_example()
{
	ring r;
	nurse<nursel1> n1;
	nurse<nursel2> n2;
	baby<baby1> b1;
	baby<baby2> b2;
	guest g;

	r.connect(bind(&nurse<nursel1>::action, n1));
	r.connect(bind(&nurse<nursel2>::action, n2));
	r.connect(bind(&baby<baby1>::action, b1));
	r.connect(bind(&baby<baby2>::action, b2));
	g.press(r);

	auto sp = std::make_shared<int>(10);
}

void functional_test()
{
	result_of_usage();
	ref_usage();
	ref_usage1();
	bind_usage();
	function_usage();
	function_callback();
	signals2_usage();
	signal_autoManage();
	signal_autoManage2();
	Observer_example();
}

#endif// LEE_FUNCTIONAL
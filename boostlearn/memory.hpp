#ifndef LEE_MEMORY
#define LEE_MEMORY

#include <iostream>
#include <boost/smart_ptr.hpp>
#include <vector>

using namespace std;
using namespace boost;

struct posix_file
{
	posix_file(const char* file_name)
	{cout << "open file:" << file_name << endl;}
	~posix_file()
	{cout << "close file" << endl;}
};

void scoped_ptr_usage()
{
	scoped_ptr<string> sp(new string("text"));

	cout << *sp << endl;
	cout << sp->size() << endl;

	scoped_ptr<int> p(new int);
	if (p)
	{
		*p = 100;
		cout << *p << endl;
	}
	p.reset();
	assert(p == 0);
	if (!p)
	{
		cout << "scoped_ptr == null" << endl;
	}
	scoped_ptr<posix_file> fp(new posix_file("/tmp/a.txt"));

	auto_ptr<int> ap(new int(10));
	scoped_ptr<int> sp1(ap);
	assert(ap.get() == 0);

	ap.reset(new int(20));
	cout << *ap << "," << *sp1 << endl;

	auto_ptr<int> ap2;
	ap2 = ap;
	assert(ap.get() == 0);
	scoped_ptr<int> sp2;
	//sp2 = sp1;
}

void scoped_array_usage()
{
	scoped_array<int> sa1(new int[100]);
	sa1[0] = 10;
	//*(sa1 + 1 = 20);

	int *arr = new int[100];
	scoped_array<int> sa(arr);
	fill_n(&sa[0], 100, 5);

	sa[10] = sa[20] + sa[30];

	vector<int> sa2(100, 5);
	sa2[10] = sa2[20] + sa2[30];
}

void f(void*)
{
	int a = 10;
	cout << a;
}

void f(int)
{
	int a = 20;
	cout << a;
}

#include <iostream>
#include <algorithm>
#include <bitset>
#include <numeric>
#include <utility>
void queen() {
	for (int queens[] = { 0, 1, 2, 3, 4, 5, 6, 7 }; ::std::next_permutation(queens, queens + 8);)
		if ((::std::bitset<15>(::std::accumulate(queens, queens + 8, ::std::make_pair(0, 0), [](::std::pair<int, int> a, int b){return ::std::make_pair((1 << (b + a.second)) | a.first, a.second + 1); }).first).count() == 8) && (::std::bitset<15>(::std::accumulate(queens, queens + 8, ::std::make_pair(0, 0), [](::std::pair<int, int> a, int b){return ::std::make_pair((1 << (7 + b - a.second)) | a.first, a.second + 1); }).first).count() == 8))
			::std::cout << queens[0] << queens[1] << queens[2] << queens[3] << queens[4] << queens[5] << queens[6] << queens[7] << ::std::endl;
}

class shared
{
private:
	std::shared_ptr<int> p;
public:
	shared(std::shared_ptr<int> p_) :p(p_){}
	void print()
	{
		cout << "count:" << p.use_count() << "v =" << *p << endl;
	}
};
void print_func(std::shared_ptr<int> p)
{
	cout << "count:" << p.use_count() << "v =" << *p << endl;
}

void shared_ptr_usage()
{
	std::shared_ptr<int> spi(new int);
	assert(spi);
	*spi = 253;
	std::shared_ptr<string> sps(new string("smart"));
	assert(sps->size() == 5);

	typedef std::shared_ptr<string> sp_t;
	map<sp_t, int> m;
	sp_t sp(new string("one"));
	m[sp] = 111;

	std::shared_ptr<std::exception> sp1(new bad_exception("error"));
	std::shared_ptr<bad_exception> sp2 = dynamic_pointer_cast<bad_exception>(sp1);
	std::shared_ptr<std::exception> sp3 = static_pointer_cast<std::exception>(sp2);
	assert(sp3 == sp1);

	std::shared_ptr<int> sp4(new int(10));
	assert(sp4.unique());

	std::shared_ptr<int> sp5 = sp4;
	assert(sp4 == sp5 && sp4.use_count() == 2);

	*sp5 = 100;
	assert(*sp4 == 100);

	sp.reset();
	assert(!sp);

	std::shared_ptr<int> p(new int(100));
	shared s1(p), s2(p);

	s1.print();
	s2.print();

	*p = 20;
	print_func(p);

	s1.print();

	std::shared_ptr<string> sp6 = std::make_shared<string>("make_shared");
	std::shared_ptr<vector<int>> spv = std::make_shared<vector<int>>(10, 2);
	assert(spv->size() == 10);

	typedef vector<std::shared_ptr<int> > vs;
	vs v(10);

	int i = 0;
	for (vs::iterator pos = v.begin(); pos != v.end();++pos)
	{
		(*pos) = std::make_shared<int>(++i);
		cout << *(*pos) << ", ";
	}
	cout << endl;

	std::shared_ptr<int> pp = v[9];
	*pp = 100;
	cout << *v[9] << endl;
}

//bridge
class sample
{
private:
	class impl;
	std::shared_ptr<impl> p;
public:
	sample();
	void print();
};

class sample::impl
{
public:
	void print()
	{
		cout << "impl print" << endl;
	}
};

sample::sample():p(new impl){}
void sample::print(){ p->print(); }

//factory 
class abstract
{
public: 
	virtual void f() = 0;
	virtual void g() = 0;
protected:
	virtual ~abstract(){}
};

class implf:public abstract
{
public:
	virtual void f()
	{
		cout << "class implf f" << endl;
	}
	virtual void g()
	{
		cout << "class implf g" << endl;
	}
};

std::shared_ptr<abstract> create()
{
	return std::shared_ptr<abstract>(new implf);
}

void design_patterns_usage()
{
	sample s;
	s.print();
	
	std::shared_ptr<abstract> p = create();
	p->f();
	p->g();

	abstract *q = p.get();
}

void shared_array_usage()
{
	int *p = new int[100];
	shared_array<int> sa(p);
	shared_array<int> sa2 = sa;

	sa[0] = 10;
	assert(sa2[0] == 10);
}

void weak_ptr_usage()
{
	std::shared_ptr<int> sp(new int(10));
	assert(sp.use_count() == 1);

	std::weak_ptr<int> wp(sp);
	assert(wp.use_count() == 1);

	if (!wp.expired())
	{
		std::shared_ptr<int> sp2 = wp.lock();
		*sp2 = 100;
		assert(wp.use_count() == 2);
	}

	assert(wp.use_count() == 1);
	sp.reset();
	assert(wp.expired());
	assert(!wp.lock());
}

#include <boost/pool/pool.hpp>

void pool_usage()
{
	pool<> pl(sizeof(int));

	int *p = (int*)pl.malloc();
	assert(pl.is_from(p));

	pl.free(p);
	for (int i = 0; i < 100; ++i)
	{
		pl.ordered_malloc(10);
	}
}

#include <boost/pool/object_pool.hpp>
struct demo_class1
{
public:
	int a, b, c;
	demo_class1(int x = 1, int y = 2, int z = 3) :a(x), b(y), c(z){}
};

void object_pool_usage()
{
	object_pool<demo_class1> pl;

	demo_class1 *p = pl.malloc();
	assert(pl.is_from(p));

	assert(p->a != 1 ||p->b !=2 ||p->c != 3);

	p = pl.construct(7,8,9);
	assert(p->a == 7);

	object_pool<string> pls;
	for (int i = 0; i < 10;++i)
	{
		string *ps = pls.construct("hello object_pool");
		cout << *ps << endl;
	}
}

#include <boost/pool/singleton_pool.hpp>

struct  pool_tag{};

typedef singleton_pool<pool_tag, sizeof(int)> spl;

#include <boost/pool/pool_alloc.hpp>

void singleton_usage()
{
	int *p = (int *)spl::malloc();
	assert(spl::is_from(p));
	spl::release_memory();

	vector<int, pool_allocator<int> > v;
	v.push_back(10);

	cout << v.size();
}

void memory_test()
{
#if 0
	scoped_ptr_usage();
	f(NULL);
	scoped_array_usage();
	shared_ptr_usage();
	design_patterns_usage();
	weak_ptr_usage();
#endif			
	object_pool_usage();
}
#endif //LEE_MEMORY
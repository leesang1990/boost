//
//  array.hpp
//  boost
//
//  Created by 李川 on 15/3/31.
//  Copyright (c) 2015年 李川. All rights reserved.
//

#ifndef LEE_ARRAY
#define LEE_ARRAY

#include <boost/array.hpp>
#include <iostream>
#include <boost/assign.hpp>

using namespace std;
using namespace boost;
using namespace boost::assign;

void array_usage()
{
    boost::array<int,10> ar;
    ar[0] = 1;
    ar.back() = 10;
    assert(ar[ar.max_size() - 1] == 10);
    
    ar.assign(777);
    for (auto pos = ar.begin(); pos != ar.end(); ++pos)
    {
        cout << *pos << endl;
    }
    
    int *p = ar.c_array();
    *(p + 5) = 253;
    cout << ar[5] <<endl;
    
    ar.at(8) = 666;
    sort(ar.begin(),ar.end());
    
}

void array_usage2()
{
    boost::array<string, 3> ar = {"alice","bob","car1"};
    
    int a[10] = {0};
    boost::array<int, 10> ar1 = {0};
    
    assert(std::equal(ar1.begin(),ar1.end(),a));
    
    boost::array<string, 3> ar2 = {"racer"};
    assert(ar2.at(1).empty());
    
    boost::array<int, 3> arr(list_of(2)(4)(6));
    for(int i = 0;i < arr.size();++i)
    {
        cout << arr[i] << ",";
    }
    
}

//void xorr(unsigned char *buf,int buf_len)  //找不到
//{
//    boost::ref_array<unsigned char> ra(buf,buf_len);
//}
//
//void ref_array_usage()
//{
//    
//}

#include <boost/dynamic_bitset.hpp>

void dynamic_bitset_usage()
{
    dynamic_bitset<> db1;
    dynamic_bitset<> db2(10);
    dynamic_bitset<> db3(0x16,BOOST_BINARY(10101));
    dynamic_bitset<> db4(string("0100"));
    dynamic_bitset<> db5(db3);
    
    dynamic_bitset<> db6;
    db6 = db4;
    
    cout << hex <<db5.to_ulong() << endl;
    cout << db4[0] << db4[1] << db4[2] << endl;
    cout << db3 << endl;
    
    
    dynamic_bitset<> db7;
    db7.resize(10,true);
    cout << db7 << endl;
    db7.resize(5);
    cout << db7 << endl;
    
    dynamic_bitset<> db8(5,BOOST_BINARY(01110));
    cout << db8 << endl;
    assert(db8.size() == 5);
    db8.clear();
    assert(db8.empty() && db8.size() == 0);
    
    dynamic_bitset<> db9(5,BOOST_BINARY(01001));
    db9.push_back(true);
    assert(db9.to_ulong() == BOOST_BINARY(101001));
    db9.append(BOOST_BINARY(101));
    cout << db9 << endl;
    
}

void dynamic_bitset_operator()
{
    dynamic_bitset<> db1(4,BOOST_BINARY(1010));
    
    db1[0] &= 1;
    db1[1] ^= 1;
    cout << db1 << endl;
    
    dynamic_bitset<> db2(4,BOOST_BINARY(0101));
    assert(db1 > db2);
    cout << (db1 ^ db2) << endl;
    cout << (db1 | db2) << endl;
    
    dynamic_bitset<> db3(4,BOOST_BINARY(0101));
    assert(db3.test(0) && !db3.test(1));
    assert(db3.any() && !db3.none());
    assert(db3.count() == 2);
    
    dynamic_bitset<> db4(db3);
    db4.flip();
    assert(db4.to_ulong() == BOOST_BINARY(1010));
    
    db4.set();
    assert(!db4.none());
    
    db4.reset();
    assert(!db4.any());
    
    db4.set(1,1);
    assert(db4.count() == 1);
    
    dynamic_bitset<> db5(5,BOOST_BINARY(00101));
    auto pos = db5.find_first();
    assert(pos == 0);
    
    pos = db5.find_next(pos);
    assert(pos == 2);
}

void dynamic_bitset_bit_operator()
{
    dynamic_bitset<> db1(5,BOOST_BINARY(10101));
    dynamic_bitset<> db2(5,BOOST_BINARY(10010));
    
    cout << (db1 | db2) << endl;
    cout << (db1 & db2) << endl;
    cout << (db1 - db2) << endl;
    
    dynamic_bitset<> db3(5,BOOST_BINARY(101));
    assert(db3.is_proper_subset_of(db1));
    
    dynamic_bitset<> db4(db2);
    assert(db4.is_subset_of(db2));
    assert(!db4.is_proper_subset_of(db2));
}

void out_put_prime()
{
    int n;
    cin >> n;
    dynamic_bitset<> db(n);
    
    db.set();
    for(dynamic_bitset<>::size_type i = db.find_next(1); i != dynamic_bitset<>::npos;i = db.find_next(i))
    {
        for (dynamic_bitset<>::size_type j = db.find_next(i); j != dynamic_bitset<>::npos; i = db.find_next(j))
        {
            if ( j % i == 0)
            {
                db[j] = 0;
            }
        }
    }
    
    for(dynamic_bitset<>::size_type i = db.find_next(2); i != dynamic_bitset<>::npos ;i = db.find_next(i))
    {
        cout << i << ",";
    }
    
}

#include <boost/unordered_map.hpp>

void unordered_map_usgae()
{
    unordered_map<int, string> um = map_list_of(1, "one")(2,"two")(3,"three");
    
    um.insert(make_pair(10, "ten"));
    cout << um[10] << endl;
    um[11] = "eleven";
    um[15] = "fifteen";
    
    auto p = um.begin();
    for(p; p != um.end();++p)
    {
        cout << p->first << "-" << p->second << ",";
    }
    cout << endl;
    
    um.erase(11);
    cout << um.size() << endl;
    
}

#include <typeinfo>
#include <set>
#include <boost/unordered_set.hpp>
#include <boost/random.hpp>
#include <boost/progress.hpp>

template<typename T>
void fill_set(T &c)
{
    variate_generator<mt19937, uniform_int<> > gen(mt19937(),uniform_int<>(0,100));
    for (int i = 0; i < 10000; ++i)
    {
        c.insert(gen());
    }
}

template<typename T>
void test_perform()
{
    T c;
    cout << typeid(c).name() << endl;
    {
        progress_timer t;
        fill_set(c);
    }
    {
        progress_timer t;
        c.count(10);
    }
    {
        progress_timer t;
        c.find(20);
    }
    
}

void test_container_perform()
{
    test_perform<multiset<int> >();
    test_perform<unordered_multiset<int> >();
}


void unordered_inset_datestruct()
{
    unordered_set<int> us = (list_of(1),2,3,4);
    cout << us.bucket_count() << endl;
    for (int i = 0; i < us.bucket_count(); ++i)
    {
        cout << us.bucket_size(i) << ",";
    }
    cout << endl;
    
    for (auto p = us.begin(); p != us.end(); p++) {
        cout << *p << ",";
    }
    cout << endl;
    
    unordered_set<int> us2(100);
    us2.rehash(200);
    
}

#include <boost/functional/hash.hpp>
struct demo_class
{
    int a;
    friend bool operator==(const demo_class& l,const demo_class& r)
    {
        return l.a == r.a;
    }
};

size_t hash_value(demo_class &s)
{
    return boost::hash<int>()(s.a);
}

void unordered_set_custom_type()
{
    unordered_set<demo_class> us;
}


#include <boost/bimap.hpp>
using namespace boost::bimaps;

void bimap_usage()
{
    bimap<int , string> bm;
    bm.left.insert(make_pair(1, "111"));
    bm.left.insert(make_pair(2, "222"));
    
    bm.right.insert(make_pair("string", 10));
    bm.right.insert(make_pair("bimap",20));
    
    for (auto pos = bm.right.begin(); pos != bm.right.end(); ++pos)
    {
        cout << "left[" << pos->first << "]=" << pos->second << endl;
    }
    
}

//TODO:
//template<typename T>
//void print_map(T &m)
//{
//    for (auto pos = m.begin(); pos != m.end(); ++pos)
//    {
//        cout << pos->first << "<-->" << pos->second << endl;
//    }
//}
//
//void bimap_usage2()
//{
//    bimap<unordered_multiset_of<int>,unordered_multiset_of<string> > bm;
//    bm.left.insert(make_pair(1, "111"));
//    bm.left.insert(make_pair(2, "222"));
//    bm.left.insert(make_pair(2, "555"));
//    
//    bm.right.insert(make_pair("string",10));
//    bm.right.insert(make_pair("bimap",20));
//    bm.right.insert(make_pair("bimap", 2));
//    
//    print_map(bm.left);
//}
#include <boost/bimap/support/lambda.hpp>

void bimap_find()//当bimap的左右视图是set时 可调用find（）
{
    typedef bimap<int, string> bm_t;
    bm_t bm = assign::list_of<bm_t::relation>(1,"mario")(2,"peach");
    
    auto pos = bm.left.find(1);
    cout << "[" << pos->first << "]=" << pos->second << endl;
    
    auto pos2 = bm.right.find("peach");
    cout << "[" << pos2->first << "]=" << pos2->second << endl;
    
    bm.left.replace_key(pos, 111);
    bm.left.replace_data(pos,"luigi");
    cout << "[" << pos->first << "]=" << pos->second << endl;
    
    bm.left.modify_key(pos, _key = 222);
    bm.left.modify_data(pos,_data = "lee");
    cout << "[" << pos->first << "]=" << pos->second << endl;
    
    bm.left.modify_key(pos, _key = 2);              //与（2,"peach")冲突 修改的元素被删除
    assert(bm.left.size() == 1);
    assert(bm.right.find("mario") == bm.right.end());
}


void bimap_project()
{
    typedef bimap<int, string> bm_t;
    bm_t bm = assign::list_of<bm_t::relation>(1,"mario")(2,"peach")(3,"333");
    
    auto left_pos = bm.left.find(3);
    auto right_pos = bm.project_right(left_pos);
    cout << "right:[" << right_pos->first << "]=" << right_pos->second;
}

#include <boost/circular_buffer.hpp>

void circular_buffer_usage1()
{
	boost::circular_buffer<int> cb(5);
	assert(cb.empty());

	cb.push_back(1);
	cb.push_front(2);
	assert(cb.front() == 2);
	cb.insert(cb.begin(), 3);

	for (auto pos = cb.begin(); pos != cb.end();++pos)
	{
		cout << *pos << ",";
		cout << endl;
	}
	
	cb.pop_front();
	assert(cb.size() == 2);
	cb.push_back();
	assert(cb[0] = 2);

	circular_buffer<int> cb1 = list_of(1)(2)(3);
	circular_buffer<int> cb2 = list_of(4)(5)(6);
	circular_buffer<int> cb3 = cb1;

	assert(cb1 < cb2);
	assert(cb1 == cb3);
}

template<typename T>
void print(T& cb)
{
	for (auto pos = cb.begin();pos != cb.end();++pos)
	{
		cout << *pos << ",";
	}
	cout << endl;
	
}

void circular_buffer_usage2()
{
	circular_buffer<int> cb = (list_of(1), 2, 3);

	print(cb);

	cb.push_back(4);
	print(cb);

	cb.push_back(5);
	print(cb);

	cb.pop_front();
	print(cb);

	assert(!cb.full());

	int *p = cb.linearize();
	assert(p[0] == 4 && p[1] == 5);

	cb.rotate(cb.begin() + 1);
	print(cb);

	circular_buffer<int> cb2 = (list_of(0), 1, 2, 3, 4, 5, 6, 7, 8, 9);
	auto pos = cb.begin();

	assert(*(pos + 3) == 3);
	//cout << *(pos + 10) << endl;
}

#include <boost/tuple/tuple.hpp>

typedef boost::tuple<int, string, double> my_tuple;

#include <boost/core/ref.hpp>

void tuple_usage1()
{
	my_tuple t1 ;
	my_tuple t2(1,"123");
	my_tuple t3(t1);

	t2 = t3;

	int x = 10;
	boost::tuple<int&> t4(x);
	//boost::tuple<int&> t5;  //error  引用类型必须初始化

	typedef boost::tuple<void>			no_instance_tl; //void类型 和函数类型没有实例
	typedef boost::tuple<double(int)>	no_instance_t2;

	typedef boost::tuple<void*>  t6;				//void* 和函数指针可以实例化
	typedef boost::tuple<double(*)(int)> t7;

	make_tuple(2, 3.0);
	make_tuple(std::string(), vector<int>());

	int i;
	std::string s;
	std::tuple<int, std::string&> t8	   = make_tuple(i, boost::ref(s));
	std::tuple<const int&, std::string> t9 = make_tuple(boost::cref(i),s);
}

void tuple_get_usage()
{
	auto t = boost::make_tuple(1,"char[]",100.00);
	assert(t.get<0>() == 1);
	assert(t.get<2>() == 100.00);
	cout << t.get<1>()<<endl;
	cout << ++t.get<0>()<<endl;
	cout << get<1>(t) << endl;

}

#include <boost/tuple/tuple_comparison.hpp>

typedef boost::tuple<int, double, string> my_tuple1;

void tuple_comparison_usage()
{
	my_tuple1 t1 = boost::make_tuple(1, 100.0, string("abc"));
	my_tuple1 t2 = boost::make_tuple(1, 200.0, string("def"));
	assert(t1 < t2);

	my_tuple1 t3(t2);
	assert(t2 == t3);
}

#include <boost/tuple/tuple_io.hpp>

void tuple_io_usage()
{
	my_tuple1 t1(1, 2.0, "string");
	my_tuple1 t2;
	cout << t1 << endl;
	cout << "please input tuple:";
	cin >> t2;							//每个元素后面必须跟一个空格
	cout << t2 << endl;

	cout << boost::tuples::set_open('[') << boost::tuples::set_close(']');
	cout << boost::tuples::set_delimiter(',');
	cout << t1 << endl;
}

void tuple_tie_usage()
{
	auto func = [=](){ return boost::make_tuple(1, 2.0, "asdf"); };

	double d; string s;
	tie(tuples::ignore, d, s) = func();
	cout << s;

	int ii; string ss;
	tie(ii, ss) = make_pair(100, "abc");
}

template<typename Tuple>
void print_tuple(const Tuple& t)
{
	cout << t.get_head() << ",";
	print_tuple(t.get_tail());
}

void print_tuple(const boost::tuples::null_type&){}

//==============================================================

template<typename Visitor,typename Tuple>
typename boost::result_of<Visitor(Tuple&)>::type
visit_tuple(Visitor v, const Tuple &t)
{
	v(t.get_head());
	return visit_tuple(v, t.get_tail());
}

struct print_visitor
{
	typedef void result_type;
	template<typename T>
	result_type operator()(const T &t)
	{
		cout << t << ",";
	}
};

template<>
void visit_tuple<print_visitor,boost::tuples::null_type>(print_visitor, const boost::tuples::null_type &){}

// template<typename T>
// struct max_visitor
// {
// 	T *tmp;
// 	max_visitor(T *t) :tmp(t){}
// 	typedef void result_type;
// 	result_type operator()(const T &t)
// 	{
// 		*tmp = *tmp < t ? t : *tmp;
// 	}
// };
// 
// template<>
// void visit_tuple<max_visitor<double>, boost::tuples::null_type>
// (max_visitor<double>, const boost::tuples::null_type &){}
//==============================================================

void tuple_assign_usage()
{
	using namespace boost::assign;

	vector<my_tuple1> v = assign::tuple_list_of(1, 1.0, "123")(2, 2.0, "456");
	assert(v.size() == 2);

	v += boost::make_tuple(3, 3.0, "789"), boost::make_tuple(4, 4.0, "abc");
	assert(v.size() == 4);

	print_tuple(v[1]);
	visit_tuple(print_visitor(), v[1]);

// 	boost::tuple<int, long, float, double> t(100, 5, 3.14, 314.15);
// 	double *max_value = new double(t.get_head());
// 	max_visitor<double> mv(max_value);
// 	visit_tuple(mv, t);
// 	cout << *max_value << endl;
	int i;
	cout << typeid(i).name() << endl;
}

void tuple_advanced_usgae()
{
	typedef boost::tuple<int, string> my_tuple1;
	typedef boost::tuple<int, my_tuple1> my_tuple2;
	assert(typeid(int) == typeid(boost::tuples::element<0,my_tuple1>::type));
	assert(typeid(string) == typeid(boost::tuples::element<1, my_tuple1>::type));
	cout << boost::tuples::length<my_tuple2>::value << endl;
}

#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>

void any_usage()
{
	//boost::shared_ptr<int> p(new int(10));
	auto p = make_shared<int>(10);    //int *p = new int(10);内存泄露
	any a = p;

	any aa(10);

	int n = any_cast<int>(aa);
	assert(n == 10);

	any_cast<int&>(aa) = 20;
	assert(any_cast<int>(aa) == 20);

	any b(100);
	b = string("char*"); //b = "char*" 错误
	//b = vector<vector<int> >();

	b = string("avatar");
	cout << any_cast<string>(b);
	//any_cast<int>(b);

}
namespace lee_any_get{
	template<typename T>
	bool can_cast(any &a)
	{
		return typeid(T) == a.type();
	}

	template<typename T>
	T& get(any &a)
	{
		BOOST_ASSERT(can_cast<T>(a));
		return *any_cast<T>(&a);
	}

	template<typename T>
	boost::shared_ptr<T>& get_shared(any &a)
	{
		BOOST_ASSERT(can_cast<boost::shared_ptr<T>>(a));
		return *any_cast<boost::shared_ptr<T>>(&a);
	}

	template<typename T>
	T* get_pointer(any &a)
	{
		BOOST_ASSERT(can_cast<T>(a));
		return any_cast<T>(&a);
	}

	template<typename T>
	any make_ptr_any(T *p = 0)
	{
		return any(boost::shared_ptr<T>(p));
	}
};


void any_assis_function()
{
	any a;
	int x = 1;
	a = x;
	assert(lee_any_get::can_cast<int>(a));
	lee_any_get::get<int>(a) = 10;
	*lee_any_get::get_pointer<int>(a) = 20;

	any b = lee_any_get::make_ptr_any<string>(new string("long"));
	//b = make_ptr_any<vector<int> >(new vector<int>);
	cout << *any_cast<boost::shared_ptr<string> >(b);
	cout << *lee_any_get::get<boost::shared_ptr<string> >(b);
	cout << *lee_any_get::get_shared<string>(b);
}

template<typename T>
struct any_print
{
	void operator()(any &a)
	try
	{
		cout << *any_cast<T>(&a) << endl;
	}
	catch (bad_any_cast &)
	{
		cout << "print error" << endl;
	}
};

template<typename T>
struct any_print<std::shared_ptr<T> >
{
	void operator()(any &a)
	try
	{
		cout << **any_cast<std::shared_ptr<T>>(&a) << endl;
	}
	catch (bad_any_cast &)
	{
		cout << "print error" << endl;
	}	
};

template<typename T>
void inline any_print_func(any &a)
{
	any_print<T>()(a);
}

void any_io()
{
	any a;
	char* str = "string";
	a = str;
	any_print<char* >()(a);
	a = 10;
	any_print<int>()(a);

	std::shared_ptr<string> ps = make_shared<string>("metroid");
	a = ps;
	any_print <std::shared_ptr<string> >()(a);

	any_print_func<std::shared_ptr<string> >(a);

	vector<any> v;
	v.push_back(10);
	v.push_back(1.414);
	v.push_back(make_shared<int>(10));

	vector<any> v2 = list_of<any>(10)(0.618)(string("char"));
	cout << v2.size();
}

#include <boost/variant.hpp>
#include <boost/variant/get.hpp>

void variant_usage()
{
	typedef variant<int, double, string> var_t;
	//variant<int, float, string> v;
	//v = "123";

	var_t v(1);
	v = 2.13;
	var_t v2("string type");
	v2 = v;

	assert(v.type() == typeid(double));

	cout << get<double>(v);
 	cout << get<int>(var_t(108));

	var_t v3;
	assert(v3.type() == typeid(int));
	assert(v3.which() == 0);

	v3 = "variant demo";
	cout << *get<string>(&v3) << endl;

	try
	{
		cout << get<double>(v3) << endl;
	}
	catch (bad_get &)
	{
		cout << "bad_get" << endl;
	}
	
}

typedef variant<int, double > var_t;

void var_print(var_t &v)
{
	if (v.type() == typeid(int))
	{
		get<int>(v) *= 2;
		cout << v << endl;
	}
	else if (v.type() == typeid(double))
	{
		get<double>(v) *= 2;
		cout << v << endl;
	}
	else
	{
		cout << "don't konw type" << endl;
	}
}

template<typename ResultType>
class static_visitor
{
public:
	typedef ResultType result_type;
protected:
	static_visitor(){}
	~static_visitor(){}
};
/*
template<typename ResultType>
struct var_print :public static_visitor<>
{
	template<typename T>
	void operator()(T &i) const
	{
		i *= 2;
		cout << i << endl;
	}

	void operator() < vector<int> >(vector<int> &v) const
	{
		copy(v.begin(), v.end(), back_inserter(v));
		for (int i = 0; i < v.size();++i)
		{
			cout << v[i] << ",";
		}
		cout << endl;
		
	}
};

#include <boost/variant/detail/apply_visitor_binary.hpp>

void variant_visitor_usage()
{
	var_t v(1);
	var_print vp;
	apply_visitor(vp, v);
}
*/
#include <boost/multi_array.hpp>

void multi_array_usage()
{
	multi_array<int, 3> ma(extents[2][3][4]);
	auto shape = ma.shape();

	for (size_t i = 0; i < ma.num_dimensions();++i)
	{
		cout << shape[i] << ",";
	}
	cout << endl << ma.num_dimensions()<< endl;

	for (int i = 0, v = 0; i < 2; ++i)
		for (int j = 0; j < 3; ++j)
			for (int k = 0; k < 4;++k)
			{
				ma[i][j][k] = v++;
			}

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				cout << ma[i][j][k] << ",";
			}
			cout << endl;
		}
		cout << endl;
	}

	boost::array<size_t, 3> idx = {0,1,2};
	ma(idx) = 100;
	cout << ma(idx) << endl;;


	//cout << ma[2][3][4];
	
}

void container_test()
{
	array_usage2();
	dynamic_bitset_usage();
	dynamic_bitset_operator();
	unordered_map_usgae();
	test_container_perform();
	unordered_inset_datestruct();
	unordered_set_custom_type();
	bimap_usage();
	//bimap_usage2();
	bimap_find();
	bimap_project();
	circular_buffer_usage2();
	tuple_usage1();
	tuple_get_usage();
	tuple_comparison_usage();
	tuple_io_usage();
	tuple_tie_usage();
	tuple_assign_usage();
	any_usage();
	any_assis_function();
	any_io();
	variant_usage();
	multi_array_usage();
}

#endif //LEE_ARRAY

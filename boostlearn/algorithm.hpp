#ifndef LEE_ALGORITHM
#define LEE_ALGORITHM

#include <iostream>
#include <boost/assign.hpp>

using namespace std;
using namespace boost::assign;

#include <boost/foreach.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/unordered_set.hpp>
#include <boost/array.hpp>

void foreach_usage()
{
	std::string str("std foreach");

	//std for each
	for each(char c in str)
	{
		cout << c << "-";
	}
	cout << endl;

	//boost foreach
	vector<int> v = (list_of(1),2,3,4,5);

	BOOST_FOREACH(int x, v)
	{
		cout << x << ",";
	}
	cout << endl;

	set<int> s = list_of(10)(20)(30);
	//BOOST_TYPEOF(*s.begin()) y;//int y;
	BOOST_FOREACH(BOOST_TYPEOF(*s.begin()) y, s)
	{
		if (++y % 7 == 0)
		{
			cout << y << endl;
			break;
		}
		
	}

#define foreach BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

	//遍历普通数组
	int ar[] = { 1, 2, 3, 4, 5 };
	foreach(int& x, ar)
	{
		cout << x << " ";
	}
	cout << endl;

	//遍历map
	map<int, string> m = map_list_of(1, "111")(2, "222")(3,"333");
	std::pair<int, string> x;
	foreach(x,m)
	{
		cout << x.first << x.second << endl;
	}
	
	//遍历用vector实现的二维数组
	vector<vector<int> > vv = list_of(list_of(1)(2))(list_of(3)(4));
	foreach(BOOST_TYPEOF(*vv.begin()) row, vv)
	{
		reverse_foreach(int& z, row)
			cout << z << ",";
		cout << endl;
	}

	//boost::array
	::boost::array<int, 5> arr = (list_of(1),2,3,4,5);
	foreach(int x, arr)
	{
		cout << x << " ";
	}
	cout << endl;

	//迭代器的pair
	std::pair<boost::array<int, 5>::iterator, boost::array<int, 5>::iterator> rng(arr.begin(), arr.end() - 2);
	foreach(int x, rng)
		cout << x << " ";
	cout << endl;

	//循环缓冲区boost::circular_buffer
	boost::circular_buffer<int> cb = (list_of(1),2,3);
	foreach(int x, cb)
		cout << x << " ";
	cout << endl;

	//无序列容器boost::unordered_set
	boost::unordered_set<double> us = list_of(3.14)(2.717)(0.618);
	foreach(double x, us)
		cout << x << " ";
	cout << endl;
}

struct Comp
{
	bool operator()(const string &a, const string &b)
	{
		return (a < b) || (b.find("BOSS") != string::npos);
	}
};

void min_max_usage()
{
	cout << std::max(200, 12) << endl;
	cout << std::min(200, 12) << endl;
	boost::tuple<int, int> x = boost::tie(minmax(1, 2).first, minmax(1, 2).second);
	cout << x.get<1>() << " " << x.get<0>()<<endl;

	string s1("5000"), s2("123BOSS");

	auto xx = minmax(s1, s2);
	boost::tuple<string, string> xxx = boost::tie(xx.first, xx.second);
	cout << xxx.get<1>() << " " << xxx.get<0>() << endl;

	auto y = minmax(s1, s2, Comp());
	boost::tuple<string, string> z = boost::make_tuple(y.first, y.second);
	cout << z.get<1>() << " " << z.get<0>() << endl;
}

#include <boost/algorithm/minmax_element.hpp>

void minmax_element_usage()
{
	vector<int> v = (list_of(633),90,67,83,2,100);

	auto x = boost::minmax_element(v.begin(),v.end());
	cout << "min: " << *x.first << endl;
	cout << "max: " << *x.second << endl;

	vector<int> vv = (list_of(3),5,2,2,10,9,10,8);
	vector<int>::iterator pos;
	pos = boost::first_min_element(v.begin(), v.end());
	assert(pos - v.begin() == 2);
	pos = boost::last_min_element(v.begin(), v.end());
	assert(pos - v.begin() == 3);
	auto xx = boost::first_min_last_max_element(v.begin(), v.end());
	assert(xx.first - v.begin() == 2 && xx.second - v.begin() == 6);
}

#define SIZE 8

void bubble_sort(int a[], int n)
{
	int i, j, temp;
	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n - 1 - i; j++)
		{
		if (a[j] < a[j + 1])
		{
			temp = a[j];
			a[j] = a[j + 1];
			a[j + 1] = temp;
		}
		}
}

void bubble_test()
{
	int number[SIZE] = { 95, 45, 15, 78, 84, 51, 24, 12 };
	int i;
	bubble_sort(number, SIZE);
	for (i = 0; i < SIZE; i++)
	{
		printf("%d   ", number[i]);
	}
	printf("\n");
}

void find_perfect_number(int range){
	for (int i = 1; i < range; i++){
		int sum = 0;
		for (int j = 1; j < i; j++){
			if (i % j == 0)
				sum += j;
		}

		if (sum == i)
			cout << i << endl;
	}

	cout << typeid(range).name();
	auto a = 10;
}

void algorithm_test()
{
#if 0
	foreach_usage();
	min_max_usage();
	minmax_element_usage();
	bubble_test();
#endif
	find_perfect_number(1000);
}

#endif //LEE_ALGORITHM
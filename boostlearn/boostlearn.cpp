// boostlearn.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "vld.h"

#include <stdio.h>
#include <cstdlib>


#include "container.hpp"
#if 0
#include "mathnumber.hpp"
#include "os.hpp"
#include "functional.hpp"
#include "datatime.hpp"
#include "memory.hpp"
#include "tool.hpp"
#include "stringandtext.hpp"
#include "thread.hpp"
#include "asio.hpp"
#endif

//#include "algorithm.hpp"

#define lambda(return_typ, function_body)	\
({											\
	return_typ fn function_body				\
	fn ;									\
})

template<int N>
struct temp_fib
{
	enum{ res = temp_fib<N - 1>::res + temp_fib<N - 2>::res };
};

template<>
struct temp_fib<1>{enum{ res = 1 };};

template<>
struct temp_fib<2>{enum{ res = 1 };};

//lambda(int, (int x, int y) { return x > y ? x : y; })(4,5);

class MyClass
{
	enum
	{
		em = 5
	};
};
int _tmain(int argc, _TCHAR* argv[])
{
#if 0
	
	
	mathnumber_test();
	string_test();
	functional_test();
	datatime_test();
	memory_test();
	tool_test();
	os_test(argc, argv);
	thread_test();
	asio_test();

	int arr[] = { 1, 1, 2, 3, 5, 8, 13 };

	int *b = 1[&arr];
	int *c = 2[&arr];
	int *d = 3[&arr];


	for (int *p = arr; p < 1[&arr]; p++)
		printf("%d\n", *p);
#endif							
	//container_test();
	//algorithm_test();

// 	float base = 0.5;
// 	int years = 0;
// 
// 	while (base < 1)
// 	{
// 		base = base + base*0.01f;
// 		years++;
// 	}
// 
// 	printf("%d", years);
	
	MyClass a; 

	cout << temp_fib<209>().res;

	
	getchar();
	return 0;
}


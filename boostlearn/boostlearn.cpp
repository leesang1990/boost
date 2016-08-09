// boostlearn.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "vld.h"

#include <stdio.h>
#include <cstdlib>

#if 0
#include "container.hpp"

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

//lambda(int, (int x, int y) { return x > y ? x : y; })(4,5);

class MyClass
{
public:
	MyClass(){};
	~MyClass(){};
	
private:
};
int _tmain(int argc, _TCHAR* argv[])
{
#if 0
	container_test();
	
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

	//algorithm_test();

	float base = 0.5;
	int years = 0;

	while (base < 1)
	{
		base = base + base*0.01f;
		years++;
	}

	printf("%d", years);
	

	getchar();
	return 0;
}


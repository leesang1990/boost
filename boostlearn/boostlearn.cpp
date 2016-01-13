// boostlearn.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "vld.h"

#include <stdio.h>
#include <cstdlib>

#if 0
#include "container.hpp"
#include "algorithm.hpp"
#include "mathnumber.hpp"
#include "os.hpp"
#include "functional.hpp"
#include "datatime.hpp"
#include "memory.hpp"
#include "tool.hpp"
#include "stringandtext.hpp"
#endif
#include "thread.hpp"
#include "asio.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
#if 0
	container_test();
	algorithm_test();
	mathnumber_test();
	string_test();
	functional_test();
	datatime_test();
	memory_test();
	tool_test();
	os_test(argc, argv);
	thread_test();
#endif							
	asio_test();
	
	getchar();
	return 0;
}


// obscure.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <stdlib.h>
#include <vector>

#include "leecode.hpp"

using namespace std;


int f()
{
	std::logic_error e("logic error");
	
	cout << "throw" << endl;
	throw e;
}

struct A
{
	int a;
	A::A():a(f())
	{

	}
};

struct B
{
	int b;
	B::B()
	try:b(f())
	{
		
	} 
	catch (std::exception e)
	{
		cout << e.what() << endl;
	}
};

struct Test {
	int data;
	Test() { cout << "Test::Test()" << endl; }
	~Test() { cout << "Test::~Test()" << endl; }
};


int main(int argc, int* argv[])
{
	// Must allocate our own memory
	Test *ptr = (Test *)malloc(sizeof(Test));

	// Use placement new
	new (ptr)Test;

	// Must call the destructor ourselves
	ptr->~Test();

	// Must release the memory ourselves
	free(ptr);

	//A a;
	//B b;

	std::vector<int> testNums = {217, 231, 523, 52, 547, 243, 648, 509,
								415, 149, 689, 710, 265, 187, 370, 56,
								977, 182, 400, 329, 471, 805, 955, 989,
								255, 766, 38, 566, 79, 843, 295, 229, 
								988, 108, 781, 619, 704, 542, 335, 307,
								359, 907, 727, 959, 161, 699, 123, 650, 
								147, 459, 657, 188, 304, 268, 405, 685, 
								620, 721, 351, 570, 899, 60, 388, 771, 
								24, 659, 425, 440, 508, 373, 32, 645, 
								409, 272, 356, 175, 533, 740, 370, 152,
								34, 510, 745, 251, 227, 494, 258, 527, 
								817, 773, 178, 194, 860, 387, 627, 851,
								449, 736, 15, 212, 529, 950, 316, 28, 65,
								484, 968, 63, 4, 643, 795, 669, 203, 677,
								139, 636, 289, 555, 430, 849, 150, 493, 301,
								377,240, 873, 965, 441, 230, 349, 447, 470 };
	
	auto ret = twoSum(testNums, 718);

	system("pause");

	return 0;
}


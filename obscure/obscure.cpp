// obscure.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <stdlib.h>

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

int _tmain(int argc, _TCHAR* argv[])
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
	B b;
	system("pause");

	return 0;
}


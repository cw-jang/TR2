// DllTestApp.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "../DllTest/DllTest.h"
//#pragma comment( lib, "DllTest.lib" )

int _tmain(int argc, _TCHAR* argv[])
{
	cMyClass MyClass;
	MyClass.Sum( 100 , 200 );
	MyClass.OutputSum();
	return 0;
}

#pragma
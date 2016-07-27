#pragma once

#include <iostream>

using namespace std;

class Sample {
	private:
		int a, b;
		int res;
	public:
		Sample() {}
		Sample(int a, int b): a(a), b(b) {}
		int getSum();
		int getA();
		int getB();
		void setRes(int res);
};

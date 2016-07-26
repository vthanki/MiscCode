#include "Calc.h"
#include <stdio.h>

using namespace std;

double getFinalResult(int a, int b, char c)
{
	Calc obj;
	double res;
	obj.setOperands(a,b);
	if (obj.setOperator(c) == 0) {
		if (obj.calculate() == 0 &&
				obj.getResult(&res) == 0) {
			return res;
		}
	}
	return -1;
}
/*
int main()
{
	return 0;
}
*/

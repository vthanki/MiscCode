#ifndef _CALC_H
#define _CALC_H

#include <iostream>
#include <string>

using namespace std;

class Calc {
	private:
		int op1, op2;
		char operation;
		double result;
	public:
		Calc() {
			operation = '\0';
		}
		Calc(int a, int b, char c) :
			op1(a),
			op2(b),
			operation(c)
		{}
		int calculate();
		void setOperands(int a, int b);
		int setOperator(char c);
		int getResult(double *result);
};

int Calc::calculate()
{
	switch(operation) {
		case '+':
			result = op1 + op2;
			break;
		case '-':
			result = op1 - op2;
			break;
		case '/':
			result = op1 / op2;
			break;
		case '*':
			result = op1 * op2;
			break;
		default:
			return -1;
	}
	return 0;
}

int Calc::getResult(double *res)
{
	if (!res)
		return -1;
	else if (operation == '\0')
		return -2;

	*res = result;
	return 0;
}

int Calc::setOperator(char c)
{
	switch(c) {
		case '+':
		case '-':
		case '/':
		case '*':
			operation = c;
			break;
		default:
			return -1;
	}
	return 0;
}

void Calc::setOperands(int a, int b)
{
	op1 = a; op2 = b;
}

double getFinalResult(int a, int b, char c);
#endif

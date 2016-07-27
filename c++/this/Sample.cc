#include "Sample.h"

void doSum(Sample *obj)
{
	obj->setRes(obj->getA() + obj->getB());
}

int Sample::getA() { return a; }
int Sample::getB() { return b; }
void Sample::setRes(int res) { this->res = res; }

int Sample::getSum() {
	doSum(this);
	return res;
}

int main()
{
	Sample s(5,10);
	cout << s.getSum() << endl;
	return 0;
}

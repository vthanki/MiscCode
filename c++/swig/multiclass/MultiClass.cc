#include "MultiClass.h"

#include <iostream>

using namespace std;

int main()
{
	Polygon p;

	p.addSide(Side(10));
	p.addSide(Side(20));
	p.addSide(Side(20));
	p.addSide(Side(10));

	cout << p.getNumberOfSides() << endl;
	cout << p.getPerimeter() << endl;

	return 0;
}

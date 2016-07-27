#ifndef _MULTI_CLASS_H
#define _MULTI_CLASS_H

#include <iostream>
#include <vector>

using namespace std;

class Side {
	private:
		int len;
	public:
		Side() {}
		Side(int l) : len(l) {}
		int getSideLength(void);
};

int Side::getSideLength(void)
{
	return len;
}


class Polygon {
	private:
		vector<Side> sides;
	public:
		int getNumberOfSides(void);
		int getPerimeter(void);
		int addSide(Side s);
		Side getSideAt(int i);
};

int Polygon::getNumberOfSides(void)
{
	return sides.size();
}

int Polygon::getPerimeter(void)
{
	int result = 0;
	for (int i = 0; i < sides.size(); i++)
		result += sides[i].getSideLength();

	return result;
}

int Polygon::addSide(Side s)
{
	if (s.getSideLength() <= 0)
		return -1;

	sides.push_back(s);
	return 0;
}

Side Polygon::getSideAt(int i)
{
	if (i >= sides.size()) {
		cout << "Invalid side index requested: " << i << endl;
		cout << "Sending side at index 0 instead" << endl;
		i = 0;
	}
	return sides[i];
}
#endif

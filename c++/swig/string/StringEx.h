#ifndef _STR_EX_H
#define _STR_EX_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class StringEx {
	private:
		vector<const char *> names;
	public:

		StringEx() {
			names.push_back("one");
			names.push_back("two");
			names.push_back("three");
		}
		void addName(const char *n);
		const char *getNameAt(int i);
		void printAll(void);
		int getSize();
};

void StringEx::addName(const char *name)
{
	cout << "Storing " << name << endl;
	string *s = new string();
	s->append(name);
	names.push_back(s->c_str());
}

void StringEx::printAll(void)
{
	for (int i = 0; i < names.size(); i++)
		cout << names[i] << endl;
}

const char* StringEx::getNameAt(int i)
{
	if (i >= names.size()) {
		cout << i << " is greater than " << names.size() << endl;
		return NULL;
	}
	cout << "Requestd " << i << " which points to "  << names[i] << endl;
	return names[i];
}

int StringEx::getSize()
{
	return names.size();
}
#endif

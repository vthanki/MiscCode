#include "StringEx.h"

#if 1
int main()
{
	StringEx obj;

	obj.addName("ram");
	obj.addName("sita");

	obj.printAll();

	cout << obj.getNameAt(0) << endl;
	cout << obj.getSize() << endl;
	return 0;
}
#endif

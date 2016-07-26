#include "myVector.h"

int countChars(MyVector vec)
{
	return vec.countChars();
}

vector<string> getAllNames(MyVector vec)
{
	vector<string> result;
	for (int i = 0; i < vec.size(); i++)
		result.push_back(vec.getNameAt(i));
	return result;
}

int main()
{
	MyVector myVec;

	myVec.addName("Ram");
	myVec.addName("Laxman");
	myVec.addName("Janaki");
	myVec.addName("Hanuman");

	cout << countChars(myVec) << endl;

	vector<string> names;
	names = getAllNames(myVec);
	for (int i = 0; i < names.size(); i++) {
		cout << "Name: " << i+1 << " " << names[i] << endl;
	}
	return 0;
}

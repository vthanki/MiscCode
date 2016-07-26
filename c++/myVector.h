#include <vector>
#include <string>
#include <iostream>

using namespace std;

class MyVector {
	private:
	vector<string> names;
	public:
	MyVector() {
		cout << "Constructor for MyVector" << endl;
	}
	void addName(string name);
	void printAllNames();
	int countChars();
	string getNameAt(int i);
	int size();
};

void MyVector::addName(string name) {
	cout << "Adding " << name << endl;
	names.push_back(name);
}

int MyVector::countChars() {
	int count = 0;
	for (int i = 0; i < names.size(); i++) {
		count += names[i].length();
	}
	return count;
}

void MyVector::printAllNames() {
	for (int i = 0; i < names.size(); i++) {
		cout << "Name: " << i+1 << " " << names[i] << endl;
	}
	cout << "Sizeof vector is " << sizeof(names) << endl;
}

string MyVector::getNameAt(int i) {
	return names[i];
}

int MyVector::size() {
	return names.size();
}

int countChars(MyVector vec);
vector<string> getAllNames(MyVector vec);

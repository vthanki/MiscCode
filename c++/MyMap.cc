#include <iostream>
#include <string>
#include <map>

using namespace std;

int main()
{
	map<string, int> pincode;
	string city = "porbandar";
	int pin = 360575;

	pincode[city] = pin;

	cout << "Pincode of " << city << " is " << pincode[city] << endl;
	return 0;
}

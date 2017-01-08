#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

const int NR_DIGITS = 100;

class BigInteger {
private:
	int arr[NR_DIGITS];
	short int len;

	void prependDigits(int digit);
	void shrink();

public:
	BigInteger(): len(0) {
		memset(arr, 0x0, sizeof(arr));
	}
	BigInteger(string digits, int base);
	BigInteger(int val);

	void show() const;
	short int getlen() const;

	string getStringValue() const;

	BigInteger operator +(BigInteger b) const;
	BigInteger operator -(BigInteger b) const;
	bool operator > (BigInteger b) const;
	bool operator < (BigInteger b) const;
	bool operator == (BigInteger b) const;
};

BigInteger::BigInteger(string digits, int base) {
	memset(arr, 0x0, sizeof(arr));
	len = digits.size();
	//TODO: Throw exception in case of unsupported base
	switch(base) {
	case 10:
		int i, j;
		for (i = len - 1,j = NR_DIGITS-1; i >= 0; i--, j--) {
			//TODO: Add sanity check for isDigit
			arr[j] = digits[i] - '0';
		}
		break;
	}
}

BigInteger::BigInteger(int value) {
	int i = NR_DIGITS - 1;
	len = 0;
	memset(arr, 0x0, sizeof(arr));
	do {
		arr[i] = value % 10;
		value /= 10;
		i--;
	} while (++len && value);
}

void BigInteger::prependDigits(int digit) {
	arr[NR_DIGITS-1-len] = digit;
	len++;
}

void BigInteger::shrink() {
	int lenToShrink = 0;
	for (int i = 0; i < NR_DIGITS && !(arr[i]); i++)
		lenToShrink++;

	len -= lenToShrink;
	if (!len) len = 1;
}

void BigInteger::show() const {
	for (int i = NR_DIGITS - len; i < NR_DIGITS; i++)
		if (arr[i] >= 0)
			cout << arr[i];
	cout << endl;
}

short int BigInteger::getlen() const {
	return len;
}


string BigInteger::getStringValue() const {
	string ret(len+1, 0);
	for (int i = 0; i < len; i++)
		ret[i] = '0' + arr[i];
	return ret;
}

bool BigInteger::operator >(BigInteger b) const {

	if (len > b.getlen())
		return true;

	if (len == b.getlen() && arr[NR_DIGITS - len] > b.arr[NR_DIGITS - len])
		return true;

	return false;
}

bool BigInteger::operator ==(BigInteger b) const {
	return getStringValue().compare(b.getStringValue()) == 0;
}

bool BigInteger::operator <(BigInteger b) const {
	return !(*this > b) && !(*this == b);
}

BigInteger BigInteger::operator +(BigInteger b) const {
	int alen = getlen(), blen = b.getlen();
	int max = alen > blen ? alen : blen;
	int carry = 0;
	BigInteger result;
	int i = 0;

	while (max > 0 || carry) {
		int sum = arr[NR_DIGITS-i-1] + b.arr[NR_DIGITS-i-1] + carry;
		int dig = sum % 10;
		carry = sum/10;
		result.prependDigits(dig);
		i++;
		max--;
	}

	return result;
}

BigInteger BigInteger::operator -(BigInteger b) const {
	BigInteger result;
	int dig, carry = 0;
	for (int i = NR_DIGITS - 1; i >= 0; i--) {
		if ((arr[i] + carry) < b.arr[i]) {
			dig = arr[i] + carry + 10 - b.arr[i];
			carry = -1;
		} else {
			dig = arr[i] + carry - b.arr[i];
			carry = 0;
		}
		result.prependDigits(dig);
	}
	result.shrink();
	return result;
}

int main(int argc, char *argv[]) {
	BigInteger b1(50);
	BigInteger b2 = 100;
	BigInteger b3 = b1 - b2;
	b3.show();
	return 0;
}

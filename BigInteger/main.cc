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
	bool isPositive;

	void prependDigits(int digit);
	void appendZeros(int count);
	void shrink();

public:
	BigInteger(): len(0), isPositive(true) {
		memset(arr, 0x0, sizeof(arr));
	}
	BigInteger(string digits, int base);
	BigInteger(int val);

	void show() const;
	short int getlen() const;

	BigInteger operator +(BigInteger b) const;
	BigInteger operator -(BigInteger b) const;
	BigInteger operator *(BigInteger b) const;
	bool operator > (BigInteger& b) const;
	bool operator < (BigInteger& b) const;
	bool operator == (BigInteger& b) const;

};

BigInteger::BigInteger(string digits, int base) {
	memset(arr, 0x0, sizeof(arr));
	isPositive = (digits[0] != '-');
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
	isPositive = (value >= 0);
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

void BigInteger::appendZeros(int count) {
	memcpy(&arr[NR_DIGITS-len-count], &arr[NR_DIGITS-len], len*sizeof(arr[0]));
	memset(&arr[NR_DIGITS-count], 0x0, count*sizeof(arr[0]));
	len += count;
}

void BigInteger::shrink() {
	int lenToShrink = 0;
	for (int i = 0; i < NR_DIGITS && !(arr[i]); i++)
		lenToShrink++;

	if (len > lenToShrink || lenToShrink == NR_DIGITS) {
		len -= lenToShrink;
		if (!len) len = 1;
	}
}

void BigInteger::show() const {
	if (!isPositive)
		cout << "-";
	for (int i = NR_DIGITS - len; i < NR_DIGITS; i++)
		if (arr[i] >= 0)
			cout << arr[i];
	cout << endl;
}

short int BigInteger::getlen() const {
	return len;
}

bool BigInteger::operator >(BigInteger& b) const {

	if (!this->isPositive && b.isPositive)
		return false;

	if (len > b.len)
		return true;

	if (len == b.len && arr[NR_DIGITS - len] > b.arr[NR_DIGITS - len])
		return true;

	return false;
}

bool BigInteger::operator ==(BigInteger& b) const {
	return (isPositive == b.isPositive) && !memcmp(arr, b.arr, sizeof(arr));
}

bool BigInteger::operator <(BigInteger& b) const {
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

	if (*this < b) {
		result = b - *this;
		result.isPositive = false;
	} else {
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
	}

	result.shrink();
	return result;
}

BigInteger BigInteger::operator *(BigInteger b) const {

	BigInteger result;
	BigInteger *intermediate = new BigInteger[b.len];
	int trailingZeros = b.len - 1;
	int intId = 0;
	for (int i = NR_DIGITS-b.len; i < NR_DIGITS ; i++) {
		int k = NR_DIGITS-1;
		int carry = 0;
		for (int j = NR_DIGITS-1; j >= NR_DIGITS-len || carry; j--) {
			int dig = (b.arr[i] * arr[j]) + carry;
			carry = dig / 10;
			intermediate[intId].arr[k] = dig%10;
			intermediate[intId].len++;

			k--;
		}
		intermediate[intId].appendZeros(trailingZeros);
		trailingZeros--;
		intId++;
	}

	for (int i = 0; i < b.len; i++) {
		result = result + intermediate[i];
	}

	return result;
}


int main(int argc, char *argv[]) {
	BigInteger b1("13395024444659582328972621742336",10), b2("13395024444659582328972621742336",10);
	BigInteger b3 = b1 * b2;
	b3.show();
	BigInteger b4 = 5, b5 = -5;
	cout << (b4 > b5);
	return 0;
}

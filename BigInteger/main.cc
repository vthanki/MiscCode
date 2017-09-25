#include <iostream>
#include <vector>
#include <string>
#include <cstring>
using namespace std;
const int NR_DIGITS = 500;

class BigInteger {
	private:
		char arr[NR_DIGITS];
		short int len;
		bool isPositive;

		void prependDigits(int digit);
		void appendZeros(int count);
		void shrink();

	public:
		class BigIntegerEx {
			string reason;
			public:
			BigIntegerEx(string s) : reason(s) {}
			string showReason() { return reason; }
		};


		BigInteger(): len(0), isPositive(true) {
			memset(arr, 0x0, sizeof(arr));
		}
		BigInteger(string digits, int base);
		BigInteger(int val);
		BigInteger(const BigInteger &b);

		void show() const;
		short int getlen() const;

		BigInteger operator +(BigInteger b) const;
		BigInteger operator -(BigInteger b) const;
		BigInteger operator *(BigInteger b) const;
		bool operator > (BigInteger b) const;
		bool operator > (int b) const;

		bool operator < (BigInteger b) const;
		bool operator < (int b) const;

		bool operator == (BigInteger b) const;

		friend ostream& operator << (ostream& os, const BigInteger &);

};

BigInteger::BigInteger(const BigInteger &b)
{
	memcpy(this->arr, b.arr, sizeof(b.arr));
	this->len = b.len;
	this->isPositive = b.isPositive;
}

BigInteger::BigInteger(string digits, int base) {

	if (digits.size() > NR_DIGITS) {
		throw BigIntegerEx("Number too large");
	}

	len = digits.size();

	memset(arr, 0x0, sizeof(arr));
	isPositive = (digits[0] != '-');

	switch(base) {
		case 10:
			int i, j;
			for (i = len - 1,j = NR_DIGITS-1; i >= 0; i--, j--) {
				if (digits[i] >= '0' && digits[i] <= '9')
					arr[j] = digits[i] - '0';
				else
					throw BigIntegerEx("Invalid digit encountered in number");
			}
			break;
		default:
			throw BigIntegerEx("Base not supported");
			break;
	}
}

BigInteger::BigInteger(int value) {
	int i = NR_DIGITS - 1;
	len = 0;
	memset(arr, 0x0, sizeof(arr));
	isPositive = (value >= 0);
	if (!isPositive)
		value = -value;
	do {
		arr[i] = value % 10;
		value /= 10;
		i--;
		/* TODO: Remove this limit by using vectors instead of an array */
		if (len > NR_DIGITS) {
			throw BigIntegerEx("Number too large");
		}
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

ostream& operator << (ostream& os, const BigInteger& b) {
	if (!b.isPositive)
		os << "-";
	for (int i = NR_DIGITS - b.len; i < NR_DIGITS; i++)
		if (b.arr[i] >= 0)
			os << static_cast<int>(b.arr[i]);
	return os;
}

bool BigInteger::operator >(BigInteger b) const {

	if (!this->isPositive && b.isPositive)
		return false;

	if (len > b.len)
		return true;

	if (len == b.len && arr[NR_DIGITS - len] > b.arr[NR_DIGITS - len])
		return true;

	return false;
}

bool BigInteger::operator >(int b) const {
	BigInteger bi(b);
	return *this > bi;
}

bool BigInteger::operator ==(BigInteger b) const {
	return (isPositive == b.isPositive) && !memcmp(arr, b.arr, sizeof(arr));
}

bool BigInteger::operator <(BigInteger b) const {
	return !(*this > b) && !(*this == b);
}

bool BigInteger::operator <(int b) const {
	BigInteger bi(b);
	return *this < bi;
}


BigInteger BigInteger::operator +(BigInteger b) const {
#if 0
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
#else
	BigInteger result;

	if (b < 0) {
		BigInteger temp = b;
		temp.isPositive = true;
		result = *this - temp;
	} else if (*this < 0) {
		BigInteger temp = *this;
		temp.isPositive = true;
		result = b - temp;
	} else {

		int carry = 0;
		int i = NR_DIGITS - 1;
		do {
			int dig = arr[i] + b.arr[i] + carry;
			result.arr[i] = dig % 10;
			carry = dig / 10;
			result.len++;
			i--;
		} while (carry || result.len < len || result.len < b.len);
	}

#endif
	return result;
}

BigInteger BigInteger::operator -(BigInteger b) const {

	BigInteger result;
#if 0
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
#else
	if (b < 0) {
		BigInteger temp = b;
		temp.isPositive = true;
		result = *this + temp;
	} else if  (*this < b) {
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
#endif
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
	delete intermediate;

	result.isPositive = isPositive && b.isPositive;

	return result;
}

BigInteger fact(BigInteger b) {
	BigInteger one(1);
	if (b == one)
		return one;

	return b*fact(b-one);
}

int main(int argc, char *argv[]) {
	try {
		BigInteger b1("13395024444659582328972621742336",12), b2("13395024444659582328972621742336",10);
		BigInteger b3 = b1 * b2;
		cout << b3 << endl;
		BigInteger b4 = 12, b5 = -4;
		cout << (b4 > 0) << endl;
		b3 = b4 * b5;
		cout << b3 << endl;
	} catch (BigInteger::BigIntegerEx& ex) {
		cout << ex.showReason();
	}

	return 0;
}

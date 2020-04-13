#include <cstring>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int NR_DIGITS = 100;

class BigInteger {
private:
    char arr[NR_DIGITS];
    short int len;
    bool isPositive;

    void prependDigits(int digit);
    void appendZeros(int count);
    void shrink();

public:
    BigInteger()
        : len(0)
        , isPositive(true)
    {
        memset(arr, 0x0, sizeof(arr));
    }
    BigInteger(string digits, int base);
    BigInteger(int val);

    void show() const;
    short int getlen() const;

    BigInteger operator+(BigInteger b) const;
    BigInteger operator-(BigInteger b) const;
    BigInteger operator*(BigInteger b) const;
    bool operator>(BigInteger& b) const;
    bool operator>(int b) const;

    bool operator<(BigInteger& b) const;
    bool operator<(int b) const;

    bool operator==(BigInteger& b) const;

    friend ostream& operator<<(ostream& os, const BigInteger&);
};
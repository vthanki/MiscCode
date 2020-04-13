#include "BigInteger.h"

int main(int argc, char* argv[])
{
    BigInteger b1("13395024444659582328972621742336", 10),
        b2("13395024444659582328972621742336", 10);
    BigInteger b3 = b1 * b2;
    cout << b3 << endl;
    BigInteger b4 = 12, b5 = -4;
    cout << (b4 > 0) << endl;
    b3 = b4 * b5;
    cout << b3 << endl;
    return 0;
}

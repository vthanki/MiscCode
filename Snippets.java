static int arr[] = new int[1000000];
static void preparePrimes(int N) {

	for (int j = 2; j < N; j++ ) {
		for (int i = j*2; i < 1000000; i+=j) {
			//if (i%j == 0)
			arr[i] = 1;
		}
	}
}

/* Required to prepare N primes by calling
 * preparePrimes() before using this method
 */
static boolean isPrimeFast(int N) {
	return arr[N] == 0;
}

public static void printNprimes(int n) {
	int i = 2;
	while (n > 0) {
		if (isPrimeFast(i)) {
			System.out.println(i);
			n--;
		}
		i++;
	}
}

public static void printGDC(int a, int b) {
int max = a > b ? a:b;
int gcd = 1;
for (int i = 2; i < max; i++) {
    if (a % i == 0 && b % i == 0) {
        gcd *= i;
        a /= i; b /= i;
        i--;
    }
}
//System.out.println(gcd);
}

public static int consecutiveFibonacciSum(int a, int b) {
	int c=0,d=1,ans = 0, sum = 0;
	for (int i = 1; i < b; i++) {
	    System.out.println(d);
	    if (i >= a)
	        ans+= d;
	    sum = c + d;
	    c = d;
	    d = sum;
	
	}
	return ans;
}

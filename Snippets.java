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

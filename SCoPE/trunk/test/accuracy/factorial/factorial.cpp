/**
 * \file factorial.cpp
 * \brief Recursive factorial algorithm
 */

int factorial(int n) {
	if (n == 1 || n == 0) {
		return 1;
	}
	return (n * factorial(n-1));
}

int uc_main(void) {
	int i, f;
 	for (i=0 ; i<500 ; i++) {
		f = factorial(i);
 	}
}


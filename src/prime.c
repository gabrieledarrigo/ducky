#include <math.h>
#include "prime.h"

/**
 * Check if the integer n is a prime number.
 *
 * @param n     the number to check
 * @return      1 if n is a prime number, 0 otherwise
 */
int is_prime(int n) {
    if (n <= 1) {
        return 0;
    }

    if (n == 2) {
        return 1;
    }

    if (n % 2 == 0) {
        return 0;
    }

    for (int i = 2; i < n; i++) {
        if ((n % i) == 0) {
            return 0;
        }
    }

    return 1;
}

/**
 * Compute the next prime number after n.
 *
 * @param n     the integer from which the search for the next prime number starts
 * @return      the next prime after n
 */
int next_prime(int n) {
    while (is_prime(n) != 1) {
        n++;
    }

    return n;
}
#include <math.h>
#include "prime.h"

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

int next_prime(int x) {
    while (is_prime(x) != 1) {
        x++;
    }

    return x;
}
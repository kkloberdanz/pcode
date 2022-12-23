#include <stdio.h>

#include "math.h"

double abs_f64(double x) {
    return (x < 0) ? -x : x;
}

u64 sqrt_u64(u64 n) {
    u64 x = n;
    u64 old_x;
    do {
        old_x = x;
        x = (x + (n / x)) >> 1;
    } while (old_x != x);
    return x;
}

double sqrt_f64(double n) {
    double x = n;
    double old_x;
    do {
        old_x = x;
        x = (x + (n / x)) / 2;
    } while (abs_f64(old_x - x) > 1e-300);
    return x;
}

#ifdef VM_MATH_TEST

#define ASSERT(X)                                                             \
    do {                                                                      \
        if (!(X)) {                                                           \
            fprintf(                                                          \
                stderr,                                                       \
                "assertion failed! (" #X ") file: %s line: %d\n",             \
                __FILE__,                                                     \
                __LINE__                                                      \
            );                                                                \
        }                                                                     \
    } while (0)

int main(void) {
    ASSERT(sqrt_u64(9) == 3);
    ASSERT(sqrt_u64(17) == 4);
    ASSERT(sqrt_u64(12309) == 110);
    ASSERT(sqrt_u64(123097773) == 11094);
    ASSERT(sqrt_u64(99789123097773) == 9989450);

    printf("%g\n", sqrt_f64(9.0));
    printf("%g\n", sqrt_f64(190.0));
    printf("%g\n", sqrt_f64(1e100));
    printf("%g\n", sqrt_f64(1e-100));
    return 0;
}
#endif /* VM_MATH_TEST */

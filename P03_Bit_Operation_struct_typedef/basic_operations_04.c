#include <stdio.h>
#include <stdlib.h>

// Powers of 2 have only one bit set, so if you subtract 1 from a power of 2
// you get a number with all bits set up to the bit of the power of 2.
int is_power(int number) {
    return (number & (number - 1)) == 0;
}

int main() {
    for (int i = 1; i <= 70; ++i) {
        if (i > 0 && is_power(i)) {
            printf("%d is a power of 2\n", i);
        } else {
            printf("--\n");
        }
    }

    return EXIT_SUCCESS;
}
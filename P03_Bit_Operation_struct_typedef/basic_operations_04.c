#include <stdio.h>
#include <stdlib.h>

int is_power(int number) { return (number & (number - 1)) == 0; }

int main() {
    for (int i = 1; i < 100; ++i) {
        if (i > 0 && is_power(i)) {
            printf("%d is a power of 2\n", i);
        } else {
            printf("Nope\n");
        }
    }

    return EXIT_SUCCESS;
}
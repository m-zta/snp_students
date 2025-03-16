#include <stdio.h>
#include <stdlib.h>

int main() {
    unsigned int number = 0x75; // Corresponds to 0111 0101
    unsigned int bit    = 3;    // bit at position 3

    printf("Initial number = 0x%02X\n", number);

    // Setting bit 3: 0111 0101 -> 0111 1101
    number = number | (1 << bit);
    printf("After setting bit %d, number = 0x%02X\n", bit, number);

    // Clearing bit 1: 0111 1101 -> 0111 1101
    bit    = 1;
    number = number & ~(1 << bit);
    printf("After clearing bit %d, number = 0x%02X\n", bit, number);

    // Toggling bit 0: 0111 1101 -> 0111 1100
    bit    = 0;
    number = number ^ (1 << bit);
    printf("After toggling bit %d, number = 0x%02X\n", bit, number);

    return EXIT_SUCCESS;
}
#include <stdio.h>
#include <stdlib.h>

// Helper function from bin_calculator.c
void print_binary(unsigned int value) {
    for (int i = 8 - 1; i >= 0; i--) {
        printf("%d", (value >> i) & 1);

        if (i % 8 == 0) {
            printf("'");
        }
    }

    printf("\n");
}

int main() {
    unsigned int number = 0x75; // Corresponds to 0111 0101
    unsigned int bit    = 3;    // bit at position 3

    printf("Initial number = 0x%02X\n", number);
    print_binary(number);
    printf("\n");

    // Setting bit 3: 0111 0101 -> 0111 1101
    number = number | (1 << bit);
    printf("Setting bit %d, number = 0x%02X\n", bit, number);
    print_binary(number);
    printf("\n");

    // Clearing bit 1: 0111 1101 -> 0111 1101
    bit    = 1;
    number = number & ~(1 << bit);
    printf("Clearing bit %d, number = 0x%02X\n", bit, number);
    print_binary(number);
    printf("\n");

    // Toggling bit 0: 0111 1101 -> 0111 1100
    bit    = 0;
    number = number ^ (1 << bit);
    printf("Toggling bit %d, number = 0x%02X\n", bit, number);
    print_binary(number);

    return EXIT_SUCCESS;
}
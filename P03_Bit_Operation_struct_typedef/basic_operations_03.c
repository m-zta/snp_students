#include <stdio.h>
#include <stdlib.h>

int main() {
    char  word[8] = "sREedEv";
    char* wordptr = &word[0];

    // ASCII value of
    // '_' is 95 (binary: 0101 1111)
    // ' ' is 32 (binary: 0010 0000)
    // 's' is 115 (binary: 0111 0011)
    // 'S' is 83 (binary: 0101 0011)
    // If you check an ASCII table with binary values like on
    // http://sticksandstones.kstrom.com/appen.html
    // you'll notice that the ASCII table is structured such that the only
    // difference (in binary) between upper and lower case alphabet characters
    // is bit 6, so '0' means upper case and '1' means lower case. Previously we
    // established that you can clear a bit with the '&' operator and set a bit
    // with the '|' operator, which is exactly what we need for conversion to
    // upper/lower case.

    while (wordptr < &word[7]) {
        printf("UPPERCASE: %c\n",
               *wordptr & '_'); // converts the char into uppercase regardless
                                // of the current casing
        printf("LOWERCASE: %c\n",
               *wordptr | ' '); // converts the char into lowercase regardless
                                // of the current casing
        wordptr++;
    }

    return EXIT_SUCCESS;
}
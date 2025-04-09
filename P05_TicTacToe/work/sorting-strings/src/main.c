#include "sort_words.h"

#include <stdio.h>
#include <stdlib.h>

// --- Main Function ---
int main(void) {
    char unique_words[MAX_WORDS][MAX_WORD_LENGTH + 1];
    int  unique_word_count = 0;

    printf("Enter up to %d unique words (max %d chars each).\n", MAX_WORDS,
           MAX_WORD_LENGTH);
    printf("Enter '%s' to finish early.\n", TERMINATOR);

    // Read words from user input
    read_words(unique_words, &unique_word_count);

    if (unique_word_count > 0) {
        sort_words(unique_words, unique_word_count);
        print_words(unique_words, unique_word_count);
    } else {
        printf("\nNo words were entered.\n");
    }

    return EXIT_SUCCESS;
}

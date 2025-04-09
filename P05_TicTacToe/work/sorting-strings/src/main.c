#include "sort_words.h" // Include the header for our sorting module

#include <stdio.h>  // For printf
#include <stdlib.h> // For EXIT_SUCCESS

// --- Main Function ---
int main(void) { // <<< Changed () to (void)
    // Use constants defined in sort_words.h
    char unique_words[MAX_WORDS][MAX_WORD_LENGTH + 1];
    int  unique_word_count = 0;

    printf("Enter up to %d unique words (max %d chars each).\n", MAX_WORDS,
           MAX_WORD_LENGTH);
    printf("Enter '%s' to finish early.\n", TERMINATOR);

    // Call functions defined in sort_words.c (via sort_words.h)
    read_words(unique_words, &unique_word_count);

    if (unique_word_count > 0) {
        sort_words(unique_words, unique_word_count);
        print_words(unique_words, unique_word_count);
    } else {
        printf("\nNo words were entered.\n");
    }

    return EXIT_SUCCESS;
}

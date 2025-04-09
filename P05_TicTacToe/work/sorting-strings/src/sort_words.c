#include "sort_words.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

// --- Function Implementations ---

void to_uppercase(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char) str[i]);
    }
}

int is_duplicate(const char words[][MAX_WORD_LENGTH + 1], int count,
                 const char* new_word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], new_word) == 0) {
            return 1; // Found a duplicate
        }
    }

    return 0;
}

void read_words(char words[][MAX_WORD_LENGTH + 1], int* word_count) {
    char input_buffer[MAX_WORD_LENGTH + 1];

    while (*word_count < MAX_WORDS) {
        printf("%d> ", *word_count + 1);

        // Read input from user
        if (scanf("%20s", input_buffer) != 1) {
            // Consume rest of the line
            while (getchar() != '\n' && !feof(stdin))
                ;

            fprintf(stderr, "Error reading input. Stopping.\n");
            break;
        };

        // Check if the user wants to terminate the input
        if (strcmp(input_buffer, TERMINATOR) == 0) {
            printf("Termination signal '%s' received.\n", TERMINATOR);
            break;
        }

        // Convert the input to uppercase
        to_uppercase(input_buffer);

        // Check if the word is a duplicate
        if (!is_duplicate(words, *word_count, input_buffer)) {
            strcpy(words[*word_count], input_buffer);
            (*word_count)++;
        } else {
            printf("'%s' is a duplicate, ignoring.\n", input_buffer);
        }

        // Consume rest of line
        while (getchar() != '\n' && !feof(stdin))
            ;
    }

    if (*word_count == MAX_WORDS) {
        printf("Maximum number of unique words (%d) reached.\n", MAX_WORDS);
    }
}

void sort_words(char words[][MAX_WORD_LENGTH + 1], int word_count) {
    char temp[MAX_WORD_LENGTH + 1];
    int  swapped;

    if (word_count < 2) {
        return;
    }

    // Bubble sort
    for (int i = 0; i < word_count - 1; i++) {
        swapped = 0;

        for (int j = 0; j < word_count - 1 - i; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                strcpy(temp, words[j]);
                strcpy(words[j], words[j + 1]);
                strcpy(words[j + 1], temp);
                swapped = 1;
            }
        }

        if (!swapped) {
            break;
        }
    }
}

void print_words(const char words[][MAX_WORD_LENGTH + 1], int word_count) {
    printf("\n===== Sorted Unique Words =====\n");

    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }

    printf("=============================\n");
}

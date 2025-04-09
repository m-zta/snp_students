#ifndef SORT_WORDS_H_
#define SORT_WORDS_H_

// --- Constants ---
#define MAX_WORDS 10
#define MAX_WORD_LENGTH 20
#define TERMINATOR "ZZZ"

// --- Function Prototypes ---

/**
 * @brief Converts a given string to uppercase in place.
 * @param str The string to convert.
 */
void to_uppercase(char* str);

/**
 * @brief Checks if a new word already exists in the array of words.
 * Comparison is case-insensitive as it assumes new_word is already uppercase.
 * @param words The array of already stored unique words.
 * @param count The current number of words stored in the array.
 * @param new_word The new word (already uppercase) to check for duplicates.
 * @return 1 if the word is a duplicate, 0 otherwise.
 */
int is_duplicate(const char words[][MAX_WORD_LENGTH + 1], int count,
                 const char* new_word);

/**
 * @brief Reads words from standard input, converts them to uppercase,
 * checks for duplicates, and stores unique words in the array.
 * @param words The 2D array to store the unique words.
 * @param word_count Pointer to the integer tracking the count of unique words.
 */
void read_words(char words[][MAX_WORD_LENGTH + 1], int* word_count);

/**
 * @brief Sorts the array of words alphabetically using a simple Bubble Sort.
 * @param words The 2D array of words to sort.
 * @param word_count The number of words in the array.
 */
void sort_words(char words[][MAX_WORD_LENGTH + 1], int word_count);

/**
 * @brief Prints the sorted words to the console.
 * @param words The sorted 2D array of words.
 * @param word_count The number of words in the array.
 */
void print_words(const char words[][MAX_WORD_LENGTH + 1], int word_count);

#endif // SORT_WORDS_H_

/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur             -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences)           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 */
/**
 * @file
 * @brief Lab implementation
 */
// begin students to add code for task 4.1
#include "read.h"

#include <stdio.h>

/**
 * @brief Read an unsigned int from stdin
 *
 * This implementation replicates the functionality of read.java.
 * It reads a line from standard input and converts it to an integer.
 */
int getInt(int max_result) {
    // Constants for character comparison
    const int EOF_CHAR      = EOF;
    const int EOL_CHAR      = '\n';
    const int ASCII_SPACE   = ' ';
    const int ASCII_DIGIT_0 = '0';
    const int ASCII_DIGIT_9 = '9';

    // Constants for buffer handling
    const int BUFFERSIZE = 10;
    const int NO_POS     = -1;

    // Buffer for reading input
    char buffer[BUFFERSIZE];

    int result = 0;

    // Read line: up to EOL or EOF
    int bytes = 0;
    int input = getchar();
    while ((input != EOL_CHAR) && (input != EOF_CHAR)) {
        if (bytes < BUFFERSIZE) { // only buffer first n characters
            buffer[bytes] = (char) input;
            bytes++;
        } else {
            result = PARSE_ERROR; // exceed buffer size, continue read line
        }
        input = getchar();
    }

    if (input == EOF_CHAR) {
        return READ_ERROR;
    }

    // Check for numbers: skip leading and trailing spaces
    int pos = 0;
    while ((pos < bytes) && (buffer[pos] <= ASCII_SPACE))
        pos++; // skip spaces

    int posOfFirstDigit = pos;
    int posOfLastDigit  = NO_POS;

    while ((pos < bytes) && (buffer[pos] >= ASCII_DIGIT_0) &&
           (buffer[pos] <= ASCII_DIGIT_9)) {
        posOfLastDigit = pos;
        pos++;
    }

    while ((pos < bytes) && (buffer[pos] <= ASCII_SPACE))
        pos++; // skip spaces

    // Produce return value
    if (result != 0) {
        // Previously detected read or parse error
        return result;
    } else if ((pos != bytes) || (posOfLastDigit == NO_POS)) {
        return PARSE_ERROR;
    } else { // Convert number
        result = 0;
        for (int i = posOfFirstDigit; i <= posOfLastDigit; i++) {
            result = result * 10 + (buffer[i] - ASCII_DIGIT_0);
            if (result > max_result) {
                return PARSE_ERROR;
            }
        }
    }

    return result;
}
// end students to add code

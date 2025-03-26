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
 * @brief Interface for reading integers from standard input
 */

#ifndef READ_H_
#define READ_H_

/**
 * @brief Error codes for getInt function
 */
#define PARSE_ERROR -1 // Error parsing input
#define READ_ERROR -2  // Error reading input

/**
 * @brief Reads an unsigned integer from standard input
 *
 * Reads a line of text from stdin and converts it to an integer.
 * The line may only contain one number with optional whitespace before/after.
 *
 * @param max_result Maximum allowed value for the result
 * @return The parsed integer, or PARSE_ERROR/READ_ERROR on failure
 */
int getInt(int max_result);

#endif // READ_H_

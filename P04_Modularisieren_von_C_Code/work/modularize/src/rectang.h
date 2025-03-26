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
 * @brief Interface for rectangle validation functions
 */

#ifndef RECTANG_H_
#define RECTANG_H_

/**
 * @brief Checks if a triangle with given sides is rectangular
 *
 * @param a First side of the triangle
 * @param b Second side of the triangle
 * @param c Third side of the triangle
 * @return 1 if triangle is rectangular, 0 otherwise
 */
int isRectangular(int a, int b, int c);

#endif // RECTANG_H_

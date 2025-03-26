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
#include "rectang.h"

/**
 * @brief Checks if a triangle with given sides is rectangular
 *
 * This implementation matches the functionality of rectang.java.
 * It determines if a triangle with the given side lengths is right-angled.
 */
int isRectangular(int a, int b, int c) {
    // Calculate squared values
    int aSquared = a * a;
    int bSquared = b * b;
    int cSquared = c * c;

    // Check if triangle is rectangular
    int isRightAngled = 0;

    if ((a == 0) && (b == 0) && (c == 0)) {
        isRightAngled = 0;
    } else if ((aSquared + bSquared) == cSquared) {
        isRightAngled = 1;
    } else if ((aSquared + cSquared) == bSquared) {
        isRightAngled = 1;
    } else if ((bSquared + cSquared) == aSquared) {
        isRightAngled = 1;
    } else {
        isRightAngled = 0;
    }

    return isRightAngled;
}
// end students to add code

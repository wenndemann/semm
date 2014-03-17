/**
 *  some useful tools.
 *  @file tools.h
 *  @author Stefan Wenn
 *  @date 04.11.2013
 *
 *  @version 0.2
 *  comments added (Doxygen).
 *
 *  @version 0.1
 *  first release
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <stdio.h>
#include <stdarg.h>

/**
 * prints a text to stdout by a defined amount of leading spaces.
 * @param offset amount of leading spaces
 * @param format a printf like string
 * @return the return value of printf
 */
int oprintf(int offset, const char *format, ...);

#endif /* TOOLS_H_ */

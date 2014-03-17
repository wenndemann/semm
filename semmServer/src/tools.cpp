/**
 *  some useful tools.
 *  @file tools.cpp
 *  @author Stefan Wenn
 *  @date 04.11.2013
 *
 *  @version 0.2
 *  comments added (Doxygen).
 *
 *  @version 0.1
 *  first release
 */

#include "tools.h"

int oprintf(int offset, const char *format, ...) {
	int rtn;
	va_list list;
    va_start(list, format);
	printf("%*s", offset, "");
	rtn = printf(format, list);
	va_end(list);
	return rtn;
}

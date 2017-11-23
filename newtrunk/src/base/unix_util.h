
#ifndef UNIX_UTIL_H
#define UNIX_UTIL_H

#include <algorithm>
#include <string>

#ifdef MINGW
extern void _itoa(int    n,
                  char * s,
                  int    maxcount);
#endif

#ifdef unix
#ifndef __MINGW32__
extern void _itoa(int    n,
                  char * s,
                  int    maxcount);
#endif
#endif

#ifdef CYGWIN
extern void itoa(int    n,
                 char * s,
                 int    maxcount);
#endif

#ifdef VS2008
double round(double aVal);
#endif

#endif // UNIX_UTIL_H



#ifndef UNIX_UTIL_H
#define UNIX_UTIL_H
#include <algorithm>  
#ifdef MINGW
extern void _itoa( int n, char* s, int maxcount );
	#endif
extern void reverse( char* s );
	#ifdef unix
extern void _itoa( int n, char* s, int maxcount );
	#endif
#ifdef CYGWIN
extern void itoa( int n, char* s, int maxcount );
#endif
#ifdef VS2008
	double round(double aVal);
#endif
#endif // UNIX_UTIL_H

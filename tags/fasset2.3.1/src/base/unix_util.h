
#ifndef UNIX_UTIL_H
#define UNIX_UTIL_H

extern void reverse( char* s );
	#ifdef UNIX
	extern void itoa( int n, char* s, int maxcount );
	#endif
#ifdef CYGWIN
extern void itoa( int n, char* s, int maxcount );
#endif
#ifdef VS2008
	int round(double aVal);
#endif
#endif // UNIX_UTIL_H

/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
//
// unix_util.h
//

#ifndef UNIX_UTIL_H
#define UNIX_UTIL_H

extern void reverse( char* s );
	#ifdef UNIX
	extern void itoa( int n, char* s, int maxcount );
	#endif
#ifdef CYGWIN
extern void itoa( int n, char* s, int maxcount );
#endif
//extern int  stricmp( const char *s1, const char *s2);
#endif // UNIX_UTIL_H

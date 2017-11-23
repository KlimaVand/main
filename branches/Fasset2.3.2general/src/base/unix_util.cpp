
#include <string.h>

#include "unix_util.h"
#include "math.h"



void reverse( char* s )
{
  int c, i, j;

  for ( i=0, j=strlen(s)-1; i<j; i++, j-- ) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}



void itoa( int n, char* s, int maxcount )
{
  int i=0, sign;

  if ((sign = n) < 0 )
    n = -n;

  // Make room for 'end-of-string' marker.
  maxcount--;

  if ( sign<0 )
    // Make room for sign char.
    maxcount--;

  while ( maxcount-- > 0 ) {
    s[i++] = n % 10 + '0';
    if ( (n /= 10) <= 0 )
      break;
  }

  if ( sign<0 )
    s[i++] = '-';

  s[i] = '\0';
  reverse(s);
}

#ifdef VS2008
int round(double aVal)
{
	//middle value point test
	if (ceil(aVal+0.5) == floor(aVal+0.5))
	{
		int a = (int)ceil(aVal);
		if (a%2 == 0)
		{return ceil(aVal);}
		else
		{return floor(aVal);}
	}

	else return floor(aVal+0.5);
};
#endif

//int stricmp( const char *s1, const char *s2)
//{
//  char *cps1 = new char[ sizeof(char) * strlen(s1) ];
//  char *cps2 = new char[ sizeof(char) * strlen(s2) ];
//
//  strcpy( cps1, s1 );
//  strcpy( cps2, s2 );
//
//  for ( unsigned int i=0; i<strlen(cps1)-1; i++ ) {
//    cps1[i] = tolower( cps1[i] );
//  }
//  for ( unsigned int i=0; i<strlen(cps2)-1; i++ ) {
//    cps2[i] = tolower( cps2[i] );
//  }
//
//  int res = strcmp( cps1, cps2 );
//
//  delete cps1;
//  delete cps2;
//
//  return res;
//}




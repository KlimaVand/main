
#include <string.h>
#include "unix_util.h"
#include "math.h"

#ifndef VS2008

#ifndef __MINGW32__


void reverse(char * s) { // Kernighan and Ritchie
      int length = strlen(s) ;
      int c, i, j;

      for (i = 0, j = length - 1; i < j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
      }
}


void _itoa(int    n,
           char * s,
           int    maxcount) {
    int i = 0, sign;

    if ((sign = n) < 0) {
        n = -n;
    }

    // Make room for 'end-of-string' marker.
    maxcount--;

    if (sign < 0) {

        // Make room for sign char.
        maxcount--;
    }

    while (maxcount-- > 0) {
        s[i++] = n % 10 + '0';

        if ((n /= 10) <= 0) {
            break;
        }
    }

    if (sign < 0) {
        s[i++] = '-';
    }

    s[i] = '\0';

    reverse(s);
}
#endif

#endif

#ifdef VS2008
double round(double aVal) {

    // middle value point test
    if (ceil(aVal + 0.5) == floor(aVal + 0.5)) {
        int a = (int) ceil(aVal);

        if (a % 2 == 0) {
            return ceil(aVal);
        } else {
            return floor(aVal);
        }
    } else {
        return floor(aVal + 0.5);
    }
}
#endif


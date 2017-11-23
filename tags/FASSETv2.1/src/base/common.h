/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __ANSICPP__
// **FN**

   #include <windows.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <iostream.h>
   #include <iomanip.h>
   #include <fstream.h>
   #include <conio.h>
   #include <dir.h>
   #include <math.h>
   #include <base.h>
   #include <linklist.h>
   #include <clonlist.h>
   #include <assert.h>
   //#define DEVCPP
   #ifdef DEVCPP
       #include <sstream>
       #include <cstring>
       using namespace std;
	#else
      #include <strstream>
   #endif
#else
   #define MAXPATH 1024
   #include <stdlib.h>
   #include <string>
   #include <iostream>
   #include <iomanip>
   #include <fstream>
   #include <math.h>
   #ifdef BUILDER
		#include <dir.h>
		#include <conio.h>
   #else
//	   #include <unistd.h>
   #endif
   #include <linklist.h>
#ifndef __BCplusplus__
   #ifndef BUILDER
	   #include "unix_util.h"
   #endif
#endif
   using namespace std;

#endif // __ANSICPP__


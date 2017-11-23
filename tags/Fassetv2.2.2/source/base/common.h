/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifdef __BCplusplus__
// **FN**

   #include <windows.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <iostream>
   #include <iomanip>
   #include <fstream>
   #include <conio.h>
   #include <dir.h>
   #include <math>
   #include "base.h"
   #include "linklist.h"
   #include "cloneList.h"
   #include <assert>
   //#define DEVCPP
   #ifdef DEVCPP
       #include <sstream>
       #include <cstring>
       using namespace std;
	#else
      #include <strstream>
      #include <string>
   #endif
#else
   #define MAXPATH 1024
   #include <stdlib.h>
   #include <string>
   #include <iostream>
   #include <iomanip>
   #include <fstream>
   #include <math.h>
   #ifdef VS2008
		#include <direct.h>
   #else
		#include <unistd.h>

   #endif
   #include "linklist.h"
#ifndef __BCplusplus__
   #ifndef BUILDER
	   #include "unix_util.h"
   #endif
#endif
   using namespace std;

#endif // __ANSICPP__
#include <string>
#include <stdlib.h>
#include "../tools/compare.h"
using namespace std;

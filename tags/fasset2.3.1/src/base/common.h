/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifdef __BCplusplus__
   #include <stdio.h>
   #include <iomanip>
   #include <conio.h>
   #include <dir.h>
   #include <strstream>

#else
   #include <iomanip>
   #include <sstream>
   #ifdef VS2008
		#include <direct.h>
   #else
		#include <unistd.h>
   #endif
#endif // __ANSICPP__
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <vector>
using namespace std;

/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef CRROPSWHTH
   #define CRROPSWHTH
#include "cropSbarley.h"

class cropSpringWheat:public cropSbarley
{
public:
	cropSpringWheat(const char * aName, const int aIndex, const base * aOwner, string cropName);
};

#endif

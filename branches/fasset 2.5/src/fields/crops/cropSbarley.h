/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "crop.h"

#ifndef CROPSBRL
  #define CROPSBRL
class cropSbarley:public crop
{
public:
	cropSbarley(const char * aName, const int aIndex, const base * aOwner, string cropName);
};

#endif

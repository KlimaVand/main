/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef cropCrimsonClove_H
   #define cropCrimsonClove_H
#include "cropClover.h"

class cropCrimsonClover:public cropClover
{
public:
	cropCrimsonClover(const char * aName, const int aIndex, const base * aOwner, string cropName);
};
#endif
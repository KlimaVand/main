/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef cropFabaBean_H
   #define cropFabaBean_H

#include "cropPea.h"
#include "phenology.h"

class cropFabaBean:public cropPea
{

public:
	cropFabaBean(const char * aName, const int aIndex, const base * aOwner);

};

#endif

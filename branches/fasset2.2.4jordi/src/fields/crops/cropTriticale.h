/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef cropTriticale_h
   #define cropTriticale_h

#include "../../soil/soil.h"
#include "cropWwheat.h"

#include "crop.h"
#include "phenology.h"

class cropTriticale:public cropWwheat
{
public:
	cropTriticale(const char * aName, const int aIndex, const base * aOwner);

};

#endif


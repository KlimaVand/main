/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "crop.h"
#include "cropmaize2.h"

#ifndef CROPMAIZE
#define CROPMAIZE

class cropMaize:public cropMaize2
{
public:
	cropMaize(const char * aName, const int aIndex, const base * aOwner);
//   double fTW();
   virtual bool ReadyForHarvest() {return Phenology->DS>1.0;}; // BMP modified 01.03.2007
};
#endif

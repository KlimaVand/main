/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "crop.h"
#ifndef CROPMAIZE
	#define CROPMAIZE

class cropMaize:public crop
{
public:
	cropMaize(const char * aName, const int aIndex, const base * aOwner, string cropName);
//   double fTW();
	   virtual void Cut(plantItem * cutPlantMaterial, double cut_height);
   virtual bool ReadyForHarvest() {return Phenology->DS>1.0;}; // BMP modified 01.03.2007
};
#endif

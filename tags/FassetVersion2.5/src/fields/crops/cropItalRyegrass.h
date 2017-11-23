/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "cropSpringWheat.h"

class cropItalRyegrass:public cropSpringWheat
{
public:
	cropItalRyegrass(const char * aName, const int aIndex, const base * aOwner, string cropName);
   void Cut(plantItem * cutPlantMaterial, double cut_height);
};


/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "cropSunflower.h"

cropSunflower::cropSunflower(const char * aName, const int aIndex, const base * aOwner)
          :cropMaize(aName,aIndex,aOwner)
{
theMessage->FatalError("cropSunflower:: this class has not been fully tested. Delete this line to test");
	// Parameters
   CropName   = "Sunflower";
   PlantItemName = "SUNFLOWER";           // These names map to products.dat !
   StrawItemName = "SUNFLOWERSTRAW";      // These names map to products.dat !
}


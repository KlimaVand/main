/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropSrape.h"

cropSrape::cropSrape(const char * aName, const int aIndex, const base * aOwner, string cropName)
          :crop(aName,aIndex,aOwner, cropName)
{
	// Parameters (Estimated from winter rape)

   PlantItemName = "SPRAPE";
   StrawItemName = "RAPESTRAW";
   WinterSeed = false;


}


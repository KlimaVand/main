/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

#include "../../base/common.h"
#include "cropCrimsonClover.h"

cropCrimsonClover::cropCrimsonClover(const char * aName, const int aIndex, const base * aOwner, string cropName)
          :cropClover(aName,aIndex,aOwner, cropName)
{
	// Parameters
   CropName   = "CrimsonClover";
   PlantItemName = "CRCLOVER";                   // These names maps to products.dat !

	
}


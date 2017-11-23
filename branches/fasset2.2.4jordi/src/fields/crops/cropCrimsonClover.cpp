/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

#include "../../base/common.h"
#include "cropCrimsonClover.h"

cropCrimsonClover::cropCrimsonClover(const char * aName, const int aIndex, const base * aOwner)
          :cropClover(aName,aIndex,aOwner)
{
	// Parameters
   CropName   = "CrimsonClover";
   PlantItemName = "CRCLOVER";                   // These names maps to products.dat !

	
}


/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropWrape.h"

cropWrape::cropWrape(const char * aName, const int aIndex, const base * aOwner, string cropName)
          :crop(aName,aIndex,aOwner, cropName)
{
	// Parameters

   PlantItemName = "WNRPE";                   // These names maps to products.dat !
   StrawItemName = "RAPESTRAW";              // These names maps to products.dat !
	WinterSeed = true;

  }


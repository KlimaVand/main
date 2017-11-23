/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropOat.h"
#include "../../base/bstime.h"

/****************************************************************************\
\****************************************************************************/
cropOat::cropOat(const char * aName, const int aIndex, const base * aOwner, string cropName)
        :crop(aName,aIndex,aOwner, cropName)
{
	// For nomenclature see base class 'crop'.

   PlantItemName = "OAT";                 // These names maps to products.dat !
   StrawItemName = "OATSTRAW";            // These names maps to products.dat !
	WinterSeed = false;
   // Parameters

}

/****************************************************************************\
\****************************************************************************/
double cropOat::Update(double ActivePar)
{
	return crop::Update(ActivePar);
}


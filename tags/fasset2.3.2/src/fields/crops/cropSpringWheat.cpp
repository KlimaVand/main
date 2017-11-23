/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropSpringWheat.h"
#include "../../products/products.h"
/****************************************************************************\
\****************************************************************************/

cropSpringWheat::cropSpringWheat(const char * aName, const int aIndex, const base * aOwner, string cropName)
                :cropSbarley(aName,aIndex,aOwner, cropName)
{

	PlantItemName = "SPWHT";                   // These names maps to products.dat !
   StrawItemName = "WHEATSTRAW";              // These names maps to products.dat !


}



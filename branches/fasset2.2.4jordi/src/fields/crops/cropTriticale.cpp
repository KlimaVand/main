/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropTriticale.h"

cropTriticale::cropTriticale(const char * aName, const int aIndex, const base * aOwner)
         :cropWwheat(aName,aIndex,aOwner)
{
	CropName = "Triticale";
   PlantItemName = "Triticale";                   // These names maps to products.dat !
   StrawItemName = "TRITSTRAW";              // These names maps to products.dat !
}

/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropSoybean.h"

cropSoybean::cropSoybean(const char * aName, const int aIndex, const base * aOwner,string cropName)
        :cropPea(aName,aIndex,aOwner, cropName)
{
theMessage->FatalError("cropSoybean:: this class has not been fully tested. Delete this line to test");
   // Basal parameters
   CropName = "Soybean";
   PlantItemName = "SOYBEAN";            // These names maps to products.dat !
   StrawItemName = "SOYBEANSTRAW";       // These names maps to products.dat !
}



/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "cropsoybean.h"

cropSoybean::cropSoybean(const char * aName, const int aIndex, const base * aOwner)
        :cropPea(aName,aIndex,aOwner)
{
theMessage->FatalError("cropSoybean:: this class has not been fully tested. Delete this line to test");
   // Basal parameters
   CropName = "Soybean";
   PlantItemName = "SOYBEAN";            // These names maps to products.dat !
   StrawItemName = "SOYBEANSTRAW";       // These names maps to products.dat !
}



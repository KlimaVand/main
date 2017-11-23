#include "../../base/common.h"
#include "cropWbarley.h"

cropWbarley::cropWbarley(const char * aName, const int aIndex, const base * aOwner, string cropName)
            :cropSbarley(aName,aIndex,aOwner, cropName)
{

   PlantItemName = "WNBRL";                 // These names maps to products.dat !
   StrawItemName = "BARLEYSTRAW";           // These names maps to products.dat !
   WinterSeed = true;

}




#include "../../base/common.h"
#include "cropSbarley.h"
#include "../../base/bstime.h"

cropSbarley::cropSbarley(string       aName,
                         const int    aIndex,
                         const base * aOwner,
                         string       cropName):
    crop(aName,
         aIndex,
         aOwner,
         cropName)
{

    // For nomenclature see base class 'crop'.
    PlantItemName = "BARLEY";         // These names maps to products.dat !
    StrawItemName = "BARLEYSTRAW";    // These names maps to products.dat !
    WinterSeed    = false;
}

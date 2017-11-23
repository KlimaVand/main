
#include "../../base/common.h"
#include "cropTriticale.h"

cropTriticale::cropTriticale(string       aName,
                             const int    aIndex,
                             const base * aOwner,
                             string       cropName):
    cropWwheat(aName,
               aIndex,
               aOwner,
               cropName)
{
    CropName      = "Triticale";
    PlantItemName = "Triticale";    // These names maps to products.dat !
    StrawItemName = "TRITSTRAW";    // These names maps to products.dat !
}


#include "../../base/common.h"
#include "cropSunflower.h"
#include "../../products/products.h"

cropSunflower::cropSunflower(string       aName,
                             const int    aIndex,
                             const base * aOwner,
                             string       cropName):
    cropMaize(aName,
              aIndex,
              aOwner,
              cropName)
{

    // Parameters
    PlantItemName = "SUNFLOWER";         // These names map to products.dat !
    StrawItemName = "SUNFLOWERSTRAW";    // These names map to products.dat !
}

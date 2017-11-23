
#include "../../base/common.h"
#include "cropFabaBean.h"
#include "../../base/climate.h"
#include "../../base/bstime.h"

cropFabaBean::cropFabaBean(string       aName,
                           const int    aIndex,
                           const base * aOwner,
                           string       cropName):
    cropPea(aName,
            aIndex,
            aOwner,
            cropName)
{
    CropName      = "FabaBean";
    PlantItemName = "FBEAN";         // These names maps to products.dat !
    StrawItemName = "FBEANSTRAW";    // These names maps to products.dat !
}

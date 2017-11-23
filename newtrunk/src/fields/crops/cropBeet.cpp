
#include "../../base/common.h"
#include "cropBeet.h"
#include "../../base/commonData.h"

cropBeet::cropBeet(string       aName,
                   const int    aIndex,
                   const base * aOwner,
                   string       cropName):
    crop(aName,
         aIndex,
         aOwner,
         cropName)
{

    // Parameters
    PlantItemName = "BEET";       // These names maps to products.dat !
    StrawItemName = "BEETTOP";    // These names maps to products.dat !
    WinterSeed    = false;        // Is the present crop a wintercrop.

    commonData * data = globalSettings -> CropInformation;

    data -> FindItem("TempSumStorage1", &TempSumStorage1);
    data -> FindItem("TempSumStorage2", &TempSumStorage2);
    data -> FindItem("MinAllocToStorage", &MinAllocToStorage);
    data -> FindItem("MaxAllocToStorage", &MaxAllocToStorage);
}

cropBeet::cropBeet(const cropBeet & acrop):
    crop(acrop)
{
    TempSumStorage1   = acrop.TempSumStorage1;
    TempSumStorage2   = acrop.TempSumStorage2;
    MinAllocToStorage = acrop.MinAllocToStorage;
    MaxAllocToStorage = acrop.MaxAllocToStorage;
}

void cropBeet::TransferDryMatterToStorage(double * dDryMatt) {
    double deltaStorage = 0.0;

    if (TempSumRoot <= TempSumStorage1) {
        deltaStorage = *dDryMatt * MinAllocToStorage;
    }

    if ((TempSumRoot > TempSumStorage1) && (TempSumRoot < TempSumStorage2)) {
        deltaStorage = *dDryMatt
                       * (MinAllocToStorage
                          + (MaxAllocToStorage - MinAllocToStorage)
                            * ((TempSumRoot - TempSumStorage1) / (TempSumStorage2 - TempSumStorage1)));
    }

    if (TempSumRoot >= TempSumStorage2) {
        deltaStorage = *dDryMatt * MaxAllocToStorage;
    }

    *dDryMatt        -= deltaStorage;
    DryMatterStorage += deltaStorage;
}

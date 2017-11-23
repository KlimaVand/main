
#include "../../base/common.h"
#include "../../base/commonData.h"
#include "cropPotato.h"

cropPotato::cropPotato(string       aName,
                       const int    aIndex,
                       const base * aOwner,
                       string       cropName):
    crop(aName,
         aIndex,
         aOwner,
         cropName)
{

    // Potato tubers approx. 24% DM ("Fodermiddeltabellen", 2000)
    // Planted approx 2 t/ha wet weight = 0.24 t DM/ha
    // Harvest amounts intermediate early: approx. 50 - 70 t wet weight; late: approx 60 - 80 t wet weight
    // Parameters
    PlantItemName = "POTATO";       // These names maps to products.dat !
    StrawItemName = "POTATOTOP";    // These names maps to products.dat !
    WinterSeed    = false;          // Is the present crop a wintercrop.

    commonData * data = globalSettings -> CropInformation;

    data -> FindItem("TempSumStorage1", &TempSumStorage1);
    data -> FindItem("TempSumStorage2", &TempSumStorage2);
    data -> FindItem("MinAllocToStorage", &MinAllocToStorage);
    data -> FindItem("MaxAllocToStorage", &MaxAllocToStorage);
}

cropPotato::cropPotato(const cropPotato & acrop):
    crop(acrop)
{
    TempSumStorage1   = acrop.TempSumStorage1;
    TempSumStorage2   = acrop.TempSumStorage2;
    MinAllocToStorage = acrop.MinAllocToStorage;
    MaxAllocToStorage = acrop.MaxAllocToStorage;
}

void cropPotato::TransferDryMatterToStorage(double * dDryMatt) {
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

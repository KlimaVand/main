
#include "../../base/common.h"
#include "staticGrazingGrass.h"

staticGrazingGrass::staticGrazingGrass() {
    cropName = "GRASS";
}

void staticGrazingGrass::SpecifiedPlan(fieldOrder * PP,
        int                                         JB_NO,
        string                                      crop_id) {
    staticCrop::SpecifiedPlan(PP, JB_NO, crop_id);
}

cloneList<fieldOperationFields> * staticGrazingGrass::MakeOpList(fieldOrder * PP,
        int                                                                   JB_NO) {

#ifdef DEVELOPING
    reduction   = reduction;
#endif

    aPlantItem1 = new plantItem;

    aPlantItem1 -> Setname("RYEGRASS");

    aPlantItem2 = new plantItem;

    aPlantItem2 -> Setname("GRASSIL1");

    string crop_id = "G4";    // TEMPORARY

    ResetOpList();

    if (!PP -> GetAutomatic()) {
        SpecifiedPlan(PP, JB_NO, crop_id);
    } else {    // Automatic plan
        {       // Ploughing
            soilTreatFields * FO = new soilTreatFields;

            FO -> DefineOp(Ploughing, crop_id, cropName, PP -> GetStartYear(), 9, 1, PP -> GetArea(), 0,
                           PP -> GetDistance());
            AddOpList(FO);
        }
        {    // Seedbed harrowing
            soilTreatFields * FO = new soilTreatFields;

            FO -> DefineOp(SeedbedHarrowing, crop_id, cropName, PP -> GetStartYear(), 9, 2, PP -> GetArea(), 0,
                           PP -> GetDistance());
            AddOpList(FO);
        }
        {    // Apply mineral fertilizer
            fertilizer * aFertilizer1 = new fertilizer;

            aFertilizer1 -> Setname("MINERAL-N");
            theProducts -> GiveProductInformation(aFertilizer1);
            aFertilizer1 -> Setamount(PP -> GetN());

            mineralFertFields * FO = new mineralFertFields;

            FO -> DefineMineralFertFieldsOp(MineralFertilizing, crop_id, cropName, PP -> GetStartYear() + 1, 4, 3,
                                            PP -> GetArea(), 0, PP -> GetDistance(), aFertilizer1, nullptr, nullptr);
            AddOpList(FO);

            delete aFertilizer1;
        }
        {    // Sowing
            sowFields * FO    = new sowFields;
            seed *      aSeed = new seed;

            aSeed -> Setname("SEEDRYEGRASS");
            theProducts -> GiveProductInformation(aSeed);
            aSeed -> Setamount(aSeed -> GetStandardDensity());
            FO -> DefineSowFieldsOp(Sowing, crop_id, cropName, PP -> GetStartYear(), 9, 3, PP -> GetArea(), 0,
                                    PP -> GetDistance(), PP -> GetPVStrat(), 1.0, aSeed);
            AddOpList(FO);

            delete aSeed;
        }
    }

    delete aPlantItem1;
    delete aPlantItem2;

    return LP;
}


#include "../../base/common.h"
#include "staticItalianRyeGrass.h"

// WARNING - All date's and amounts are wrong !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
staticItalianRyeGrass::staticItalianRyeGrass() {
    cropName = "ITALRYEG";
}

void staticItalianRyeGrass::SpecifiedPlan(fieldOrder * PP,
        int                                            JB_NO,
        string                                         crop_id) {
    staticCrop::SpecifiedPlan(PP, JB_NO, crop_id);

    if (PP -> GetNumOfHarvestDates() > 1) {
        theMessage -> Warning("static_italryeg::SpecifiedPlan - more than one harvest date specified");
    }
}

cloneList<fieldOperationFields> * staticItalianRyeGrass::MakeOpList(fieldOrder * PP,
        int                                                                      JB_NO) {
    aPlantItem1 = new plantItem;

    aPlantItem1 -> Setname(cropName);

    aPlantItem2 = new plantItem;

    aPlantItem2 -> Setname("ITALRYEGSTRAW");

    string crop_id = "IR";    // TEMPORARY

    ResetOpList();

    if (!PP -> GetAutomatic()) {
        SpecifiedPlan(PP, JB_NO, crop_id);
    } else {    // Automatic plan
        theMessage -> FatalError("static_italryeg::MakeOpList - automatic plan not implemented");
    }

    delete aPlantItem1;
    delete aPlantItem2;

    return LP;
}

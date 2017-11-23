
#include "../../base/common.h"
#include "staticFallowPermanent.h"

staticFallowPermanent::staticFallowPermanent() {
    cropName = "FLLW";
}

void staticFallowPermanent::SpecifiedPlan(fieldOrder * PP,
        int                                            JB_NO,
        string                                         crop_id) {
    staticCrop::SpecifiedPlan(PP, JB_NO, crop_id);
}

cloneList<fieldOperationFields> * staticFallowPermanent::MakeOpList(fieldOrder * PP,
        int                                                                      JB_NO) {
    aPlantItem1 = new plantItem;

    aPlantItem1 -> Setname("RYEGRASS");

    string crop_id = "F2";    // TEMPORARY

    ResetOpList();

    if (!PP -> GetAutomatic()) {
        SpecifiedPlan(PP, JB_NO, crop_id);
    } else {    // Automatic plan
        {       // Afhugning
            theProducts -> GiveProductInformation(aPlantItem1);
            aPlantItem1 -> Setamount(ExpectedYield_(JB_NO, PP -> GetNLevel(), PP -> GetPVStrat()));

            harvestFields * FO = new harvestFields;

            FO -> DefineHarvestFieldsOper(CutSetAside, crop_id, cropName, PP -> GetStartYear() + 1, 7, 14,
                                          PP -> GetArea(), 0, PP -> GetDistance(), 0, true,
                                          PP -> GetFractionStrawHarvested(0), PP -> GetFractionGrainHarvested(0));
            FO -> SetForageProduct(aPlantItem1);
            AddOpList(FO);
        }
    }

    delete aPlantItem1;

    return LP;
}

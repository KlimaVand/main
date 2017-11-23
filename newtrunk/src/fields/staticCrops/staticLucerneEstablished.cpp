
#include "../../base/common.h"
#include "staticLucerneEstablished.h"

staticLucerneEstablished::staticLucerneEstablished() {
    cropName = "LUCERNE";
}

void staticLucerneEstablished::SpecifiedPlan(fieldOrder * PP,
        int                                               JB_NO,
        string                                            crop_id) {
    staticCrop::SpecifiedPlan(PP, JB_NO, crop_id);
}

cloneList<fieldOperationFields> * staticLucerneEstablished::MakeOpList(fieldOrder * PP,
        int                                                                         JB_NO) {
    aPlantItem1 = new plantItem;

    aPlantItem1 -> Setname(cropName);

    aPlantItem2 = new plantItem;

    aPlantItem2 -> Setname("LUCERNESTRAW");

    string crop_id = "L2";    // TEMPORARY

    ResetOpList();

    if (!PP -> GetAutomatic()) {
        SpecifiedPlan(PP, JB_NO, crop_id);
    } else {    // Automatic plan - three cuts
        theMessage -> FatalError("static_lucerne_estab: automatic plan for lucerne not implemented");

        {
            plantItem * aPlantItem = new plantItem;

            aPlantItem -> Setname("LUCERNE");
            theProducts -> GiveProductInformation(aPlantItem);
            aPlantItem -> Setamount(ExpectedYield_(JB_NO, PP -> GetNLevel(), PP -> GetPVStrat()));

            harvestFields * FO = new harvestFields;

            FO -> DefineHarvestFieldsOper(CutSetAside, crop_id, cropName, PP -> GetStartYear() + 1, 6, 20,
                                          PP -> GetArea(), 0, PP -> GetDistance(), 0, true);
            FO -> SetMainProduct(aPlantItem);
            AddOpList(FO);

            delete aPlantItem;
        }
        {
            plantItem * aPlantItem = new plantItem;

            aPlantItem -> Setname("LUCERNE");
            theProducts -> GiveProductInformation(aPlantItem);
            aPlantItem -> Setamount(ExpectedYield_(JB_NO, PP -> GetNLevel(), PP -> GetPVStrat()));

            harvestFields * FO = new harvestFields;

            FO -> DefineHarvestFieldsOper(CutSetAside, crop_id, cropName, PP -> GetStartYear() + 1, 7, 26,
                                          PP -> GetArea(), 0, PP -> GetDistance(), 0, true);
            FO -> SetMainProduct(aPlantItem);
            AddOpList(FO);

            delete aPlantItem;
        }
        {
            plantItem * aPlantItem = new plantItem;

            aPlantItem -> Setname("LUCERNE");
            theProducts -> GiveProductInformation(aPlantItem);
            aPlantItem -> Setamount(ExpectedYield_(JB_NO, PP -> GetNLevel(), PP -> GetPVStrat()));

            harvestFields * FO = new harvestFields;

            FO -> DefineHarvestFieldsOper(CutSetAside, crop_id, cropName, PP -> GetStartYear() + 1, 9, 12,
                                          PP -> GetArea(), 0, PP -> GetDistance(), 0, true);
            FO -> SetMainProduct(aPlantItem);
            AddOpList(FO);

            delete aPlantItem;
        }
    }

    delete aPlantItem1;
    delete aPlantItem2;

    return LP;
}

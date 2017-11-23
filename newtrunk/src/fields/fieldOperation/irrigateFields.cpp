
#include "../../base/common.h"
#include "irrigateFields.h"

irrigateFields::irrigateFields() {
    aWater              = nullptr;
    automaticIrrigation = false;
}

irrigateFields::irrigateFields(const irrigateFields & i):
    fieldOperationFields(i)
{
    if (i.aWater) {
        aWater = new water(*i.aWater);
    } else {
        aWater = nullptr;
    }

    automaticIrrigation = i.automaticIrrigation;
}

irrigateFields::~irrigateFields() {
    if (aWater) {
        delete aWater;
    }
}

irrigateFields * irrigateFields::clone() const {
    irrigateFields * i = new irrigateFields(*this);

    return i;
}

void irrigateFields::DefineIrrigateFieldsOp(operationNames op,
        string                                             crop,
        string                                             cropNm,
        int                                                yr,
        int                                                mon,
        int                                                d,
        double                                             ar,
        int                                                fieldn,
        double                                             dist,
        water *                                            theWater,
        bool                                               automatic) {
    fieldOperationFields::DefineOp(op, crop, cropNm, yr, mon, d, ar, fieldn, dist);

    aWater              = new water(*theWater);
    automaticIrrigation = automatic;
}

/*
 * Calculate the cost of this operation.
 * If update is true it will write the cost to the indicate file.
 * If update is false it will return the cost in the cost variable )
 */
bool irrigateFields::CalcCost(linkList<operation> * aOperationList,
                              linkList<techEquip> * aTechFarmList,
                              calcLPType            mode,
                              double *              cost,
                              bool                  update) {
    if (!update
            && ((mode == hoursCombine) || (mode == hoursAnimalFert) || (mode == hoursMineralFert)
                || (mode == hoursPlantCare) || (mode == varCostAnimalFert) || (mode == varCostMineralFert))) {
        return false;
    }

    // Calculate cost of operation
    linkList<operation>::PS aOperationNode;
    aOperationList -> PeekHead(aOperationNode);

    while (aOperationNode) {
        if (aOperationNode -> element -> GetoperationNumber() == op_id) {
            break;
        }

        aOperationNode = aOperationNode -> next;
    }

    if (!aOperationNode) {
        theMessage -> WarningWithDisplay("irrigateFields::CalcCost - Operation ", operationNameList[op_id],
                                         " not found in list");

        return false;
    } else if (area <= 0.0) {
        theMessage -> WarningWithDisplay("irrigateFields::CalcCost - area is less than zero");

        return false;
    } else {
        irrigateTech * irrigateTechInstance;

        irrigateTechInstance = (irrigateTech *) aOperationNode -> element;

        irrigateTechInstance -> Setarea(area);
        irrigateTechInstance -> Setdistance(distance);
        irrigateTechInstance -> SetoperationName(operationNameList[op_id]);
        irrigateTechInstance -> SetcropName(cropName);
        irrigateTechInstance -> SetTheWater(aWater);
        irrigateTechInstance -> CreateGang(aTechFarmList);

        if (update) {
            irrigateTechInstance -> Update(aTechFarmList);
        } else {
            irrigateTechInstance -> CalcCost(mode);

            *cost = irrigateTechInstance -> Getcost();
        }

        irrigateTechInstance -> DeleteGang();
        irrigateTechInstance -> ClearTheWater();

        return true;
    }
}

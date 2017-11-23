
#include "../../base/common.h"
#include "plantCareFields.h"

plantCareFields::plantCareFields() {
    aChemical1 = nullptr;
    aChemical2 = nullptr;
    aChemical3 = nullptr;
    aWater     = nullptr;
}

plantCareFields::plantCareFields(const plantCareFields & p):
    fieldOperationFields(p)
{
    if (p.aChemical1) {
        aChemical1 = new chemical(*p.aChemical1);
    } else {
        aChemical1 = nullptr;
    }

    if (p.aChemical2) {
        aChemical2 = new chemical(*p.aChemical2);
    } else {
        aChemical2 = nullptr;
    }

    if (p.aChemical3) {
        aChemical3 = new chemical(*p.aChemical3);
    } else {
        aChemical3 = nullptr;
    }

    if (p.aWater) {
        aWater = new water(*p.aWater);
    } else {
        aWater = nullptr;
    }
}

plantCareFields::~plantCareFields() {
    if (aChemical1) {
        delete aChemical1;
    }

    if (aChemical2) {
        delete aChemical2;
    }

    if (aChemical3) {
        delete aChemical3;
    }

    if (aWater) {
        delete aWater;
    }
}

plantCareFields * plantCareFields::clone() const {
    plantCareFields * p = new plantCareFields(*this);

    return p;
}

void plantCareFields::DefinePlantCareFieldsOp(operationNames op,
        string                                               crop,
        string                                               cropNm,
        int                                                  yr,
        int                                                  mon,
        int                                                  d,
        double                                               ar,
        int                                                  fieldn,
        double                                               dist,
        chemical *                                           chem1,
        chemical *                                           chem2,
        chemical *                                           chem3,
        water *                                              wat) {
    fieldOperationFields::DefineOp(op, crop, cropNm, yr, mon, d, ar, fieldn, dist);

    aChemical1 = new chemical(*chem1);

    if (chem2) {
        aChemical2 = new chemical(*chem2);
    }

    if (chem3) {
        aChemical3 = new chemical(*chem3);
    }

    aWater = new water(*wat);
}

/*
 * Calculate the cost of this operation.
 * If update is true it will write the cost to the indicate file.
 * If update is false it will return the cost in the cost variable
 */
bool plantCareFields::CalcCost(linkList<operation> * aOperationList,
                               linkList<techEquip> * aTechFarmList,
                               calcLPType            mode,
                               double *              cost,
                               bool                  update) {
    if (!update
            && ((mode == hoursCombine) || (mode == hoursAnimalFert) || (mode == hoursMineralFert)
                || (mode == varCostAnimalFert) || (mode == varCostMineralFert))) {
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
        theMessage -> WarningWithDisplay("plantCareFields::CalcCost - Operation ", operationNameList[op_id],
                                         " not found in list");

        return false;
    } else if (area <= 0.0) {
        theMessage -> WarningWithDisplay("plantCareFields::CalcCost - area is less than zero");

        return false;
    } else {
        plantCareTech * plantCareTechInstance;

        plantCareTechInstance = (plantCareTech *) aOperationNode -> element;

        plantCareTechInstance -> Setarea(area);
        plantCareTechInstance -> Setdistance(distance);
        plantCareTechInstance -> SetoperationName(operationNameList[op_id]);
        plantCareTechInstance -> SetcropName(cropName);
        plantCareTechInstance -> SetTheWaterAndChemicals(aWater, aChemical1, aChemical2, aChemical3);

        if (plantCareTechInstance -> CreateGang(aTechFarmList)) {

            // All neccesary machines present at farm
            if (update) {
                plantCareTechInstance -> Update(aTechFarmList);
            } else {
                plantCareTechInstance -> CalcCost(mode);

                *cost = plantCareTechInstance -> Getcost();
            }
        } else {    // Buy service
            if (update) {
                product * hiredService = new product;

                hiredService -> Setname("MachinePoolCost");
                theProducts -> GiveProductInformation(hiredService);
                hiredService -> Setamount(plantCareTechInstance -> GetMachinePoolPrice() * area);

                // theProducts->AddProduct(hiredService); Removed BMP 12.10.2000
                plantCareTechInstance -> UpdateProducts(true);
                theOutput -> AddIndicator(708, "07.08 Cost of machine contractors", "Dkr",
                                          -plantCareTechInstance -> GetMachinePoolPrice() * area);

                delete hiredService;
            } else if (mode == varCostTotalNotFert) {
                *cost = -plantCareTechInstance -> GetMachinePoolPrice();
            } else {
                *cost = 0.0;
            }
        }

        plantCareTechInstance -> DeleteGang();
        plantCareTechInstance -> ClearTheWaterAndChemicals();

        return true;
    }
}

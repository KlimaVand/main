/*
 *
 * Supports several species
 *
 */

#include "../../base/common.h"
#include "harvestFields.h"

harvestFields::harvestFields() {
    for (int i = 0; i < MaxPlants; i++) {
        MainProduct[i]   = nullptr;
        StrawProduct[i]  = nullptr;
        ForageProduct[i] = nullptr;
    }

    CuttingHeight          = 0.07;
    FractionGrainHarvested = 0;
    FractionStrawHarvested = 0;
    forcedHarvest          = false;
}

harvestFields::harvestFields(const harvestFields & h):
    fieldOperationFields(h)
{
    for (int i = 0; i < MaxPlants; i++) {
        if (h.MainProduct[i]) {
            MainProduct[i] = new plantItem(*h.MainProduct[i]);
        } else {
            MainProduct[i] = nullptr;
        }

        if (h.StrawProduct[i]) {
            StrawProduct[i] = new plantItem(*h.StrawProduct[i]);
        } else {
            StrawProduct[i] = nullptr;
        }

        if (h.ForageProduct[i]) {
            ForageProduct[i] = new plantItem(*h.ForageProduct[i]);
        } else {
            ForageProduct[i] = nullptr;
        }
    }

    CuttingHeight          = h.CuttingHeight;
    FractionStrawHarvested = h.FractionStrawHarvested;
    FractionGrainHarvested = h.FractionGrainHarvested;
    forcedHarvest          = h.forcedHarvest;
}

harvestFields::~harvestFields() {
    for (int i = 0; i < MaxPlants; i++) {
        if (MainProduct[i]) {
            delete MainProduct[i];
        }

        if (StrawProduct[i]) {
            delete StrawProduct[i];
        }

        if (ForageProduct[i]) {
            delete ForageProduct[i];
        }

        MainProduct[i]   = nullptr;
        StrawProduct[i]  = nullptr;
        ForageProduct[i] = nullptr;
    }
}

harvestFields * harvestFields::clone() const {
    harvestFields * h = new harvestFields(*this);

    return h;
}

void harvestFields::SetMainProduct(plantItem * aMainProduct,
                                   int         i) {
    if ((i >= MaxPlants) || (i < 0)) {
        theMessage -> FatalError("harvestFields::SetMainProduct - function called with illegal index");
    }

    if (!aMainProduct) {
        theMessage -> FatalError("harvestFields::SetMainProduct - function called with nullptr");
    }

    if (MainProduct[i]) {
        delete MainProduct[i];
    }

    MainProduct[i] = new plantItem(*aMainProduct);
}

void harvestFields::AddMainProduct(plantItem * aMainProduct,
                                   int         i) {
    if ((i >= MaxPlants) || (i < 0)) {
        theMessage -> FatalError("harvestFields::AddMainProduct - function called with illegal index");
    }

    if (!aMainProduct) {
        theMessage -> FatalError("harvestFields::AddMainProduct - function called with nullptr");
    }

    if (!MainProduct[i]) {
        theMessage -> FatalError("harvestFields::AddMainProduct - can not add product to nullptr product pointer");
    }

    *MainProduct[i] = *MainProduct[i] + plantItem(*aMainProduct);
}

void harvestFields::SetStrawProduct(plantItem * aStrawProduct,
        int                                     i) {
    if ((i >= MaxPlants) || (i < 0)) {
        theMessage -> FatalError("harvestFields::SetStrawProduct - function called with illegal index");
    }

    if (!aStrawProduct) {
        theMessage -> FatalError("harvestFields::SetStrawProduct - function called with nullptr");
    }

    if (StrawProduct[i]) {
        delete StrawProduct[i];
    }

    StrawProduct[i] = new plantItem(*aStrawProduct);
}

void harvestFields::AddStrawProduct(plantItem * aStrawProduct,
        int                                     i) {
    if ((i >= MaxPlants) || (i < 0)) {
        theMessage -> FatalError("harvestFields::AddStrawProduct - function called with illegal index");
    }

    if (!aStrawProduct) {
        theMessage -> FatalError("harvestFields::AddStrawProduct - function called with nullptr");
    }

    if (!StrawProduct[i]) {
        theMessage -> FatalError("harvestFields::AddStrawProduct - can not add product to nullptr product pointer");
    }

    *StrawProduct[i] = *StrawProduct[i] + plantItem(*aStrawProduct);
}

void harvestFields::SetForageProduct(plantItem * aForageProduct,
        int                                      i) {
    if ((i >= MaxPlants) || (i < 0)) {
        theMessage -> FatalError("harvestFields::SetForageProduct - function called with illegal index");
    }

    if (!aForageProduct) {
        theMessage -> FatalError("harvestFields::SetForageProduct - function called with nullptr");
    }

    if (ForageProduct[i]) {
        delete ForageProduct[i];
    }

    ForageProduct[i] = new plantItem(*aForageProduct);
}

void harvestFields::DefineHarvestFieldsOper(operationNames op,
        string                                             crop,
        string                                             cropNm,
        int                                                yr,
        int                                                mon,
        int                                                d,
        double                                             ar,
        int                                                fieldn,
        double                                             dist,
        double                                             reduc,
        bool                                               forced,
        double                                             StrHarvest,
        double                                             GrnHarvest) {
    fieldOperationFields::DefineOp(op, crop, cropNm, yr, mon, d, ar, fieldn, dist);

    forcedHarvest          = forced;
    FractionStrawHarvested = StrHarvest;
    FractionGrainHarvested = GrnHarvest;
}

bool harvestFields::CalcCost(linkList<operation> * aOperationList,
                             linkList<techEquip> * aTechFarmList,
                             calcLPType            mode,
                             double *              cost,
                             bool                  update) {
    if (!update
            && ((mode == hoursAnimalFert) || (mode == hoursMineralFert) || (mode == hoursPlantCare)
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
        theMessage -> FatalError("harvestFields::CalcCost - Operation ", operationNameList[op_id],
                                 " not found in list");

        return false;
    } else if (area <= 0.0) {
        theMessage -> FatalError("harvestFields::CalcCost - area is less than zero");

        return false;
    } else {
        harvestTech * harvestTechInstance;

        harvestTechInstance = (harvestTech *) aOperationNode -> element;

        harvestTechInstance -> Setarea(area);
        harvestTechInstance -> Setdistance(distance);
        harvestTechInstance -> SetoperationName(operationNameList[op_id]);
        harvestTechInstance -> SetcropName(cropName);

        for (int i = 0; i < MaxPlants; i++) {

            // if (MainProduct[i]!=nullptr) !!!
            harvestTechInstance -> SetMainProduct(i, MainProduct[i]);

            // if (StrawProduct[i]!=nullptr) !!!
            harvestTechInstance -> SetStrawProduct(i, StrawProduct[i]);

            // if (ForageProduct[i]!=nullptr) !!!
            harvestTechInstance -> SetForageProduct(i, ForageProduct[i]);
        }

        harvestTechInstance -> SetoperationID(op_id);

        if (harvestTechInstance -> CreateGang(aTechFarmList)) {

            // All neccesary machines present at farm
            if (update) {
                harvestTechInstance -> Update(aTechFarmList);
            } else {
                harvestTechInstance -> CalcCost(mode);

                *cost = harvestTechInstance -> Getcost();
            }
        } else {    // Buy service
            if (update) {
                product * hiredService = new product;

                hiredService -> Setname("MachinePoolCost");
                theProducts -> GiveProductInformation(hiredService);
                hiredService -> Setamount(harvestTechInstance -> GetMachinePoolPrice() * area);

                // theProducts->AddProduct(hiredService); Removed BMP 12.10.2000
                harvestTechInstance -> UpdateProducts(true);
                theOutput -> AddIndicator(708, "07.08 Cost of machine contractors", "Dkr",
                                          -harvestTechInstance -> GetMachinePoolPrice() * area);

                delete hiredService;
            } else if (mode == varCostTotalNotFert) {
                *cost = -harvestTechInstance -> GetMachinePoolPrice();
            } else {
                *cost = 0.0;
            }
        }

        harvestTechInstance -> DeleteGang();

        // NJH, feb07
        ofstream * file = theMessage -> GiveHandle();

        Output(file);

        return true;
    }
}

void harvestFields::UpdateProducts() {
    linkList<plantItem> * aList = new linkList<plantItem>;

    for (int i = 0; i < MaxPlants; i++) {
        if (MainProduct[i]
                && (MainProduct[i] -> GetAmount()
                    > 0)) {    // pack MainProducts into a linklist and send to Products for transport to storage
            aList -> InsertLast(MainProduct[i]);
        }
    }

    if (aList -> NumOfNodes() > 0) {
        theProducts -> AddHarvestGrainProducts(aList);
        aList -> ForgetAll();
    }

    for (int i = 0; i < MaxPlants; i++) {
        if (StrawProduct[i] && (StrawProduct[i] -> GetAmount() > 0)) {
            aList -> InsertLast(StrawProduct[i]);
        }
    }

    if (aList -> NumOfNodes() > 0) {
        theProducts -> AddHarvestStrawProducts(aList);
        aList -> ForgetAll();
    }

    for (int i = 0; i < MaxPlants; i++) {
        if (ForageProduct[i] && (ForageProduct[i] -> GetAmount() > 0)) {
            aList -> InsertLast(ForageProduct[i]);
        }
    }

    if (aList -> NumOfNodes() > 0) {
        theProducts -> AddForageProducts(aList);
        aList -> ForgetAll();
    }

    delete aList;
}

void harvestFields::Output(ofstream * fs) {
    for (int i = 0; i < MaxPlants; i++) {
        if (MainProduct[i]) {
            *fs << " Product harvested " << MainProduct[i] -> GetName() << " "
                << (MainProduct[i] -> GetAmount() * MainProduct[i] -> GetdryMatter() / area) << " tDM/ha "
                << MainProduct[i] -> GetAllN().n * 100.0 << " %N";
        }
    }

    for (int i = 0; i < MaxPlants; i++) {
        if (StrawProduct[i]) {
            *fs << " Product harvested " << StrawProduct[i] -> GetName() << " "
                << (StrawProduct[i] -> GetAmount() * StrawProduct[i] -> GetdryMatter() / area) << " tDM/ha "
                << StrawProduct[i] -> GetAllN().n * 100.0 << " %N";
        }
    }

    for (int i = 0; i < MaxPlants; i++) {
        if (ForageProduct[i]) {
            *fs << " Product harvested " << ForageProduct[i] -> GetName() << " "
                << (ForageProduct[i] -> GetAmount() * ForageProduct[i] -> GetdryMatter() / area) << " tDM/ha "
                << ForageProduct[i] -> GetAllN().n * 100.0 << " %N";
        }
    }
}

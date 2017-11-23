
#include "../base/common.h"
#include "irrigateTech.h"
#include "../products/energy.h"

irrigateTech::irrigateTech(string       aName,
                           const int    aIndex,
                           const base * aOwner):
    fieldOperationTech(aName,
                       aIndex,
                       aOwner)
{
    workingLength     = 0.0;
    constantPerShift  = 0.0;
    variablePerLength = 0.0;
    hoursPerDay       = 0.0;
    theWater          = nullptr;
}

irrigateTech::~irrigateTech() {
    if (theWater) {
        delete theWater;
    }
}

void irrigateTech::ReadParameters(commonData *& file) {
    fieldOperationTech::ReadParameters(file);
    file -> FindItem("workingLength", &workingLength);
    file -> FindItem("constantPerShift", &constantPerShift);
    file -> FindItem("variablePerLength", &variablePerLength);
    file -> FindItem("hoursPerDay", &hoursPerDay);
}

void irrigateTech::SetTheWater(water * aWater) {
    theWater = new water(*aWater);
}

void irrigateTech::ClearTheWater() {
    if (theWater) {
        delete theWater;

        theWater = nullptr;
    }
}

void irrigateTech::CalcCost(calcLPType mode) {
    switch (mode) {
        case hoursTotalNotCombineNotFert :
            CalcGangHours();

            cost = aGang -> GetgangHours();

            break;

        case varCostTotalNotFert :
            CalcGangHours();
            CalcGangMaintenance();
            CalcGangElectricity();

            cost = -1.0
                   * (aGang -> GetgangElectricity() * theProducts -> GetExpectedBuyPriceYear1("ELECTRICITY")
                      + aGang -> GetgangMaintenance()
                      + theWater -> GetAmount() * theProducts -> GetExpectedBuyPriceYear1("WATER"));

            // It is assumed that the irrigationfacilities not are rented
            break;

        default :
            cost = 0.0;
    }
}

void irrigateTech::CalcGangHours() {
    linkList<techEquip> * aTechEquipList;

    aTechEquipList = aGang -> GetTechEquipList();

    linkList<techEquip>::PS aTechEquipNode;

    techEquip aMainTechEquip("maintechequip", 0, this);

    aMainTechEquip = *(aGang -> GetMainTechEquip());

    double       size         = aMainTechEquip.Getsize();
    double       pumpCapacity = 0.0;
    installation aWaterPump("installation", 0, this);

    aTechEquipList -> PeekHead(aTechEquipNode);

    while (aTechEquipNode) {
        if ((aTechEquipNode -> element -> GetName() == "installation")
                && (aTechEquipNode -> element -> GettechEquipName() == "WaterPump")) {
            break;
        }

        aTechEquipNode = aTechEquipNode -> next;
    }

    if (aTechEquipNode) {
        aWaterPump   = *(installation *) aTechEquipNode -> element;
        pumpCapacity = aWaterPump.Getcapacity();
    }

    if (pumpCapacity == 0) {
        theMessage -> FatalError("pumpCapacity = 0 in operation ",  operationName);
    }

    double h1 = (size * workingLength) / 10000;    // area/placing
    double E  = (constantPerShift + (variablePerLength * workingLength / 100))
                * (1 + relaxationAllowance);       // movingtime pr. placing
    double S = ((E / h1) * 1.1 * area) / 60;    // hours total

    aGang -> SetgangHours(S);
}

void irrigateTech::CalcGangElectricity() {
    double         aGangElectricity = 0.0;
    installation * aInstallation;

    aInstallation = new installation("installation", 0, this);

    techEquip * aMainTechEquip;

    aMainTechEquip = aGang -> GetMainTechEquip();

    if (aMainTechEquip -> GetName() == "installation") {
        *aInstallation   = *(installation *) aMainTechEquip;
        aGangElectricity += aInstallation -> GetelectricityConsumption();
    }

    delete aInstallation;

    double totalWaterAmount = theWater -> GetAmount() * area;

    aGangElectricity *= totalWaterAmount;

    aGang -> SetgangElectricity(int(aGangElectricity));
}

void irrigateTech::UpdateProducts(bool contractor) {
    energy * aEnergyProduct = new energy;

    aEnergyProduct -> Setname("ELECTRICITY");
    theProducts -> GiveProductInformation(aEnergyProduct);
    aEnergyProduct -> Setamount(aGang -> GetgangElectricity());

    // theOutput->AddIndicator(2002,"20.02 Electricity consumption","KWh",aEnergyProduct->GetAmount());
    theProducts -> SubtractProduct(aEnergyProduct);

    delete aEnergyProduct;

    (*theWater) * area;

    // theOutput->AddIndicator(7002,"70.02 Irrigation","cubic metre",theWater->GetAmount());
    theProducts -> SubtractProduct(theWater);
}

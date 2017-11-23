
#include "../base/common.h"
#include "sowTech.h"
#include "../products/energy.h"
#include "../products/products.h"

sowTech::sowTech(string       aName,
                 const int    aIndex,
                 const base * aOwner):
    fieldOperationTech(aName,
                       aIndex,
                       aOwner)
{
    prepFillSowing     = 0.0;
    fillingTime1Sowing = 0.0;
    fillingTime2Sowing = 0.0;
    fillingAmount1     = 0.0;
    fillingAmount2     = 0.0;
    roadSpeed          = 0.0;
    changingTrailer    = 0.0;
    loadingTime        = 0.0;
    prepLoading        = 0.0;
    numOfSeeds         = 0;

    for (int i = 0; i < MaxPlants; i++) {
        theSeeds[i] = nullptr;
    }
}

sowTech::~sowTech() {
    for (int i = 0; i < MaxPlants; i++) {
        if (theSeeds[i]) {
            delete theSeeds[i];
        }
    }
}

void sowTech::ReadParameters(commonData *& file) {
    fieldOperationTech::ReadParameters(file);
    file -> FindItem("prepFillSowing", &prepFillSowing);
    file -> FindItem("fillingTime1Sowing", &fillingTime1Sowing);
    file -> FindItem("fillingTime2Sowing", &fillingTime2Sowing);
    file -> FindItem("fillingAmount1", &fillingAmount1);
    file -> FindItem("fillingAmount2", &fillingAmount2);
    file -> FindItem("roadSpeed", &roadSpeed);
    file -> FindItem("changingTrailer", &changingTrailer);
    file -> FindItem("prepLoading", &prepLoading);
    file -> FindItem("loadingTime", &loadingTime);
}

void sowTech::AddSeed(seed * s) {
    theSeeds[numOfSeeds] = new seed(*s);

    numOfSeeds++;
}

void sowTech::ClearTheSeeds() {
    for (int i = 0; i < MaxPlants; i++) {
        if (theSeeds[i]) {
            delete theSeeds[i];
        }

        theSeeds[i] = nullptr;
    }
}

void sowTech::CalcCost(calcLPType mode) {
    techEquip aMainTechEquip("maintechequip", 0, this);

    switch (mode) {
        case hoursTotalNotCombineNotFert :
            CalcGangHours();

            cost = aGang -> GetgangHours();

            break;

        case varCostTotalNotFert :
            CalcGangHours();
            CalcGangFuel();
            CalcGangMaintenance();

            break;

        default :
            cost = 0.0;
    }
}

void sowTech::CalcGangHours() {
    techEquip aMainTechEquip("maintechequip", 0, this);

    aMainTechEquip = *(aGang -> GetMainTechEquip());

    double    size          = aMainTechEquip.Getsize();
    double    carryCapacity = 0.0;
    implement aImplement("implement", 0, this);

    linkList<techEquip> * aTechEquipList;
    linkList<techEquip>::PS aTechEquipNode;

    aTechEquipList = aGang -> GetTechEquipList();

    aTechEquipList -> PeekHead(aTechEquipNode);

    while (aTechEquipNode) {
        if (aTechEquipNode -> element -> GetName() == "implement") {
            aImplement = *(implement *) (aTechEquipNode -> element);

            if (aImplement.GettechEquipName() != "SeedDrill") {
                carryCapacity += aImplement.GetCarryCapacity();
            }
        }

        aTechEquipNode = aTechEquipNode -> next;
    }

    if (workingSpeed == 0) {
        theMessage -> FatalError("workingSpeed = 0 in operation ", operationName);
    }

    if (size == 0) {
        theMessage -> FatalError("size = 0 in operation ", operationName);
    }

    if (area == 0) {
        theMessage -> FatalError("area = 0 in operation ", operationName);
    }

    if (fillingAmount1 == 0) {
        theMessage -> FatalError("fillingAmount1 = 0 in operation ", operationName);
    }

    if (roadSpeed == 0) {
        theMessage -> FatalError("roadSpeed = 0 in operation ", operationName);
    }

    if (changingTrailer == 0) {
        theMessage -> FatalError("changingTrailer = 0 in operation ",  operationName);
    }

    if (carryCapacity == 0) {
        theMessage -> FatalError("carryCapacity = 0 in operation ",  operationName);
    }

    double amount1;
    double amount2     = 0.0;
    double totalAmount = 0.0;

    for (int i = 0; i < MaxPlants; i++) {
        if (theSeeds[i]) {
            totalAmount += theSeeds[i] -> GetAmount();
        }
    }

    // Warning the calculation is not completely adjusted to more than two different seeds 4/11-98 JB
    amount1 = 0;

    if (theSeeds[0]) {
        amount1 = theSeeds[0] -> GetAmount();
    } else {
        theMessage -> WarningWithDisplay("sowTech::CalcGangHours - no seed defined");
    }

    if (theSeeds[1]) {
        amount2 = theSeeds[1] -> GetAmount();
    }

    double A = ((area * 600) / (workingSpeed * size) + (turningTime * (sqrt(area * 10000 / 2) - size * 6)) / size
                + turningHeadland + (disturbanceAllowance * area)) / area;
    double temp = 0.0;

    if (fillingAmount2 == 0) {
        temp = amount1 / fillingAmount1;
    } else {
        temp = max(amount1 / fillingAmount1, amount2 / fillingAmount2);
    }

    double I = (fillingTime1Sowing * amount1 / 100) + (fillingTime2Sowing * amount2 / 100) + (prepFillSowing * temp);
    double T = ((distance * 0.12) / roadSpeed + changingTrailer) * (totalAmount / carryCapacity);
    double L = (prepLoading + (loadingTime * carryCapacity / 100)) * (totalAmount / carryCapacity);
    double S = ((A + I + T + L) * (1 + relaxationAllowance) * 1.1 * area) / 60;

    aGang -> SetgangHours(S);
}

void sowTech::UpdateProducts(bool contractor) {
    fieldOperationTech::UpdateProducts(contractor);

    for (int i = 0; i < MaxPlants; i++) {
        if (theSeeds[i]) {
            theSeeds[i] -> Setamount(theSeeds[i] -> GetAmount() * area);
            theProducts -> SubtractProduct(theSeeds[i]);
        }
    }
}

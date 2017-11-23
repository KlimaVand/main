
#include "../base/common.h"
#include "plantCareTech.h"
#include "../products/energy.h"

plantCareTech::plantCareTech(string       aName,
                             const int    aIndex,
                             const base * aOwner):
    fieldOperationTech(aName,
                       aIndex,
                       aOwner)
{
    roadSpeed           = 0.0;
    specificArea        = 0.0;
    fillingTime         = 0.0;
    prepFillingTank     = 0.0;
    treatmentFrequency1 = 0.0;
    treatmentFrequency2 = 0.0;
    theWater            = nullptr;
    theChemical1        = nullptr;
    theChemical2        = nullptr;
    theChemical3        = nullptr;
}

plantCareTech::~plantCareTech() {
    if (theWater) {
        delete theWater;
    }

    if (theChemical1) {
        delete theChemical1;
    }

    if (theChemical2) {
        delete theChemical2;
    }

    if (theChemical3) {
        delete theChemical3;
    }
}

void plantCareTech::ReadParameters(commonData *& file) {
    fieldOperationTech::ReadParameters(file);
    file -> FindItem("roadSpeed", &roadSpeed);
    file -> FindItem("specificArea", &specificArea);
    file -> FindItem("prepFillingTank", &prepFillingTank);
    file -> FindItem("fillingTime", &fillingTime);
}

void plantCareTech::SetTheWaterAndChemicals(water * aWater,
        chemical *                                  aChemical1,
        chemical *                                  aChemical2,
        chemical *                                  aChemical3) {
    if (aWater) {
        theWater = new water;

        {
            *theWater = *aWater;
        }
    }

    if (aChemical1) {
        theChemical1  = new chemical;
        *theChemical1 = *aChemical1;
    }

    if (aChemical2) {
        theChemical2  = new chemical;
        *theChemical2 = *aChemical2;
    }

    if (aChemical3) {
        theChemical3  = new chemical;
        *theChemical3 = *aChemical3;
    }
}

void plantCareTech::ClearTheWaterAndChemicals() {
    if (theWater) {
        delete theWater;

        theWater = nullptr;
    }

    if (theChemical1) {
        delete theChemical1;

        theChemical1 = nullptr;
    }

    if (theChemical2) {
        delete theChemical2;

        theChemical2 = nullptr;
    }

    if (theChemical3) {
        delete theChemical3;

        theChemical3 = nullptr;
    }
}

void plantCareTech::CalcCost(calcLPType mode) {
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

            cost = -1.0
                   * (aGang -> GetgangFuel() * theProducts -> GetExpectedBuyPriceYear1("DIESEL")
                      + aGang -> GetgangMaintenance()
                      + area * theChemical1 -> GetAmount()
                        * (theChemical1 -> GetaPrice()) -> GetExpectedBuyPriceYear(1));

            if (theChemical2) {
                cost += -1.0 * area * theChemical2 -> GetAmount()
                        * (theChemical1 -> GetaPrice()) -> GetExpectedBuyPriceYear(1);
            }

            if (theChemical3) {
                cost += -1.0 * area * theChemical3 -> GetAmount()
                        * (theChemical1 -> GetaPrice()) -> GetExpectedBuyPriceYear(1);
            }

            break;

        default :
            cost = 0.0;
    }
}

void plantCareTech::CalcGangHours() {
    techEquip aMainTechEquip("maintechequip", 0, this);

    aMainTechEquip = *(aGang -> GetMainTechEquip());

    double size     = aMainTechEquip.Getsize();
    double tankSize = aMainTechEquip.GetCarryCapacity();

    if (workingSpeed == 0) {
        theMessage -> FatalError("workingSpeed = 0 in operation ",  operationName);
    }

    if (size == 0) {
        theMessage -> FatalError("size = 0 in operation ", operationName);
    }

    if (area == 0) {
        theMessage -> FatalError("area = 0 in operation ",  operationName);
    }

    if (tankSize == 0) {
        theMessage -> FatalError("tankSize = 0 in operation ",  operationName);
    }

    if (roadSpeed == 0) {
        theMessage -> FatalError("roadSpeed = 0 in operation ",  operationName);
    }

    if (specificArea == 0) {
        theMessage -> FatalError("specificArea = 0 in operation ",  operationName);
    }

    double A = (((area * 600) / (workingSpeed * size)) + ((turningTime * (sqrt(area * 10000 / 2) - 2 * size)) / size)
                + (turningHeadland + disturbanceAllowance * area)) / area;
    double n = ceil(specificArea * theWater -> GetAmount() / tankSize);
    double T = (distance * n * 0.12) / (roadSpeed * specificArea);
    double I = ((prepFillingTank * n) + (fillingTime * specificArea * theWater -> GetAmount() / 100)) / specificArea;
    double S = ((A + T + I) * (1 + relaxationAllowance) * 1.1 * area) / 60;

    aGang -> SetgangHours(S);
}

void plantCareTech::UpdateProducts(bool contractor) {
    fieldOperationTech::UpdateProducts(contractor);
    UpdateChemical(theChemical1);

    if (theChemical2) {
        UpdateChemical(theChemical2);
    }

    if (theChemical3) {
        UpdateChemical(theChemical3);
    }
}

void plantCareTech::UpdateChemical(chemical * aChemical) {
    (*aChemical) * area;
    theProducts -> SubtractProduct(aChemical);

    double aAmount = aChemical -> GetAmount();
    int    flag    = aChemical -> Gettype();

    switch (flag) {
        case herbicide :
            theOutput -> AddIndicator(8001, "80.01 Herbicide consumption", "TFI", aAmount);

            break;

        case fungicide :
            theOutput -> AddIndicator(8002, "80.02 Fungicide consumption", "TFI", aAmount);

            break;

        case insecticide :
            theOutput -> AddIndicator(8003, "80.03 Insecticide consumption", "TFI", aAmount);

            break;

        default :
            break;
    }
}

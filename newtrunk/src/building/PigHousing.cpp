
#include "../base/common.h"
#include "../base/IndicatorOutput.h"
#include "../building/PigHousing.h"
#include "../livestock/swineStock.h"
#include "../base/controlParameters.h"
#include "../base/commonData.h"
#include "../tools/convert.h"
#include <string>

PigHousing::PigHousing(string       aname,
                       const int    aIndex,
                       const base * aOwner,
                       commonData * runData,
                       commonData * common):
    stable(aname,
           aIndex,
           aOwner,
           runData,
           common)
{

    Livestock = new animalStock(aname, aIndex, aOwner);

    string PigHouseName(aname);

    PigHouseName += "(";
    PigHouseName += conv.IntToString(aIndex);
    PigHouseName += ").Section";

    int numbersOfSections = 0;
    int small             = 9999;

    runData -> getSectionsNumbers(PigHouseName, &small, &numbersOfSections);

    for (int i = small; i <= numbersOfSections; i++) {
        pigstysection * aSection = new pigstysection(PigHouseName, i, this, runData, common, controlledVent);

        sections -> InsertLast(aSection);
    }

    // string inhouse(aname);
    // see if the flooring sends manure to an inhouse manure store
    for (int i = 0; i < inhouseStores -> NumOfNodes(); i++) {
        manurestore * aStore = inhouseStores -> ElementAtNumber(i);

        for (int i = 0; i < sections -> NumOfNodes(); i++) {
            animalSection * aSection = sections -> ElementAtNumber(i);

            aSection -> LinkManureStores(aStore);
        }
    }

    if (theControlParameters -> GetvolatModel() > 0) {
        checkArea();
    }
}

PigHousing::~PigHousing() {}

PigHousing::PigHousing(const PigHousing & aStable):
    stable(aStable)
{
    theMessage -> FatalError("PigHousing::PigHousing has not been used for a while");

    sections = new linkList<animalSection>();

    for (int i = 0; i < aStable.sections -> NumOfNodes(); i++) {
        animalSection * p = new pigstysection(*(pigstysection *) (aStable.sections -> ElementAtNumber(i)));

        sections -> InsertLast(p);
        p -> GetContains();
    }

    standardSections = new linkList<animalSection>();

    for (int i = 0; i < aStable.standardSections -> NumOfNodes(); i++) {
        animalSection * p = new pigstysection(*(aStable.standardSections -> ElementAtNumber(i)));

        standardSections -> InsertLast(p);
    }
}

/*
 * Update the stable
 */
void PigHousing::DailyUpdate() {
    stable::DailyUpdate();

    // double hours = GetLabour();
    // electricity is commented out until I understand how it is calculated

    /*
     *   theOutput->AddIndicator("07.02 Labour, stable","hours",hours);
     * theOutput->AddIndicator("07.03 Labour, maintance and management","hours",0.20*hours);  // 5% for building maintance + 15% for management
     * theOutput->AddIndicator("07.04 Total labour","hours",hours*1.20);
     */
}

/*
 * Minimum ventilation in cubic metres per sec
 */
double PigHousing::CalcMinimumVentilationRate() {
    double retVal = 0.0;

    for (int i = 0; i < sections -> NumOfNodes(); i++) {
        pigstysection * p = (pigstysection *) sections -> ElementAtNumber(i);

        retVal += p -> GetNrOfAnimals() * p -> GetminVentilation();
    }

    return retVal;
}

/*
 * Maximum ventilation in cubic metres per sec
 */
double PigHousing::CalcMaximumVentilationRate() {
    double retVal = 0.0;

    for (int i = 0; i < sections -> NumOfNodes(); i++) {
        pigstysection * p = (pigstysection *) sections -> ElementAtNumber(i);

        retVal += p -> GetNrOfAnimals() * p -> GetmaxVentilation();
    }

    return retVal;
}

;

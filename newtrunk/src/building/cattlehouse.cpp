
#include "../base/common.h"
#include "../base/IndicatorOutput.h"
#include "cattlehouse.h"
#include "../livestock/dairyStock.h"
#include "../base/controlParameters.h"
#include "animalSection.h"
#include "../base/commonData.h"

cattlehouse::cattlehouse(string       aname,
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
    Livestock         = new dairyStock(aname, aIndex, aOwner);
    insideTemperature = theClimate -> GetMeanTemperature();

    string sectionsName(aname);

    sectionsName = sectionsName + "(" + conv.IntToString(aIndex) + ").Section";

    int large = 0;
    int small = 4444;

    runData -> getSectionsNumbers(sectionsName, &small, &large);

    for (int inx = small; inx <= large; inx++) {
        animalSection * aSection = new animalSection(sectionsName, inx, this, runData, common,
                                       controlledVent);

        sections -> InsertLast(aSection);
    }

    for (int i = 0; i < inhouseStores -> NumOfNodes(); i++) {
        manurestore * aStore = inhouseStores -> ElementAtNumber(i);

        for (int i = 0; i < sections -> NumOfNodes(); i++) {
            animalSection * aSection = sections -> ElementAtNumber(i);

            aSection -> LinkManureStores(aStore);
        }
    }

    if (theControlParameters -> GetvolatModel() > 1) {
        checkArea();
    }
}

cattlehouse::cattlehouse(const cattlehouse & cattlehouse):
    stable(cattlehouse)
{
    theMessage -> FatalError("cattlehouse::cattlehouse has not been used for a while");

    Livestock = new dairyStock(*cattlehouse.Livestock);
}

cattlehouse::~cattlehouse() {}

void cattlehouse::DailyUpdate() {
    stable::DailyUpdate();
}


#include "../base/common.h"
#include "biogasReactor.h"
#include "../base/IndicatorOutput.h"
#include "../base/climate.h"
#include "../base/controlParameters.h"
#include "../base/nixcbits.h"
#include "../base/settings.h"

biogasReactor::~biogasReactor() {
    external = true;
}

biogasReactor::biogasReactor(string       aName,
                             const int    aIndex,
                             const base * aOwner,
                             commonData * runData,
                             commonData * common):
    manurestore(aName,
                aIndex,
                aOwner,
                runData,
                common),
    building(aName,
             aIndex,
             aOwner,
             runData,
             common)
{
    if (globalSettings -> DetailsData.getManureManageDetails()) {
        OpenDetailsFile();
    } else {
        detailsFile = nullptr;
    }
}

/*
 * Dailyactions
 */
void biogasReactor::DailyUpdate() {
    building::DailyUpdate();

    /*
     *    theOutput->AddIndicator("35.43 NH3-N volatilisation from manure storage","kg N",volatAmmonia*1000.0);
     * theOutput->AddIndicator("35.44 N2-N volatilisation from manure storage","kg N",N2Emission*1000.0);
     * theOutput->AddIndicator("35.45 N2O-N volatilisation from manure storage","kg N",N2OEmission*1000.0);
     * theOutput->AddIndicator("45.43 CO2-C volatilisation from manure storage","kg C",CO2Emission*1000.0);
     * theOutput->AddIndicator("45.44 CH4-C volatilisation from manure storage","kg C",CH4Emission*1000.0);
     */
}

void biogasReactor::GiveIndicator(int indicatorType) {
    if (indicatorType == environmentalIndicator) {

        /*
         * theOutput->AddStateIndicator("35.60 N bound in manure storage","kg N",N*1000.0);
         * theOutput->AddStateIndicator("45.60 C bound in manure storage","kg C",C*1000.0);
         * theOutput->AddStateIndicator("11.70 Slurry in biogasReactor","t",Vt/1000);
         */
    }
}

/*
 *       Scale the manure storage that receives material from this instore  Note the cumulative effect on capacity
 */
void biogasReactor::Scale(double annualAmount) {
    double newCapacity = annualAmount + GetCapacity();

    manurestore::Scale(newCapacity);    // scales capcity and area via manurestore
}

ostream & operator <<(ostream &     os,
                      biogasReactor s) {

    /*
     *  os << setiosflags(ios::left) << setw(40) << s.rT << endl;
     * os << setiosflags(ios::left) << setw(40) << "Cover resistance " << s.rCover << endl;
     */
    return os;
}

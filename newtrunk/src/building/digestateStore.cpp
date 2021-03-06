/*
 *  Describes a separate free-standing store)
 */

#include "../base/common.h"
#include "digestateStore.h"
#include "../base/IndicatorOutput.h"
#include "../base/climate.h"
#include "../base/controlParameters.h"
#include "../base/nixcbits.h"
#include "../base/settings.h"

digestateStore::~digestateStore() {
    external = true;
}

digestateStore::digestateStore(string       aName,
                               const int    aIndex,
                               const base * aOwner,
                               commonData * runData,
                               commonData * common):
    slurrystore(aName,
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

digestateStore::digestateStore(digestateStore & s):
    slurrystore(s)
{
    external = s.external;
}

/*
 *      Dailyactions
 */
void digestateStore::DailyUpdate() {
    building::DailyUpdate();

    /*
     *   manurestore::DailyUpdate(volatAmmonia, N2Emission, N2OEmission, CO2Emission, CH4Emission);
     * theOutput->AddIndicator("35.43 NH3-N volatilisation from manure storage","kg N",volatAmmonia*1000.0);
     * theOutput->AddIndicator("35.44 N2-N volatilisation from manure storage","kg N",N2Emission*1000.0);
     * theOutput->AddIndicator("35.45 N2O-N volatilisation from manure storage","kg N",N2OEmission*1000.0);
     * theOutput->AddIndicator("45.43 CO2-C volatilisation from manure storage","kg C",CO2Emission*1000.0);
     * theOutput->AddIndicator("45.44 CH4-C volatilisation from manure storage","kg C",CH4Emission*1000.0);
     */
    GiveIndicator(environmentalIndicator);
}

void digestateStore::GiveIndicator(int indicatorType) {
    if (indicatorType == environmentalIndicator) {

        /*
         *       theOutput->AddStateIndicator("35.60 N bound in manure storage","kg N",N*1000.0);
         * theOutput->AddStateIndicator("45.60 C bound in manure storage","kg C",C*1000.0);
         * theOutput->AddStateIndicator("11.70 Slurry in digestateStore","t",Vt/1000);
         */
    }
}

/*
 * Calling manurestore::RcvManure(const manure* someManure)
 */
void digestateStore::RcvManure(const manure * someManure) {
    manurestore::RcvManure(someManure);

    // theOutput->AddIndicator("19.72 Biogas slurry produced","t",someManure->GetAmount());
}

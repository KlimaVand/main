/*
 *
 *  Author: Lars Kroll Kristensen
 * Date: 1997
 * Revision: Bj�rn Molt Petersen February 1998
 *
 */

// #include <common.h>

#include "genAsset.h"

#ifdef _STANDALONE

#include "../base/common.h"

#else

#include "../base/IndicatorOutput.h"

#endif

GenAsset::GenAsset() {
    NewValue            = 0;
    AverageValuePercent = 0;
    MaintenancePercent  = 0;
    InsurancePercent    = 0;
    DepreciationPercent = 0;
    ActualCapacity      = 0.0;
    RefCapacity         = 0.0;
    scalable            = false;
}

/*
 * Loads the user defines values into fasset
 */
void GenAsset::initEconomy(commonData *& aBase) {
    aBase -> setCritical(true);
    aBase -> FindItem("NewValue", &NewValue);
    aBase -> FindItem("AverageValue", &AverageValuePercent);
    aBase -> FindItem("Maintenance", &MaintenancePercent);
    aBase -> FindItem("Insurance", &InsurancePercent);
    aBase -> FindItem("Depreciation", &DepreciationPercent);

    if (scalable) {
        aBase -> FindItem("RefCapacity", &RefCapacity);
    }

    aBase -> setCritical(false);
}

/*
 * Update indicators
 */
void GenAsset::GiveEconomicIndicators() {

#ifndef _STANDALONE
    theOutput -> AddIndicator(economicIndicator, "07.02 Depreciation buildings & inventory", "Dkr",
                              -getDepreciationValue());
    theOutput -> AddIndicator(economicIndicator, "07.04 Insurance buildings & inventory", "Dkr", -getInsuranceValue());
    theOutput -> AddIndicator(economicIndicator, "07.05 Maintenance buildings & inventory", "Dkr",
                              -getMaintenanceValue());
    theOutput -> AddIndicator(economicIndicator, "03.02 Value of buildings & inventory", "Dkr", getTotalValue());
#endif

}

double GenAsset::getMaintenanceValue() {
    if (!scalable) {
        return (NewValue * MaintenancePercent);
    } else {
        return (ActualCapacity / RefCapacity) * (NewValue * MaintenancePercent);
    }
}

double GenAsset::getInsuranceValue() {
    if (!scalable) {
        return (NewValue * InsurancePercent);
    } else {
        return (ActualCapacity / RefCapacity) * (NewValue * InsurancePercent);
    }
}

double GenAsset::getDepreciationValue() {
    if (!scalable) {
        return (NewValue * DepreciationPercent);
    } else {
        return (ActualCapacity / RefCapacity) * (NewValue * DepreciationPercent);
    }
}

double GenAsset::getTotalValue() {
    if (!scalable) {
        return (NewValue * AverageValuePercent);
    } else {
        return (ActualCapacity / RefCapacity) * (NewValue * AverageValuePercent);
    }
}

/*
 *  Author   Lars Noboe Andersen & Poul Lassen
 * Date
 * Revision   Bj�rn Molt Petersen, May-June 1997
 */

#include "../base/common.h"
#include "techFarm.h"

techFarm::techFarm(string       aName,
                   const int    aIndex,
                   const base * aOwner):
    techEquip(aName,
              aIndex,
              aOwner)
{
    reqWorker       = 0;
    age             = 0;
    deprPercent     = 0.0;
    hoursPrimo      = 0;
    hoursUltimo     = 0;
    lifetimeHours   = 0;
    lifetimeYears   = 0;
    maintenancePrm  = 0.0;
    insurancePrm    = 0.0;
    newValue        = 0.0;
    percentNewValue = 1.0;
    deprK           = 0.0;
    deprKHours      = 0.0;
    deprKLifetime   = 0.0;
}

techFarm::techFarm(const techFarm & atechFarm):
    techEquip(atechFarm)
{
    reqWorker       = atechFarm.reqWorker;
    age             = atechFarm.age;
    deprPercent     = atechFarm.deprPercent;
    hoursPrimo      = atechFarm.hoursPrimo;
    hoursUltimo     = atechFarm.hoursUltimo;
    lifetimeHours   = atechFarm.lifetimeHours;
    lifetimeYears   = atechFarm.lifetimeYears;
    maintenancePrm  = atechFarm.maintenancePrm;
    insurancePrm    = atechFarm.insurancePrm;
    newValue        = atechFarm.newValue;
    percentNewValue = atechFarm.percentNewValue;
    deprK           = atechFarm.deprK;
    deprKHours      = atechFarm.deprKHours;
    deprKLifetime   = atechFarm.deprKLifetime;
}

techFarm::~techFarm() {}

void techFarm::ReadParameters(commonData *& file) {
    techEquip::ReadParameters(file);
    file -> FindItem("deprPercent", &deprPercent);
    file -> FindItem("insurancePrm", &insurancePrm);
    file -> FindItem("newValue", &newValue);
    file -> FindItem("percentNewValue", &percentNewValue);
    file -> FindItem("size", &size);
    file -> FindItem("carryCapacity", &carryCapacity);
    file -> FindItem("maintenancePrm", &maintenancePrm);
    file -> FindItem("age", &age);
    file -> FindItem("hoursPrimo", &hoursPrimo);
    file -> FindItem("hoursUltimo", &hoursUltimo);
    file -> FindItem("lifetimeHours", &lifetimeHours);
    file -> FindItem("lifetimeYears", &lifetimeYears);
    file -> FindItem("reqWorker", &reqWorker);
    file -> FindItem("deprK", &deprK);
    file -> FindItem("deprKHours", &deprKHours);
    file -> FindItem("deprKLifetime", &deprKLifetime);
}

void techFarm::UpdateHours(int aInvestStrategi) {
    if (aInvestStrategi == dynamicReinvest) {
        hoursPrimo = hoursUltimo;
    }

    hoursUltimo = 0;
}

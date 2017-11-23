/*
 *  Author   Lars Noboe Andersen & Poul Lassen
 * Revision   Bjørn Molt Petersen, May-June 1997
 */

#include "../base/common.h"
#include "selfPropelled.h"

selfPropelled::selfPropelled(string       aName,
                             const int    aIndex,
                             const base * aOwner):
    techEquip(aName,
              aIndex,
              aOwner)
{
    fuelConsumption          = 0.0;
    fuelConsumptionTransport = 0.0;
    hp                       = 0.0;
    netCapacityWheat         = 0;
}

selfPropelled::selfPropelled(const selfPropelled & aselfPropelled):
    techEquip(aselfPropelled)
{
    netCapacityWheat         = 0;
    fuelConsumption          = aselfPropelled.fuelConsumption;
    fuelConsumptionTransport = aselfPropelled.fuelConsumptionTransport;
    hp                       = aselfPropelled.hp;
}

selfPropelled::~selfPropelled() {}

void selfPropelled::ReadParameters(commonData *& file) {
    techEquip::ReadParameters(file);
    file -> FindItem("hp", &hp);
    file -> FindItem("fuelConsumption", &fuelConsumption);
    file -> FindItem("netCapacityWheat", &netCapacityWheat);
    file -> FindItem("fuelConsumptionTransport", &fuelConsumptionTransport);
}

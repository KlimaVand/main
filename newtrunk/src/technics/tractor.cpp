/*
 *  Author   Lars Noboe Andersen & Poul Lassen
 * Revision   Bjørn Molt Petersen, May-June 1997
 */

#include "../base/common.h"
#include "tractor.h"

tractor::tractor(string       aName,
                 const int    aIndex,
                 const base * aOwner):
    techEquip(aName,
              aIndex,
              aOwner)
{
    hp                       = 0.0;
    fuelConsumptionTransport = 0.0;
}

tractor::tractor(const tractor & atractor):
    techEquip(atractor)
{
    hp                       = atractor.hp;
    fuelConsumptionTransport = atractor.fuelConsumptionTransport;
}

tractor::~tractor() {}

void tractor::ReadParameters(commonData *& file) {
    techEquip::ReadParameters(file);
    file -> FindItem("hp", &hp);
    file -> FindItem("fuelConsumptionTransport", &fuelConsumptionTransport);
}

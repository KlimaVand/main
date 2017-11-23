/*
 *  Author   Lars Noboe Andersen & Poul Lassen
 * Revision   Bjørn Molt Petersen, May-June 1997
 */

#include "../base/common.h"
#include "implement.h"

implement::implement(string       aName,
                     const int    aIndex,
                     const base * aOwner):
    techEquip(aName,
              aIndex,
              aOwner)
{
    fuelConsumption = 0.0;
}

implement::implement(const implement & aimplement):
    techEquip(aimplement)
{
    fuelConsumption = aimplement.fuelConsumption;
}

implement::~implement() {}

void implement::ReadParameters(commonData *& file) {
    techEquip::ReadParameters(file);
    file -> FindItem("fuelConsumption", &fuelConsumption);
}

/*
 *  Author   Lars Noboe Andersen & Poul Lassen
 * Revision   Bjørn Molt Petersen, May-June 1997
 */

#include "../base/common.h"
#include "installation.h"

installation::installation(string       aName,
                           const int    aIndex,
                           const base * aOwner):
    techEquip(aName,
              aIndex,
              aOwner)
{
    capacity               = 0.0;
    electricityConsumption = 0.0;
}

installation::installation(const installation & ainstallation):
    techEquip(ainstallation)
{
    capacity               = ainstallation.capacity;
    electricityConsumption = ainstallation.electricityConsumption;
}

installation::~installation() {}

void installation::ReadParameters(commonData *& file) {
    techEquip::ReadParameters(file);
    file -> FindItem("electricityConsumption", &electricityConsumption);
    file -> FindItem("capacity", &capacity);
}

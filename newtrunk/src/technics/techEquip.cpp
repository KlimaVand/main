/*
 * Author   Lars Noboe Andersen & Poul Lassen
 * Revision   Bj�rn Molt Petersen, May-June 1997
 */

#include "../base/common.h"
#include "techEquip.h"
#include "../base/IndicatorOutput.h"
#include <string.h>
#include "../base/commonData.h"

techEquip::techEquip(string       aName,
                     const int    aIndex,
                     const base * aOwner):
    base(aName,
         aIndex,
         aOwner)
{
    techEquipName = "";
    size          = 0.0;
    carryCapacity = 0.0;
    age           = 0;
    hoursPrimo    = 0;
    hoursUltimo   = 0;
}

techEquip::techEquip(const techEquip & atechEquip):
    base(atechEquip)
{
    techEquipName = atechEquip.techEquipName;
    size          = atechEquip.size;
    carryCapacity = atechEquip.carryCapacity;
    age           = atechEquip.age;
    hoursPrimo    = atechEquip.hoursPrimo;
    hoursUltimo   = atechEquip.hoursUltimo;
}

techEquip::~techEquip() {}

void techEquip::ReadParameters(commonData *& file) {
    string section = Name;

    if (file -> FindSection(section, Index)) {
        file -> FindItem("techEquipName", &techEquipName);
        file -> FindItem("size", &size);
        file -> FindItem("carryCapacity", &carryCapacity);
        file -> setCritical(false);
    } else {
        theMessage -> FatalError("techEquip::ReadParameters - ", section, " not found");
    }
}

double techEquip::getHoursUsed() {
    return GethoursUltimo() - GethoursPrimo();
}

void techEquip::UpdateHours() {
    hoursPrimo  = 0.0;
    hoursUltimo = 0.0;
}

/*
 *       Should be called once a year
 */
void techEquip::GiveIndicators() {
    GiveEconomicIndicators();

    hoursPrimo = hoursUltimo;
}

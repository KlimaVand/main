/*
 * Created by Nick Hutchings, Sept 2006
 */

#include "../base/common.h"
#include "manureApplic.h"

manureApplic::manureApplic(string       aName,
                           const int    aIndex,
                           const base * aOwner):
    techEquip(aName,
              aIndex,
              aOwner)
{
    propBuried        = 0.0;
    propAreaWetted    = 1.0;
    volume            = 1E30;
    cropContamination = true;
    NH3emissionFactor = 0.0;
}

manureApplic::manureApplic(const manureApplic & amanureApplic):
    techEquip(amanureApplic)
{
    NH3emissionFactor = 0;
    volume            = 0;
    propAreaWetted    = propBuried = 0;
    cropContamination = 0;

    theMessage -> FatalError("tetemanure.cpp::attempt to use undefined copy constructor");
}

manureApplic::~manureApplic() {}

void manureApplic::ReadParameters(commonData *& file) {
    techEquip::ReadParameters(file);
    file -> FindItem("propBuried", &propBuried);
    file -> FindItem("propAreaWetted", &propAreaWetted);
    file -> FindItem("volume", &volume);
    file -> FindItem("NH3emissionFactor", &NH3emissionFactor);

    int dumint = 0;

    file -> FindItem("cropContamination", &dumint);

    if (dumint == 1) {
        cropContamination = true;
    } else {
        cropContamination = false;
    }
}

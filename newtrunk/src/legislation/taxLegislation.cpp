
#include "../base/common.h"
#include "taxLegislation.h"

taxLegislation::taxLegislation():
    base()
{
    InitVariables();
}

taxLegislation::taxLegislation(string       aName,
                               const int    aIndex,
                               const base * aOwner):
    base(aName,
         aIndex,
         aOwner)
{
    InitVariables();
}

taxLegislation::taxLegislation(int          aType,
                               double       aTax,
                               int          aStartYear,
                               int          aStopYear,
                               string       aName,
                               const int    aIndex,
                               const base * aOwner):
    base(aName,
         aIndex,
         aOwner)
{
    InitVariables();

    type      = aType;
    tax       = aTax;
    startYear = aStartYear;
    stopYear  = aStopYear;
}

taxLegislation::~taxLegislation() {}

void taxLegislation::InitVariables() {
    type      = 0;
    tax       = 0;
    startYear = 0;
    stopYear  = 0;
    level     = 0;
}

void taxLegislation::ReadParameters(commonData *& file) {
    file -> setCritical(true);
    file -> FindSection(Name, Index);
    file -> FindItem("type", &type);
    file -> FindItem("tax", &tax);
    file -> FindItem("startYear", &startYear);
    file -> FindItem("stopYear", &stopYear);
    file -> FindItem("level", &level);
}

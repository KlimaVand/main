
#include "../base/common.h"
#include "manualLabourTech.h"

manualLabourTech::manualLabourTech(string       aName,
                                   const int    aIndex,
                                   const base * aOwner):
    fieldOperationTech(aName,
                       aIndex,
                       aOwner)
{
    hoursPerUnit = 0.0;
}

manualLabourTech::~manualLabourTech() {}

void manualLabourTech::ReadParameters(commonData *& file) {
    fieldOperationTech::ReadParameters(file);
}

void manualLabourTech::CalcCost(calcLPType mode) {
    CalcGangHours();

    cost = aGang -> GetgangHours();

#ifdef DEVELOPING
#endif

}

void manualLabourTech::CalcGangHours() {
    aGang -> SetgangHours(hoursPerUnit * area);
}

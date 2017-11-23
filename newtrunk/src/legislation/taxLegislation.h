
#ifndef __TAXLEGIS_H
#define __TAXLEGIS_H

#include "../base/base.h"
#include "../base/commonData.h"

class taxLegislation:
    public base
{
    protected:
        int    type;         // 0: 1: 2: 3:
        double tax;          // the tax
        double level;
        int    startYear;    // the first simulation year the tax should be applied
        int    stopYear;     // the last simulation year the tax should be applied

        void InitVariables();

    public:
        taxLegislation();

        taxLegislation(string       aName,
                       const int    aIndex = -1,
                       const base * aOwner = nullptr);

        taxLegislation(int          aType,
                       double       tax,
                       int          startYear,
                       int          stopYear,
                       string       aName,
                       const int    aIndex = -1,
                       const base * aOwner = nullptr);

        ~taxLegislation();

        // Set and Get functions
        bool taxThisYear(int simulationYear) {
            return ((simulationYear >= startYear) && (simulationYear <= stopYear));
        }

        double getTax() {
            return tax;
        }

        double getLevel() {
            return level;
        }

        int getType() {
            return type;
        }

        void ReadParameters(commonData *& file);
};
#endif


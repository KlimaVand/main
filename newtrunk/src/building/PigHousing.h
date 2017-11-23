
#ifndef PIGSTABLE_H
#define PIGSTABLE_H

#include "../base/common.h"
#include "stable.h"
#include "pigstysection.h"

class swineStock;


class PigHousing:
    public stable
{
    private:
        PigHousing() {}

    public:
        PigHousing(string       aname,
                   const int    aIndex,
                   const base * aOwner,
                   commonData * runData,
                   commonData * common);

        PigHousing(const PigHousing & aStable);

        virtual ~PigHousing();

        virtual void DailyUpdate();

        virtual double CalcMinimumVentilationRate();

        virtual double CalcMaximumVentilationRate();
};
#endif


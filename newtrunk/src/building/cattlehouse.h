
#ifndef CATSTABLE_H
#define CATSTABLE_H

#include "../base/common.h"
#include "stable.h"

class dairyStock;


class cattlehouse:
    public stable
{
    dairyStock * Livestock;

    public:
        cattlehouse(string       aname,
                    const int    aIndex,
                    const base * aOwner,
                    commonData * runData,
                    commonData * common);

        ~cattlehouse();

        cattlehouse(const cattlehouse & acattlehouse);

        virtual void DailyUpdate();

        double GetThermaHeatIndex(double airTemperature,
                                  double relativeHumidity);
};
#endif


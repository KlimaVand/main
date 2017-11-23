/*
 *
 *  Author   Lars Noboe Andersen & Poul Lassen
 * Date
 * Revision   Bjørn Molt Petersen, May-June 1997
 *
 */

#ifndef __TETETRAC_H
#define __TETETRAC_H

#include "techEquip.h"

/*
 *
 * Class: tractor
 *
 */
class tractor:
    public techEquip
{

    /* Attributes */
    protected:
        double hp;
        double fuelConsumptionTransport;

        /* Actions */

    private:
        tractor();    // Dissable use of default constructor

    public:
        tractor(string       aName,
                const int    aIndex = -1,
                const base * aOwner = nullptr);

        tractor(const tractor & atractor);

        virtual ~tractor();

        virtual void ReadParameters(commonData *& file);

        double GetFuelConsumptionTransport() {
            return fuelConsumptionTransport;
        }
};
#endif


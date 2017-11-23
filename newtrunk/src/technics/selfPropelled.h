/*
 *
 *  Author   Lars Noboe Andersen & Poul Lassen
 * Date
 * Revision   Bjørn Molt Petersen, May-June 1997
 *
 */

#ifndef __TETESELF_H
#define __TETESELF_H

#include "techEquip.h"

/*
 *
 * Class: selfPropelled
 *
 */
class selfPropelled:
    public techEquip
{

    /* Attributes */
    protected:
        double fuelConsumption;
        double fuelConsumptionTransport;
        double netCapacityWheat;
        double hp;

        /* Actions */

    private:
        selfPropelled();    // Dissable use of default constructor

    public:
        selfPropelled(string       aName,
                      const int    aIndex = -1,
                      const base * aOwner = nullptr);

        selfPropelled(const selfPropelled & aselfPropelled);

        virtual ~selfPropelled();

        virtual void ReadParameters(commonData *& file);

        double GetfuelConsumption() {
            return fuelConsumption;
        }

        double GetFuelConsumptionTransport() {
            return fuelConsumptionTransport;
        }

        double GetNetCapacityWheat() {
            return netCapacityWheat;
        }
};
#endif


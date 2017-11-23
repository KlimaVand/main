/*
 *
 *  Author   Lars Noboe Andersen & Poul Lassen
 * Date
 * Revision   Bjørn Molt Petersen, May-June 1997
 *
 */

#ifndef __TETEIMPL_H
#define __TETEIMPL_H

#include "techEquip.h"

/*
 *
 * Class: implement
 *
 */
class implement:
    public techEquip
{

    /* Attributes */
    protected:
        double fuelConsumption;

        /* Actions */

    private:
        implement();    // Dissable the use of default constructor

    public:
        implement(string       aName,
                  const int    aIndex,
                  const base * aOwner = nullptr);

        implement(const implement & aimplement);

        virtual ~implement();

        virtual void ReadParameters(commonData *& file);

        double GetfuelConsumption(void) {
            return fuelConsumption;
        }
};
#endif


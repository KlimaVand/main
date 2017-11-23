/*
 *
 *  Author   Lars Noboe Andersen & Poul Lassen
 * Date
 * Revision   Bjørn Molt Petersen, May-June 1997
 *
 */

#ifndef __TETEINST_H
#define __TETEINST_H

#include "techEquip.h"

/*
 *
 * Class: installation
 *
 */
class installation:
    public techEquip
{

    /* Attributes */
    protected:
        double capacity;
        double electricityConsumption;

        /* Actions */

    private:
        installation();    // Dissable the use of default constructor

    public:
        installation(string       aName,
                     const int    aIndex,
                     const base * aOwner = nullptr);

        installation(const installation & ainstallation);

        virtual ~installation();

        virtual void ReadParameters(commonData *& file);

        double GetelectricityConsumption(void) {
            return electricityConsumption;
        }

        double Getcapacity(void) {
            return capacity;
        }
};
#endif


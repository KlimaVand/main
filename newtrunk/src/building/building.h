// !Superclass for all buildings

/*
 * !Superclass for all buildings.  It should not be instanced.
 */

#ifndef BUILDING_H
#define BUILDING_H

#include "../base/commonData.h"
#include "../base/base.h"
#include "../tools/convert.h"

/*
 *
 * Class: building
 *
 */
class building:
    public base
{

    /* Attributes */
    private:

        // ! Default Constructor

    protected:
        bool scalable;

        building();

        // ! Usable groundarea in sqr(m), usable for whatever
        double Area;

        // !Identifier to allow the building to be located by a GIS
        int location;

        /* Actions */

    private:

    public:
        void SetScalable(bool scale) {
            scalable = scale;
        }

        building(string        aName,
                 const int     Aindex,
                 const base *  aOwner,
                 commonData *& runData,
                 commonData *& common);

        building(const building & aBuilding);

        ~building();

        // ! Daily actions should be called every day by manager
        virtual void DailyUpdate();

        // !Return the location identifier
        int Getlocation() {
            return location;
        }

        // !Set the location identifier

        /*
         * !
         * param aVal the value to which the location identifier should be set
         */
        void Setlocation(int aVal) {
            location = aVal;
        }
};
#endif


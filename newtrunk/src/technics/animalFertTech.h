/*
 *
 * Author
 * Date
 * Revision   Bj�rn Molt Petersen, May 1997
 *
 */

#ifndef __TEOPFIAN_H
#define __TEOPFIAN_H

#include "../products/manure.h"
#include "../products/products.h"
#include "fieldOperationTech.h"

/*
 *
 * Class: animalFertTech
 *
 */
class animalFertTech:
    public fieldOperationTech
{

    /* Attributes */
    protected:
        double prepLoading;
        double roadSpeed;
        double fuelPerAmount;
        double spreadingCapacity;
        double loadingCapacity;
        double prepForSpreading;
        int    f;
        int    g;
        double j;

        // int appMethodNo;    //unique number used to identify the application method when estimating ammonia volatilisation
        manure * theManure;

        /* Actions */

    private:
        animalFertTech();                            // Dissable use of default constructor

        animalFertTech(const animalFertTech & a);    // Dissable the compilers generation of default copy constructor.

        animalFertTech & operator = (
                const animalFertTech & a);    // Dissable the compilers generation of default assignment operator.

    protected:
        virtual void CalcGangHours();

        virtual void CalcGangFuel();

    public:
        animalFertTech(string       aName,
                       const int    aIndex = -1,
                       const base * aOwner = nullptr);

        virtual ~animalFertTech();

        virtual void ReadParameters(commonData *& file);

        void SetTheManure(manure * aManure);

        void ClearTheManure();

        virtual void CalcCost(calcLPType mode);

        techEquip * GetApplicationTech();
};
#endif


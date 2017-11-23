/*
 *
 *  Author   Lars Noboe Andersen & Poul Lassen
 * Date
 * Revision   Bjørn Molt Petersen, May-June 1997
 *
 */

#ifndef __TEOPFIPC_H
#define __TEOPFIPC_H

#include "../products/water.h"
#include "../products/chemical.h"
#include "../products/products.h"
#include "fieldOperationTech.h"

/*
 *
 * Class: plantCareTech
 *
 */
class plantCareTech:
    public fieldOperationTech
{

    /* Attributes */
    protected:
        double     roadSpeed;
        double     specificArea;
        double     fillingTime;
        double     prepFillingTank;
        double     treatmentFrequency1;
        double     treatmentFrequency2;
        water *    theWater;
        chemical * theChemical1;
        chemical * theChemical2;
        chemical * theChemical3;

        /* Actions */

    private:
        plantCareTech();                           // Dissable the use of default constructor

        plantCareTech(const plantCareTech & p);    // Dissable the compilers generation of default copy constructor.

        plantCareTech & operator = (
                const plantCareTech & p);          // Dissable the compilers generation of default assignment operator.

    protected:
        virtual void CalcGangHours();

    public:
        plantCareTech(string       aName,
                      const int    aIndex,
                      const base * aOwner = nullptr);

        virtual ~plantCareTech();

        virtual void ReadParameters(commonData *& file);

        void SetTheWaterAndChemicals(water *    aWater,
                                     chemical * aChemical1,
                                     chemical * aChemical2,
                                     chemical * aChemical3);

        void ClearTheWaterAndChemicals();

        virtual void CalcCost(calcLPType mode);

        virtual void UpdateProducts(bool contractor = false);

        virtual void UpdateChemical(chemical * aChemical);
};
#endif


/*
 *
 *  Author   Lars Noboe Andersen & Poul Lassen
 * Date
 * Revision   Bjørn Molt Petersen, May-June 1997
 *
 */

#ifndef __TEOPFIST_H
#define __TEOPFIST_H

#include "fieldOperationTech.h"

/*
 *
 * Class: soilTreatTech
 *
 */
class soilTreatTech:
    public fieldOperationTech
{

    /* Attributes */
    protected:
        int    k;
        double firstFurrow;
        double markHeadland;

        /* Actions */

    private:
        soilTreatTech();                           // Dissable the use of default constructor

        soilTreatTech(const soilTreatTech & s);    // Dissable the compilers generation of default copy constructor.

        soilTreatTech & operator = (
                const soilTreatTech & s);          // Dissable the compilers generation of default assignment operator.

    protected:
        virtual void CalcGangHours();

    public:
        soilTreatTech(string       aName,
                      const int    aIndex,
                      const base * aOwner = nullptr);

        virtual ~soilTreatTech();

        virtual void ReadParameters(commonData *& file);

        virtual void CalcCost(calcLPType mode);

        virtual void UpdateProducts(bool contractor = false);

        int SameConditionsSoilTreat(double area,
                                    double distance) {
            return ((area == oldArea) && (distance == oldDistance));
        }

        double OldCost() {
            return oldCost;
        }
};
#endif


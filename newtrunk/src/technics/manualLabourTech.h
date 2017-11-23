/*
 *
 *  Author   Lars Noboe Andersen & Poul Lassen
 * Date
 * Revision   Bjørn Molt Petersen, May-June 1997
 *
 */

#ifndef __TEOPFIML_H
#define __TEOPFIML_H

#include "fieldOperationTech.h"

/*
 *
 * Class: manualLabourTech
 *
 */
class manualLabourTech:
    public fieldOperationTech
{

    /* Attributes */
    protected:
        double hoursPerUnit;

        /* Actions */

    private:
        manualLabourTech();                     // Dissable the use of default constructor

        manualLabourTech(const manualLabourTech & m);    // Dissable the compilers generation of default copy constructor.

        manualLabourTech & operator = (
                const manualLabourTech & m);    // Dissable the compilers generation of default assignment operator.

    protected:
        virtual void CalcGangHours();

    public:
        manualLabourTech(string       aName,
                         const int    aIndex,
                         const base * aOwner = nullptr);

        virtual ~manualLabourTech();

        virtual void ReadParameters(commonData *& file);

        virtual void CalcCost(calcLPType mode);

        void SetHoursPerUnit(const double HPU) {
            hoursPerUnit = HPU;
        }
};
#endif


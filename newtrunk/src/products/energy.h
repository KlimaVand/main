/*
 *
 * Revision                       july major revision of all product's.         (JB)
 * All operators calls the superclass
 * operator check's
 * if similar types, storage etc  added/subtracted
 * if amount=0 the object gets the same unit, storage etc
 * as the object that is added to it
 *
 */

#ifndef __PENERGY_H
#define __PENERGY_H

#include "product.h"

class energy:
    public product
{
    protected:
        double kWhPerUnit;    // Unit: kWh

        void InitVariables();

    public:
        energy();

        energy(string       aName,
               const int    aIndex = -1,
               const base * aOwner = nullptr);

        energy(const energy & aenergy);

        virtual ~energy();

        double GetkWhPerUnit(void) const {
            return kWhPerUnit;
        }

        virtual product & operator = (const product & someEnergy);

        virtual product & operator +(const product & someEnergy);

        virtual product & operator -(const product & someEnergy);

        virtual product * clone() const;

        virtual void ReadParameters(commonData * data,
                                    string       sectionName);
};
#endif


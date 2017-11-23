/*
 * Revision                       july major revision of all product's.         (JB)
 * All operators calls the superclass
 * operator check's
 * if similar types, storage etc  added/subtracted
 * if amount=0 the object gets the same unit, storage etc
 * as the object that is added to it
 */

#ifndef __PWATER_H
#define __PWATER_H

#include "product.h"

// ! water class in fasset
class water:
    public product
{
    protected:

        // ! fraction of N in water
        double N_content;    // Unit: fraction

        void InitVariables();

    public:
        water();

        water(string       aName,
              const int    aIndex = -1,
              const base * aOwner = nullptr);

        water(const water & awater);

        virtual ~water();

        void SetN_content(double aN_content) {
            N_content = aN_content;
        }

        // Get functions
        double GetN_content(void) const {
            return N_content;
        }

        virtual product & operator = (const product & someWater);

        virtual product & operator +(const product & someWater);

        virtual product & operator -(const product & someWater);

        virtual product * clone() const;

        virtual void ReadParameters(commonData * data,
                                    string       sectionName);
};
#endif


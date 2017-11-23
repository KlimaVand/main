/*
 * Revision                       july major revision of all product's.         (JB)
 * All operators calls the superclass
 * operator check's
 * if similar types, storage etc  added/subtracted
 * if amount=0 the object gets the same unit, storage etc
 * as the object that is added to it
 *
 */

#ifndef __PSEED_H
#define __PSEED_H

#include "liveMatter.h"

class seed:
    public liveMatter
{
    protected:

        // ! Standard sowing density Unit: g/m2
        double standardDensity;

        void InitVariables();

    public:
        seed();

        seed(string       aName,
             const int    aIndex = -1,
             const base * aOwner = nullptr);

        seed(const seed & aseed);

        virtual ~seed();

        friend ostream & operator <<(ostream &    os,
                                     const seed & aseed);

        void SetN_content(double & aN_content) {
            orgN_content = aN_content;
        }

        // Get functions
        double GetN_content(void) const {
            return orgN_content.n;
        }

        double GetStandardDensity(void) const {
            return standardDensity;
        }

        virtual product & operator = (const product & someSeed);

        virtual product & operator +(const product & someSeed);

        virtual product & operator -(const product & someSeed);

        virtual product * clone() const;

        virtual void ReadParameters(commonData * data,
                                    string       sectionName);
};
#endif


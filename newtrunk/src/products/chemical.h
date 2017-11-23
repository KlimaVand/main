/*
 *
 * Revision               july major revision of all product's.         (JB)
 * All operators calls the superclass
 * operator check's
 * if similar types, storage etc  added/subtracted
 * if amount=0 the object gets the same unit, storage etc
 * as the object that is added to it
 *
 */

#ifndef __PCHEMIC_H
#define __PCHEMIC_H

#include "product.h"

class chemical:
    public product
{
    protected:

        // ! Enum for types of chemicals: herbicide, fungicide, pesticicde
        int type;

        // ! how much of the chemical that is reacting
        double activeSubstancePerUnit;
        double amountPerTFI;

        void InitVariables();

        int GetChemType(string aChemType);

    public:
        chemical();

        chemical(string       aName,
                 const int    aIndex = -1,
                 const base * aOwner = nullptr);

        chemical(const chemical & achemical);

        virtual ~chemical();

        friend ostream & operator <<(ostream &        os,
                                     const chemical & achemical);

        void Settype(const int atype) {
            type = atype;
        }

        void SetactiveSubstancePerUnit(const double aactiveSubstancePerUnit) {
            activeSubstancePerUnit = aactiveSubstancePerUnit;
        }

        void SetamountPerTFI(const double aamountPerTFI) {
            amountPerTFI = aamountPerTFI;
        }

        // Get functions
        int Gettype(void) const {
            return type;
        }

        double GetactiveSubstancePerUnit(void) const {
            return activeSubstancePerUnit;
        }

        double GetAmountPerTFI(void) const {
            return amountPerTFI;
        }

        virtual product & operator = (const product & someChemical);

        virtual product & operator +(const product & someChemical);

        virtual product & operator -(const product & someChemical);

        void showstats(ostream * os);

        virtual product * clone() const;

        double GetByTFI(void);

        void SetByTFI(double aTFI);

        virtual void ReadParameters(commonData * data,
                                    string       sectionName);
};
#endif


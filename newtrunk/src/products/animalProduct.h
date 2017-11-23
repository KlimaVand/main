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

#ifndef __Panimal_H
#define __Panimal_H

#include "liveMatter.h"

class animalProduct:
    public liveMatter
{
    private:

        // ! The minimum weight that this type of animal can have in tons
        double startWeight;

        // ! The maximum weight that this type of animal can have in tons
        double endWeight;
        double animalPrHa;    // pcs pr Ha

        void InitVariables();

    public:

        // ! Default Constructor
        animalProduct();

        animalProduct(string       aName,
                      const int    aIndex = -1,
                      const base * aOwner = nullptr);

        animalProduct(const animalProduct & aanimalProduct);

        virtual ~animalProduct();

        friend ostream & operator <<(ostream &             os,
                                     const animalProduct & aanimalProduct);

        // Get functions
        // ! Get the amount of organic N
        double GetN_content() {
            return orgN_content.n;
        }

        // ! the the start Weight
        double GetStartWeight() {
            return startWeight;
        }

        // ! the the end Weight
        double GetEndWeight() {
            return endWeight;
        }

        // ! Get a fraction of weight
        virtual double GetNumber() {
            return amount / endWeight;
        }

        double GetAnimalPrHa() {
            return animalPrHa;
        }

        void SetN_content(double aContent) {
            orgN_content = aContent;
        }

        void SetEndWeight(double anendWeight) {
            endWeight = anendWeight;
        }

        virtual product & operator = (const product & someanimalProduct);

        virtual product & operator +(const product & someanimalProduct);

        virtual product & operator -(const product & someanimalProduct);

        void showstats(ostream * os);

        virtual product * clone() const;

        virtual void ReadParameters(commonData * data,
                                    string       sectionName);
};
#endif


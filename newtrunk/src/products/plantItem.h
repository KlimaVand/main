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

#ifndef __PPLANTIT_H
#define __PPLANTIT_H

#include "decomposable.h"
#include "product.h"
#include "../base/base.h"

/*
 *
 * Class: plantItem
 *
 */
class plantItem:
    public decomposable
{
    protected:
        string tradeName;
        int    tradeable;    // can the plantitem be traded

        void InitVariables();

    public:
        plantItem();

        plantItem(string       aName,
                  const int    aIndex = -1,
                  const base * aOwner = nullptr);

        plantItem(const plantItem & aplantItem);

        virtual ~plantItem();

        string GetTradeName(void) const {
            return tradeName;
        }

        int GetTradeable(void) const {
            return tradeable;
        }

        virtual product & operator = (const product & somePlantItem);

        virtual product & operator +(const product & somePlantItem);

        virtual product & operator -(const product & somePlantItem);

        virtual product * clone() const;

        virtual void ReadParameters(commonData * data,
                                    string       sectionName);
};
#endif


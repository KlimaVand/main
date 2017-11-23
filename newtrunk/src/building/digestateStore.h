/*
 * NJH - 2.7.99
 */

#ifndef DIGESTATE_STORAGE_H
#define DIGESTATE_STORAGE_H

// #define STALDDEBUG

#include "slurrystore.h"
#include "../products/manure.h"
#include "../base/common.h"
#include "../base/budget.h"

/*
 *
 * Class: digestateStore
 * instanciable class used to simulate a slurry-storage.
 *
 *
 */
class digestateStore:
    public slurrystore
{
    private:

        // digestateStore() {};

    public:
        digestateStore(string       aName,
                       const int    aIndex,
                       const base * aOwner,
                       commonData * runData,
                       commonData * common);

        digestateStore(digestateStore & s);

        virtual ~digestateStore();

        // initialize from file
        // virtual double GetAreaPrAnimal() {return AreaPrAnimal;};   //surface area per animal - used if manure store is to be scaled to animal numbers present
        virtual void DailyUpdate();

        virtual void GiveIndicator(int indicatorType);

        void RcvManure(const manure * someManure);
};
#endif


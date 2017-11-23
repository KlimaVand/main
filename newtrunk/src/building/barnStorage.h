
#ifndef B_STORA2_H
#define B_STORA2_H

#include "../building/building.h"
#include "../building/storage.h"
#include "../base/budget.h"
#include "../products/feedItem.h"

/*
 *
 * Class: B_storage
 * superclass of all storage buildings. Should not be instanced.
 * Use one of the derived storages instead, or derive your own.
 *
 */

// !Superclass for all storage

/*
 * !Superclass for storage.  Should not be instanted.  Use one of the derived storage classes
 * instead or derive one of your own.
 */
class barnStorage:
    public building
{
    private:
        barnStorage() {
            stores = nullptr;
        }


    protected:

        // !List of stores
        linkList<storage> * stores;

        // budget Nbudget;
        storage defaultStorage;

    public:
        barnStorage(string        aName,
                    const int     aIndex,
                    const base *  aOwner,
                    commonData *& runData,
                    commonData *& common);

        barnStorage(const barnStorage & s);

        ~barnStorage();

        // !Return amount available in storage of a given product

        /*
         * !
         * param p contains details of the target product
         */
        virtual double GetAvailableAmountOf(const product * p);

        // !Request an amount of a product from storage. Creates an instance of product class and returns a pointer

        /*
         * !
         * param amount the amount requested
         * param p the product requested
         */
        virtual product * GetFromStore(double    amount,
                                       product * p);

        // !Send an amount of a product to a store. Creates an instance of product class and returns a pointer

        /*
         * !
         * param p the product to be sent
         */
        virtual int SendToStore(product * p);

        // ! Handles all daily actions of the storage
        virtual void DailyUpdate();

        // !no idea what this does
        virtual void GiveValueOfInventory();

        // !Check nutrient budgets
        virtual void EndBudget(bool show = false);

        // !Return in the amount of a feed product that is available

        /*
         * !
         * param aProduct the product of interest
         */
        virtual void GetAvailableFeedProduct(feedItem * aProduct);

        // !Returned a copy of a stored feed product

        /*
         * !
         * param aProduct the feed product to be copied
         */
        virtual product * GetCopyStoredFeedProduct(const product * aProduct);

        // !Generate indicator output

        /*
         * !
         * param indicatorType the type of indicating requested
         */
};
#endif


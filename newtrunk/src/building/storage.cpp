
#include "../base/common.h"
#include "../building/storage.h"

// Why does storage.cpp need these two when B_storage.cpp does not??

#include "../products/plantItem.h"
#include "../products/feedItem.h"
#include "../base/commonData.h"

storage::storage() {
    theProduct  = nullptr;
    theCapacity = 0.0;
}

storage::storage(string       aname,
                 const int    aIndex,
                 const base * aOwner,
                 double       aCapacity,
                 commonData * runData,
                 commonData * common):
    base(aname,
         aIndex,
         aOwner)
{
    theCapacity = aCapacity;

    string Alias;

    runData -> FindSection(Name, Index);

    runData -> setCritical(true);
    runData -> FindItem("Alias", &Alias);
    runData -> setCritical(false);
    common -> setCritical(true);

    int    indexInOld = -1;
    string place      = "nullptr";

    common -> findSection(Name, 0, "Alias", Alias, &place, &indexInOld);
    common -> FindSection(place, indexInOld);

    theProduct = nullptr;
}

storage::~storage() {
    if (theProduct) {
        delete theProduct;
    }
}

storage::storage(storage & aStore) {
    theCapacity = aStore.theCapacity;

    if (aStore.theProduct) {
        theProduct = aStore.theProduct -> clone();
    } else {
        theProduct = nullptr;
    }
}

double storage::Getamountstored() {
    return theProduct -> GetAmount();
}

/*
 * returns true product if:
 * a. the stored product name is the same as the desired product name
 * b. the stored product trade name is the same as the desired product name
 * c. the stored product trade name is the same as the desired product plant name
 */
bool storage::ProductCompare(const product * desiredProduct) {
    if (((theProduct -> GetObjType() == plantItemObj) || (theProduct -> GetObjType() == feedItemObj))
            && (desiredProduct -> GetObjType() == feedItemObj)) {
        if (((liveMatter *) theProduct) -> GetCode() == ((liveMatter *) desiredProduct) -> GetCode()) {
            return true;
        } else {
            return false;
        }
    } else {
        string desiredProductName     = desiredProduct -> GetName();
        string storedProductName      = theProduct -> GetName();
        string storedProductTradename = storedProductName;

        if ((theProduct -> GetObjType() == plantItemObj)
                && ((((plantItem *) theProduct) -> GetTradeName()) != "NONE") ) {
            storedProductTradename = ((plantItem *) theProduct) -> GetTradeName();
        }

        string desiredProductPlantname = "NONE";

        if (desiredProduct -> GetObjType() == feedItemObj) {
            desiredProductPlantname = ((feedItem *) desiredProduct) -> GetPlantName();
        }

        bool temp1 = false,
             temp2 = false,
             temp3 = false;

        if (storedProductName == desiredProductName)  {
            temp1 = true;
        }

        if (storedProductTradename == desiredProductName)  {
            temp2 = true;
        }

        if (storedProductTradename == desiredProductPlantname)  {
            temp3 = true;
        }

        return (temp1 || temp2 || temp3);
    }
}

/*
 * adding a product to what is already stored
 */
void storage::AddToStore(product * p) {
    *theProduct = *theProduct + *p;
}

/*
 * extract a product for store.
 */
product * storage::GetFromStore(double RequestedAmount) {
    double    inventoryAmount  = theProduct -> GetAmount();
    double    inventoryExtract = min(inventoryAmount, RequestedAmount);
    product * productFound     = nullptr;

    // if (inventoryAmount > 0.0) // if nothing in storage then do not cast
    {
        theProduct -> Setamount(inventoryExtract);

        productFound = theProduct -> clone();

        theProduct -> Setamount(inventoryAmount - inventoryExtract);
    }

    return productFound;
}

/*
 * returning the name of the product stored
 */
string storage::GiveNameStoredProduct() {
    return theProduct -> GetName();
}

ostream & operator <<(ostream &       os,
                      const storage & s) {
    os << setiosflags(ios::left) << setw(40) << "Capacity " << s.GetCapacity() << endl;

    // s.theProduct->showstats(&os);
    // inventory ?
    return os;
}

/*
 * Returns a copy of what is stored if is the same as input. If someone accidently has stored a plantItemObj in this store fasset wil halt
 */
product * storage::GetCopyStoredFeedProduct(const product * product1) {
    feedItem * aProductCopy = nullptr;

    if (ProductCompare(product1)) {
        if ((product1 -> GetObjType() == feedItemObj) && (theProduct -> GetObjType() == plantItemObj)) {
            aProductCopy = new feedItem(*(feedItem *) product1);

            theMessage -> FatalError("all plant products should be stored as feedItems, so this should not happen");

            // aProductCopy->CopyFromPlantItem(*(liveMatter*) theProduct);  //overwrite variables common to feedItem and plantItem
            aProductCopy -> Setamount(theProduct -> GetAmount());
        }

        if ((theProduct -> GetObjType() == feedItemObj) && (theProduct -> GetAmount() > 0.0)) {
            aProductCopy = new feedItem(*(feedItem *) theProduct);
        }

        return aProductCopy;
    } else {
        return nullptr;
    }
}

/*
 * Makes a copy of what is stored if it is a feedItemObj, feedType and there is something stored.
 * Else it returns nullptr
 */
product * storage::GetCopyStoredFeedProduct(int feedType) {
    if (theProduct -> GetObjType() == feedItemObj) {
        if ((((feedItem *) theProduct) -> GetfeedCategory() == feedType) && (theProduct -> GetAmount() > 0.0)) {
            feedItem * aProductCopy;

            aProductCopy = new feedItem(*((feedItem *) theProduct));

            aProductCopy -> Setamount(theProduct -> GetAmount());

            return aProductCopy;
        }
    }

    return nullptr;
}

/*
 * returning a pointer to what is stored if it is a feedItemObj, feedType and there is something stored.
 * Else it returns nullptr
 */
product * storage::GetStoredFeedProductPtr(int feedType) {
    if (theProduct -> GetObjType() == feedItemObj) {
        if ((((feedItem *) theProduct) -> GetfeedCategory() == feedType) && (theProduct -> GetAmount() > 0.0)) {
            return theProduct;
        }
    }

    return nullptr;
}

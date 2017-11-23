
#include "../base/common.h"
#include "../base/message.h"
#include "products.h"
#include "price.h"
#include "product.h"
#include "animalProduct.h"
#include "seed.h"
#include "fertilizer.h"
#include "chemical.h"
#include "water.h"
#include "energy.h"
#include "manure.h"
#include "milk.h"
#include "decomposable.h"
#include "feedItem.h"
#include "premium.h"
#include "feedResource.h"
#include "../base/settings.h"


#ifndef __ECOSYSTEM__

#include "../manager/manureManager.h"

#ifndef _STANDALONE

#include "../building/buildings.h"

#endif

#endif

#include "../base/IndicatorOutput.h"

products::products():
    base()
{
    theProductList             = new linkList<product>;
    specialManureList          = new linkList<manure>;
    thePriceList               = new linkList<price>;
    simulationYear             = 0;
    SellthePlantProducts       = false;
    SelltheStrawProducts       = false;
    SelltheNonRoughageProducts = false;
}

products::products(string       aName,
                   const int    aIndex,
                   const base * aOwner):
    base(aName,
         aIndex,
         aOwner)
{
    SelltheNonRoughageProducts = SellthePlantProducts = SelltheStrawProducts = false;
    theProductList             = new linkList<product>;
    specialManureList          = new linkList<manure>;
    thePriceList               = new linkList<price>;
    simulationYear             = 0;
}

products::products(const products & aproducts):
    base(aproducts)
{
    SelltheNonRoughageProducts = SellthePlantProducts = SelltheStrawProducts = false;
    simulationYear             = 0;
    theProductList             = new linkList<product>;
    specialManureList          = new linkList<manure>;
    thePriceList               = new linkList<price>;
}

products::~products() {

#ifndef __BCplusplus__
    if (theProductList) {
        delete theProductList;
    }
#endif

    if (thePriceList) {
        delete thePriceList;
    }
}

ostream & operator <<(ostream &        os,
                      const products & aproducts) {
    os << setiosflags(ios::left) << setw(40) << "theProductSubList " << endl;
    os << setiosflags(ios::left) << setw(40) << "theProductAddList " << endl;

    return os;
}

void products::InitializeProducts(string productFileName,
                                  string baseProductFileName) {
    fileAccess hd;

    hd.changeDir(globalSettings -> getInputDirectory());

    int          first, num, index;
    string       productSection;
    commonData * baseData = new commonData();

    baseData -> readFile(baseProductFileName);
    baseData -> setCritical(true);

    commonData * runData = new commonData();

    runData -> setCritical(false);
    runData -> readFile(productFileName);

    productSection = "product";

    baseData -> getSectionsNumbers(productSection, &first, &num);



    for (index = first; index < (first + num); index++) {
        product * productInstance = new product(productSection, index, this);

        productInstance -> ReadParameters(baseData, productSection);

        string name = productInstance -> GetName();

        productInstance -> ReadParameters(runData, productSection);

        if (name != productInstance -> GetName()) {
            string error = "product " + conv.IntToString(index) + " does not have the same names in the 2 files";

            theMessage -> FatalError(error);
        }

        AddElementProductList(productInstance);
    }

    productSection = "animal";

    baseData -> getSectionsNumbers(productSection, &first, &num);

    for (index = first; index < (first + num); index++) {
        animalProduct * animalInstance = new animalProduct(productSection, index, this);

        animalInstance -> ReadParameters(baseData, productSection);
        animalInstance -> ReadParameters(runData, productSection);

        string name = animalInstance -> GetName();

        if (name != animalInstance -> GetName())  {
            string error = "animal " + conv.IntToString(index) + " does not have the same names in the 2 files";

            theMessage -> FatalError(error);
        }

        AddElementProductList(animalInstance);
    }

    productSection = "decomposable";

    baseData -> getSectionsNumbers(productSection, &first, &num);

    for (index = first; index < (first + num); index++) {
        decomposable * decomposableInstance = new decomposable(productSection, index, this);

        decomposableInstance -> ReadParameters(baseData, productSection);

        string name = decomposableInstance -> GetName();

        decomposableInstance -> ReadParameters(runData, productSection);

        if (name != decomposableInstance -> GetName()) {
            string error = "decomposable " + conv.IntToString(index) + " does not have the same names in the 2 files";

            theMessage -> FatalError(error);
        }

        AddElementProductList(decomposableInstance);
    }

    productSection = "plantitem";

    baseData -> getSectionsNumbers(productSection, &first, &num);

    for (index = first; index < (first + num); index++) {
        plantItem * plantInstance = new plantItem(productSection, index, this);

        plantInstance -> ReadParameters(baseData, productSection);

        string name = plantInstance -> GetName();

        plantInstance -> ReadParameters(runData, productSection);

        if (name != plantInstance -> GetName()) {
            string error = "plantitem " + conv.IntToString(index) + " does not have the same names in the 2 files";

            theMessage -> FatalError(error);
        }

        AddElementProductList(plantInstance);
    }

    productSection = "seed";

    baseData -> getSectionsNumbers(productSection, &first, &num);

    for (index = first; index < (first + num); index++) {
        seed * seedInstance = new seed(productSection, index, this);

        seedInstance -> ReadParameters(baseData, productSection);

        string name = seedInstance -> GetName();

        seedInstance -> ReadParameters(runData, productSection);

        if (name != seedInstance -> GetName()) {
            string error = "seed " + conv.IntToString(index) + " does not have the same names in the 2 files";

            theMessage -> FatalError(error);
        }

        AddElementProductList(seedInstance);
    }

    productSection = "chemical";

    baseData -> getSectionsNumbers(productSection, &first, &num);

    for (index = first; index < (first + num); index++) {
        chemical * chemicalInstance = new chemical(productSection, index, this);

        chemicalInstance -> ReadParameters(baseData, productSection);

        string name = chemicalInstance -> GetName();

        chemicalInstance -> ReadParameters(runData, productSection);

        if (name != (chemicalInstance -> GetName())) {
            string error = "chemical " + conv.IntToString(index) + " does not have the same names in the 2 files";

            theMessage -> FatalError(error);
        }

        AddElementProductList(chemicalInstance);
    }

    productSection = "manure";

    baseData -> getSectionsNumbers(productSection, &first, &num);

    for (index = first; index < (first + num); index++) {
        manure * manureInstance = new manure(productSection, index, this);

        manureInstance -> ReadParameters(baseData, productSection);

        string name = manureInstance -> GetName();

        manureInstance -> ReadParameters(runData, productSection);

        if (name != (manureInstance -> GetName())) {
            string error = "manure " + conv.IntToString(index) + " does not have the same names in the 2 files";

            theMessage -> FatalError(error);
        }

        AddElementProductList(manureInstance);
    }

    productSection = "milk";

    baseData -> getSectionsNumbers(productSection, &first, &num);

    for (index = first; index < (first + num); index++) {
        milk * milkInstance = new milk(productSection, index, this);

        milkInstance -> ReadParameters(baseData, productSection);

        string name = milkInstance -> GetName();

        milkInstance -> ReadParameters(runData, productSection);

        if (name != (milkInstance -> GetName())) {
            string error = "milk " + conv.IntToString(index) + " does not have the same names in the 2 files";

            theMessage -> FatalError(error);
        }

        AddElementProductList(milkInstance);
    }

    productSection = "water";

    baseData -> getSectionsNumbers(productSection, &first, &num);

    for (index = first; index < (first + num); index++) {
        water * waterInstance = new water(productSection, index, this);

        waterInstance -> ReadParameters(baseData, productSection);

        string name = waterInstance -> GetName();

        waterInstance -> ReadParameters(runData, productSection);

        if (name != (waterInstance -> GetName())) {
            string error = "water " + conv.IntToString(index) + " does not have the same names in the 2 files";

            theMessage -> FatalError(error);
        }

        AddElementProductList(waterInstance);
    }

    productSection = "energy";

    baseData -> getSectionsNumbers(productSection, &first, &num);

    for (index = first; index < (first + num); index++) {
        energy * energyInstance = new energy(productSection, index, this);

        energyInstance -> ReadParameters(baseData, productSection);

        string name = energyInstance -> GetName();

        energyInstance -> ReadParameters(runData, productSection);

        if (name != (energyInstance -> GetName())) {
            string error = "energy " + conv.IntToString(index) + " does not have the same names in the 2 files";

            theMessage -> FatalError(error);
        }

        AddElementProductList(energyInstance);
    }

    productSection = "fertilizer";

    baseData -> getSectionsNumbers(productSection, &first, &num);

    for (index = first; index < (first + num); index++) {
        fertilizer * fertilizerInstance = new fertilizer(productSection, index, this);

        fertilizerInstance -> ReadParameters(baseData, productSection);

        string name = fertilizerInstance -> GetName();

        fertilizerInstance -> ReadParameters(runData, productSection);

        if (name != (fertilizerInstance -> GetName()) ) {
            string error = "fertilizer " + conv.IntToString(index) + " does not have the same names in the 2 files";

            theMessage -> FatalError(error);
        }

        AddElementProductList(fertilizerInstance);
    }

    productSection = "premium";

    baseData -> getSectionsNumbers(productSection, &first, &num);

    for (index = first; index < (first + num); index++) {
        premium * premiumInstance = new premium(productSection, index, this);

        premiumInstance -> ReadParameters(baseData, productSection);

        string name = premiumInstance -> GetName();

        premiumInstance -> ReadParameters(runData, productSection);

        if (name != (premiumInstance -> GetName()) ) {
            string error = "premium " + conv.IntToString(index) + " does not have the same names in the 2 files";

            theMessage -> FatalError(error);
        }

        AddElementProductList(premiumInstance);
    }

    delete baseData;
    delete runData;
}

double products::GetExpectedBuyPriceYear1(string ident) {
    price * aPrice;

    aPrice = GetPriceElement(ident);

    return aPrice -> GetExpectedBuyPriceYear(1);
}

/*
 *
 * Handles the registration of the sale of agricultural products
 *
 */
void products::AddProduct(product * aProduct) {

#ifndef _STANDALONE

    // string pname=aProduct->GetName();
    string nameStr = aProduct -> GetName();

    // char* indstr = "99.99 unsp sold";
    price * aPrice = aProduct -> GetaPrice();

    if ((!aPrice)
            && (aProduct -> GetObjType() == animalObj)) {    // some products have no price (e.g. home produced silage)
        theMessage -> FatalError("Products::AddProduct - Cannot find price for ", nameStr);
    }

    double CSent, NSent, income;

    switch (aProduct -> GetObjType()) {
        case feedItemObj :
        case plantItemObj :

#ifndef __ECOSYSTEM__
            theBuildings -> StoreProduct(aProduct);

            NSent = aProduct -> GetAmount() * ((liveMatter *) aProduct) -> GetAllN().n * 1000.0;

            theOutput -> AddIndicator(3504, "35.04 N from harvest to store", "kg N", NSent);

            CSent = aProduct -> GetAmount() * ((liveMatter *) aProduct) -> GetC_content() * 1000.0;

            theOutput -> AddIndicator(4504, "45.04 C from harvest to store", "kg C", CSent);

            if (aProduct -> GetaPrice()) {
                double income = ((aProduct -> GetAmount()) * ((aProduct -> GetaPrice()) -> GetactualSellPrice()));

                theOutput -> AddIndicator(1701, "17.01 Gross output of plant production", "Dkr", income);
            }
#else
            NSent = aProduct -> GetAmount() * ((liveMatter *) aProduct) -> GetAllN().n * 1000.0;

            theOutput -> AddIndicator(3520, "35.20 N in sold plant products", "kg N", NSent);
#endif

            break;

        /*
         *      case milkObj:
         * theOutput->AddIndicator("21.34 N in milk","kg N",Number * milkN/1000.0);
         */

#ifndef __ECOSYSTEM__
        case animalObj :
            income = ((aProduct -> GetAmount()) * (aPrice -> GetactualSellPrice()));

            if ((nameStr == "SUCKLINGPIG") || (nameStr == "PIGLETS") || (nameStr == "BACONERS") || (nameStr == "SOWS")
                    || (nameStr == "NEWSOWS")) {
                NSent = ((animalProduct *) aProduct) -> GetN_content() * aProduct -> GetAmount();

                theOutput -> AddIndicator(3420, "34.20 N in sold pigs", "kg N", NSent * 1000.0);

                CSent = ((animalProduct *) aProduct) -> GetC_content() * aProduct -> GetAmount();

                theOutput -> AddIndicator(4462, "44.62 C in sold pigs", "kg C", CSent * 1000.0);

                if (nameStr == "SUCKLINGPIG") {
                    theOutput -> AddIndicator(534, "05.34 Weaners sold", "Dkr", income);
                    theOutput -> AddIndicator(1905, "19.05 Sold weaners", "no",
                                              ((animalProduct *) aProduct) -> GetNumber());
                } else if (nameStr == "PIGLETS") {
                    theOutput -> AddIndicator(532, "05.32 Piglets sold", "Dkr", income);
                    theOutput -> AddIndicator(1906, "19.06 Sold piglets", "no",
                                              ((animalProduct *) aProduct) -> GetNumber());
                } else if (nameStr == "BACONERS") {
                    theOutput -> AddIndicator(531, "05.31 Baconers sold", "Dkr", income);
                    theOutput -> AddIndicator(1907, "19.07 Sold baconers", "no",
                                              ((animalProduct *) aProduct) -> GetNumber());
                } else if (nameStr == "NEWSOWS") {
                    theOutput -> AddIndicator(530, "05.30 Sows sold", "Dkr", income);
                    theOutput -> AddIndicator(1908, "19.08 Sold new sows", "no",
                                              ((animalProduct *) aProduct) -> GetNumber());
                } else if (nameStr == "SOWS") {
                    theOutput -> AddIndicator(533, "05.33 Cast sows sold", "Dkr", income);
                    theOutput -> AddIndicator(1909, "19.09 Sold cast sows", "no",
                                              ((animalProduct *) aProduct) -> GetNumber());
                }
            } else    // end of piggy matters
                    if ((nameStr == "HEIFERS1") || (nameStr == "HEIFERS2") || (nameStr == "HEIFERS3")
                        || (nameStr == "DAIRY") || (nameStr == "FBEEF1") || (nameStr == "FBEEF2")
                        || (nameStr == "FBEEF3") || (nameStr == "SUCKLER") || (nameStr == "MBEEF1")
                        || (nameStr == "MBEEF2") || (nameStr == "MBEEF3") || (nameStr == "CALF")) {
                NSent = ((animalProduct *) aProduct) -> GetN_content() * aProduct -> GetAmount();

                theOutput -> AddIndicator(3321, "33.21 N in sold cattle", "kg N", NSent);

                CSent = ((animalProduct *) aProduct) -> GetC_content() * aProduct -> GetAmount();

                theOutput -> AddIndicator(4321, "43.21 C in sold cattle", "kg C", CSent);

                if ((nameStr == "HEIFERS1") || (nameStr == "HEIFERS2")) {
                    theOutput -> AddIndicator(550, "05.50 Young 1st year heifers sold", "Dkr", income);
                    theOutput -> AddIndicator(1935, "19.35 Sold 1st year heifers", "no",
                                              ((animalProduct *) aProduct) -> GetNumber());
                } else if (nameStr == "HEIFERS3") {
                    theOutput -> AddIndicator(551, "05.51 Young 2nd year heifers sold", "Dkr", income);
                    theOutput -> AddIndicator(1936, "19.36 Sold 2nd year heifers", "no",
                                              ((animalProduct *) aProduct) -> GetNumber());
                } else if (nameStr == "DAIRY") {
                    theOutput -> AddIndicator(552, "05.52 Dairy cattle sold", "Dkr", income);
                    theOutput -> AddIndicator(1937, "19.37 Sold dairy cattle", "no",
                                              ((animalProduct *) aProduct) -> GetNumber());
                } else if (nameStr == "CALVES") {
                    theOutput -> AddIndicator(553, "05.53 Beef cattle calves sold", "Dkr", income);
                    theOutput -> AddIndicator(1938, "19.38 Sold beef calves", "no",
                                              ((animalProduct *) aProduct) -> GetNumber());
                } else if ((nameStr == "FBEEF1") || (nameStr == "FBEEF2") || (nameStr == "FBEEF3")
                           || (nameStr == "MBEEF1") || (nameStr == "MBEEF2") || (nameStr == "MBEEF3")
                           || (nameStr == "CALF")) {
                    theOutput -> AddIndicator(554, "05.54 Mature beef cattle sold", "Dkr", income);
                    theOutput -> AddIndicator(1939, "19.39 Sold beef cattle", "no",
                                              ((animalProduct *) aProduct) -> GetNumber());
                }
            }    // end of cattle matters

            break;
#endif

    }            // end of switch
#endif

}

void products::AddHarvestGrainProducts(linkList<plantItem> * aList) {
    AddHarvestProducts(1, aList);
}

void products::AddHarvestStrawProducts(linkList<plantItem> * aList) {
    AddHarvestProducts(2, aList);
}

void products::AddForageProducts(linkList<plantItem> * aList) {
    AddHarvestProducts(2, aList);
}

/*
 * **************************************************************************'
 * Mix all the products to form a single new product
 * Assign a product code to the new product
 * Assign a feed code to the new product
 */
void products::AddHarvestProducts(int                   feedCat,
                                  linkList<plantItem> * HarvestList) {

    // Check om der er mere end nul, hvis der er g� videre, ellers udskriv en advarsel !!!
    int        productCode = 0;
    feedItem * itemToStore = nullptr;

    productCode = GetHarvestProductCode(HarvestList);    // load product info for this product

    if (productCode != 0)                                                     // single product with a code from the feed table was found
    {
        feedItem * tempItem = (feedItem *) GetProductElement(productCode);    // get pointer to product

        if (tempItem) {
            itemToStore = new feedItem(*(tempItem));                          // create new feed item

            // if a single harvested product is present, itemToStore is a copy of the harvested product.
            // If more than one harvested product is present, they are all added to itemToStore
            for (int i = 0; i < HarvestList -> NumOfNodes(); i++) {
                plantItem * HarvestItem = HarvestList -> ElementAtNumber(i);

                if (HarvestItem -> GetAmount() > 0) {    // add the plantItems to the single feedItem
                    itemToStore -> AddPlantItem(*((plantItem *) HarvestItem), feedCat);
                }
            }
        } else {
            productCode = 0;                                                  // no product with this code was found
        }
    }

    if (productCode
            == 0) {    // no single product was found in feed table, so use the product name(s) to look for a product or products in products list
        if (HarvestList -> NumOfNodes()
                > 1) {    // this function always produces a single product. If two or more products are present and no product code is found, throw an error
            theMessage -> FatalError(
                "products::AddHarvestProducts - no mixed product found for two or more products in harvest");
        } else {
            plantItem * HarvestItem = HarvestList -> ElementAtNumber(0);

            itemToStore = new feedItem();

            plantItem * aplantItem = ((plantItem *) GetProductPtr(HarvestItem -> GetName()));

            if (!aplantItem) {
                theMessage -> FatalError("products::Harvested product unknown");
            } else {
                if (HarvestItem -> GetAmount() > 0) {
                    *aplantItem + *((plantItem *) HarvestItem);
                    itemToStore -> AddPlantItem(*((plantItem *) HarvestItem), feedCat);
                }
            }
        }
    }

    if (itemToStore -> GetAmount() > 0) {
        itemToStore -> SetCode(productCode);
        itemToStore -> SetfeedCategory(feedCat);
        AddProduct(itemToStore);

        if ((SellthePlantProducts) || ((SelltheStrawProducts) && (itemToStore -> GetfeedCategory() == 4))
                || ((SelltheNonRoughageProducts) && (itemToStore -> GetfeedCategory() != 2))) {
            SellPlantItem(itemToStore);
        }
    }

    delete itemToStore;
}

/*
 *
 * Sells a single plant item
 *
 */
void products::SellPlantItem(product * itemToSell) {

#ifndef _STANDALONE
    plantItem * aPlantItem   = (plantItem *) itemToSell;
    double      income       = 0.0;
    string      pname        = itemToSell -> GetName();

#ifndef __ECOSYSTEM__
    double      amountStored = theBuildings -> AmountProductStored(aPlantItem);
#else
    double      amountStored = 0.0;
#endif

    if (amountStored > 0.0) {
        if (amountStored < aPlantItem -> GetAmount()) {
            theMessage -> Warning(
                "products::SellPlantItem: trying to sell more than there are stored. Lower the amount.");
            aPlantItem -> Setamount(amountStored);
        }

        SubtractProduct(aPlantItem);

        double kgTotalN = aPlantItem -> GetAmount() * aPlantItem -> GetTotalN().n * 1000.0;
        double kgTotalC = aPlantItem -> GetAmount() * aPlantItem -> GetC_content() * 1000.0;

        // theOutput->AddIndicator(1971,"19.71 Exported plant products","t",aPlantItem->GetAmount());
        theOutput -> AddIndicator(3520, "35.20 N in sold plant products", "kg", kgTotalN);
        theOutput -> AddIndicator(4520, "45.20 C in sold plant products", "kg", kgTotalC);

#ifdef NITROSCAPE

        // harvested crops that are sold are considered to be imported feed here
        theOutput -> AddIndicator(3501, "35.01 N from imported feed", "kg", kgTotalN);
        theOutput -> AddIndicator(3501, "45.01 C from imported feed", "kg", kgTotalC);
#endif

        if (aPlantItem -> GetaPrice()) {
            income = ((aPlantItem -> GetAmount()) * ((aPlantItem -> GetaPrice()) -> GetactualSellPrice()));

            theOutput -> AddIndicator(510, "05.10 Plant products sold", "Dkr", income);
        }

        if (pname == "WNWHT") {
            theOutput -> AddIndicator(511, "05.11 W.wheat sold", "Dkr", income);
            //theOutput -> AddIndicator(521, "05.21 Grass for seed sold", "Dkr", income); // ck ????
        } else if (pname == "SPWHT")  {
            theOutput -> AddIndicator(512, "05.12 S.wheat sold", "Dkr", income);
        } else if (pname == "WNRPE")  {
            theOutput -> AddIndicator(513, "05.13 W.rape sold", "Dkr", income);
        } else if (pname == "SPRAPE")  {
            theOutput -> AddIndicator(51, "05.14 S.rape sold", "Dkr", income);
        } else if (pname == "WNBRL") {
            theOutput -> AddIndicator(515, "05.15 W.barley sold", "Dkr", income);
        } else if (pname == "BARLEY") {
            theOutput -> AddIndicator(516, "05.16 S.barley sold", "Dkr", income);
        } else if (pname == "RYE") {
            theOutput -> AddIndicator(517, "05.17 Rye sold", "Dkr", income);
        } else if (pname == "OAT") {
            theOutput -> AddIndicator(518, "05.18 Oat sold", "Dkr", income);
        } else if (pname == "PEA") {
            theOutput -> AddIndicator(519, "05.19 Pea sold", "Dkr", income);
        } else if (pname == "POTATO") {
            theOutput -> AddIndicator(520, "05.20 Potato sold", "Dkr", income);
        } else if (pname == "MAIZE") {
            theOutput -> AddIndicator(522, "05.22 Maize sold", "Dkr", income);
        } else if (pname == "BEET") {
            theOutput -> AddIndicator(523, "05.23 Beet sold", "Dkr", income);
        } else if (pname.compare("STRAW") > 0) {
            theOutput -> AddIndicator(524, "05.24 Straw sold", "DKr", income);
        } else if (pname == "FEEDRYEGRASS" || pname == "FEEDGRASSCLOVER") {
            theOutput -> AddIndicator(525, "05.25 Roughage sold", "DKr", income);
        }
    }
#endif

}

void products::SubtractProduct(product * aProduct) {
    SubtractProduct(aProduct, true);
}

void products::SubtractProduct(product * aProduct,
                               bool      import) {

#ifndef _STANDALONE

#ifndef __ECOSYSTEM__
    product * storedProduct   = nullptr;    // taken from storage
    product * importedProduct = nullptr;    // imported to farm
    double    req             = aProduct -> GetAmount();
    double    fromStore       = 0.0,
              imported        = 0.0;

    if (req > 0.0) {
        storedProduct = theBuildings -> ExtractProduct(aProduct);
    }

    if (storedProduct) {
        fromStore = storedProduct -> GetAmount();

        double NRemoved = 0.0,
               CRemoved = 0.0;

        switch (aProduct -> GetObjType()) {
            case feedItemObj :
                NRemoved = ((liveMatter *) storedProduct) -> GetAllN().n * fromStore * 1000;

                theOutput -> AddIndicator(3582, "35.82 N removed from stored feed products", "kg N", NRemoved);

                CRemoved = ((liveMatter *) storedProduct) -> GetC_content() * fromStore * 1000;

                theOutput -> AddIndicator(4582, "45.82 C removed from stored feed products", "kg C", CRemoved);

                break;

            case plantItemObj :
                NRemoved = ((liveMatter *) storedProduct) -> GetAllN().n * fromStore * 1000;

                theOutput -> AddIndicator(3581, "35.81 N removed from stored plant products", "kg N", NRemoved);

                CRemoved = ((liveMatter *) storedProduct) -> GetC_content() * fromStore * 1000;

                theOutput -> AddIndicator(3581, "45.81 C removed from stored plant products", "kg C", CRemoved);

                break;

            case manureObj :
                theManure_manager -> AddManureExtracted(fromStore);

                NRemoved = ((liveMatter *) storedProduct) -> GetAllN().n * fromStore * 1000;

                theOutput -> AddIndicator(3521, "35.21 N removed from manure storage", "kg N", NRemoved);

                CRemoved = ((liveMatter *) storedProduct) -> GetC_content() * fromStore * 1000;

                theOutput -> AddIndicator(4521, "45.21 C removed from manure storage", "kg C", CRemoved);

                if (aProduct -> GetmainUnitName() == 3) {    // if manure requested in kg N not tonnes FW
                    fromStore = NRemoved;
                }

                break;
        }
    }

    aProduct -> Setamount(0);    // zero product before adding stored material (and imported, if requested)

    imported = req - fromStore;    // amounts in tonnes or kg, depending on mainUnitName

    // Note: if the material taken from store is insufficient to satisfy demand, the remainder is imported
    if ((import) && (imported > 0.0)) {
        importedProduct = aProduct -> clone();

        importedProduct -> Setamount(imported);
        ImportProduct(importedProduct);
        *aProduct + *importedProduct;
    }

    delete importedProduct;    // JB added this

    if ((aProduct -> GetObjType() == manureObj)
            && (aProduct -> GetmainUnitName() == 3)) {    // requested in kg but returned in tonnes FW
        aProduct -> SetmainUnitName(2);
    }

    if (fromStore > 0.0) {
        *aProduct + *storedProduct;

        delete storedProduct;    // JB added this
    }
#endif

#endif

}

double products::ImportProduct(product * aProduct) {
    double    imported        = aProduct -> GetAmount();
    product * importedProduct = aProduct -> clone();    // imported to farm
    string    nameStr         = aProduct -> GetName();
    double    cost            = 0.0;
    int       indicatValue    = 9999;
    string    indstr          = "99.99 unspecified object bought";

    if (imported > 0.0) {

#ifndef __ECOSYSTEM__
        if (aProduct -> GetObjType() == manureObj) {
            if (importedProduct -> GetmainUnitName() == 3) {

                // amount requested is expressed in kg N not t FW, NJH Feb 2009
                imported = importedProduct -> GetAmount() / (((manure *) importedProduct) -> GetAllN().n * 1000.0);

                importedProduct -> Setamount(imported);
            }

            imported = theManure_manager -> GetImportableManure(imported);
        }
#endif

        importedProduct -> Setamount(imported);

        double NImported = 0.0,
               CImported = 0.0;

        if (importedProduct -> GetaPrice()) {
            cost = ((importedProduct -> GetAmount()) * ((importedProduct -> GetaPrice()) -> GetactualBuyPrice()));
        }

        switch (importedProduct -> GetObjType()) {
            case productObj :
                if (importedProduct -> GetName() == "hiredHours") {
                    indstr       = "06.70 Costs hired hours";
                    indicatValue = 670;
                } else if (importedProduct -> GetName() == "VET") {
                    indstr       = "06.33 Cost of vet and production control";
                    indicatValue = 633;
                }

                break;

            case waterObj :
                indstr       = "";
                indicatValue = 0;

                break;    // assuming that water has no price!

            case seedObj :
                indstr       = "06.11 Costs seed";
                indicatValue = 611;

                break;

            case fertilizerObj :
                indstr       = "06.12 Costs mineral fertilizer";
                indicatValue = 612;

                break;

            case chemicalObj :
                indstr       = "06.13 Costs chemicals";
                indicatValue = 613;

                break;

            case energyObj :
                if (importedProduct -> GetName() == "DIESEL") {
                    indstr       = "06.14 Costs fuel";
                    indicatValue = 614;
                } else {
                    indstr       = "06.15 Costs electricity";
                    indicatValue = 615;
                }

                break;

            case plantItemObj :
                indstr       = "06.32 Costs feed and bedding";
                indicatValue = 632;
                NImported    = ((plantItem *) importedProduct) -> GetAllN().n * importedProduct -> GetAmount() * 1000;

                theOutput -> AddIndicator(3503, "35.03 N from imported bedding", "kg N", NImported);

                CImported = ((plantItem *) importedProduct) -> GetC_content() * importedProduct -> GetAmount() * 1000;

                theOutput -> AddIndicator(4503, "45.03 C from imported bedding", "kg C", CImported);

                // should be straw registered here theOutput->AddIndicator("10.20 Imported manure","t",importedProduct->GetAmount());
                break;

            case feedItemObj :
                indstr       = "06.32 Costs feed and bedding";
                indicatValue = 632;
                NImported    = ((feedItem *) importedProduct) -> GetAllN().n * importedProduct -> GetAmount() * 1000;

                theOutput -> AddIndicator(3501, "35.01 N from imported feed", "kg N", NImported);

                CImported = ((feedItem *) importedProduct) -> GetC_content() * importedProduct -> GetAmount() * 1000;

                theOutput -> AddIndicator(4501, "45.01 C from imported feed", "kg C", CImported);

                // this needs changing if we want to import roughage other than grass!
                if ((importedProduct -> GetName() == "FEEDRYEGRASS")
                        || (importedProduct -> GetName() == "FEEDGRASSCLOVER")) {
                    theOutput -> AddIndicator(1952, "19.52 Imported roughage", "t", importedProduct -> GetAmount());
                } else {
                    theOutput -> AddIndicator(1951, "19.51 Imported cereals", "t", importedProduct -> GetAmount());
                }

                break;

            case manureObj :
                indstr       = "06.16 Costs imported manure";
                indicatValue = 616;

                theOutput -> AddIndicator(1950, "19.50 Imported manure", "t", importedProduct -> GetAmount());

                NImported = ((manure *) importedProduct) -> GetAllN().n * importedProduct -> GetAmount() * 1000;

                theOutput -> AddIndicator(3502, "35.02 N from imported manure", "kg N", NImported);

                CImported = ((manure *) importedProduct) -> GetC_content() * importedProduct -> GetAmount() * 1000;

                theOutput -> AddIndicator(4502, "45.02 C from imported manure", "kg C", CImported);

                break;

            case animalObj :
                if ((nameStr == "SUCKLINGPIG") || (nameStr == "PIGLETS") || (nameStr == "BACONERS")
                        || (nameStr == "SOWS") || (nameStr == "NEWSOWS")) {
                    NImported = ((animalProduct *) importedProduct) -> GetN_content() * importedProduct -> GetAmount()
                                * 1000.0;

                    theOutput -> AddIndicator(3404, "34.04 N in bought pigs", "kg N", NImported);

                    CImported = ((animalProduct *) importedProduct) -> GetC_content() * importedProduct -> GetAmount()
                                * 1000.0;

                    theOutput -> AddIndicator(4404, "44.04 C in bought pigs", "kg C", CImported);

                    if (nameStr == "SUCKLINGPIG") {
                        indstr       = "06.28 Weaners bought";
                        indicatValue = 628;

                        theOutput -> AddIndicator(1902, "19.02 Bought weaners", "no",
                                                  ((animalProduct *) aProduct) -> GetNumber());
                    } else if (nameStr == "PIGLETS") {
                        indstr       = "06.29 Piglets bought";
                        indicatValue = 629;

                        theOutput -> AddIndicator(1903, "19.03 Bought piglets", "no",
                                                  ((animalProduct *) aProduct) -> GetNumber());
                    } else if (nameStr == "BACONERS") {
                        indstr       = "06.30 Baconers bought";
                        indicatValue = 630;

                        theOutput -> AddIndicator(1904, "19.04 Bought baconers", "no",
                                                  ((animalProduct *) aProduct) -> GetNumber());
                    } else if ((nameStr == "SOWS") || (nameStr == "NEWSOWS")) {
                        indstr       = "06.31 Sows bought";
                        indicatValue = 631;

                        theOutput -> AddIndicator(1901, "19.01 Bought sows", "no",
                                                  ((animalProduct *) aProduct) -> GetNumber());
                    }
                } else if ((nameStr == "HEIFERS1") || (nameStr == "HEIFERS2") || (nameStr == "HEIFERS3")
                           || (nameStr == "DAIRY") || (nameStr == "FBEEF1") || (nameStr == "FBEEF2")
                           || (nameStr == "FBEEF3") || (nameStr == "SUCKLER") || (nameStr == "MBEEF1")
                           || (nameStr == "MBEEF2") || (nameStr == "MBEEF3")) {
                    NImported = ((animalProduct *) importedProduct) -> GetN_content() * importedProduct -> GetAmount();

                    theOutput -> AddIndicator(3303, "33.03 N in bought cattle", "kg N", NImported);

                    CImported = ((animalProduct *) importedProduct) -> GetC_content() * importedProduct -> GetAmount();

                    theOutput -> AddIndicator(4303, "43.03 C in bought cattle", "kg C", CImported);

                    if ((nameStr == "HEIFERS1") || (nameStr == "HEIFERS2")) {
                        indstr       = "06.50 Young 1st year heifers bought";
                        indicatValue = 650;

                        theOutput -> AddIndicator(1930, "19.30 Bought 1st year heifers", "no",
                                                  ((animalProduct *) aProduct) -> GetNumber());
                    } else if (nameStr == "HEIFERS3") {
                        indstr       = "06.51 Young 2nd year heifers bought";
                        indicatValue = 651;

                        theOutput -> AddIndicator(1931, "19.31 Bought 2nd year heifers", "no",
                                                  ((animalProduct *) aProduct) -> GetNumber());
                    } else if (nameStr == "DAIRY") {
                        indstr       = "06.52 Dairy cattle bought";
                        indicatValue = 652;

                        theOutput -> AddIndicator(1932, "19.32 Bought dairy cattle", "no",
                                                  ((animalProduct *) aProduct) -> GetNumber());
                    } else if ((nameStr == "FBEEF1") || (nameStr == "FBEEF2") || (nameStr == "FBEEF3")
                               || (nameStr == "MBEEF1") || (nameStr == "MBEEF2") || (nameStr == "MBEEF3")) {
                        indstr       = "06.54 Beef cattle calves bought";
                        indicatValue = 654;

                        theOutput -> AddIndicator(1933, "19.33 Bought beef calves", "no",
                                                  ((animalProduct *) aProduct) -> GetNumber());
                    } else if (nameStr == "SUCKLER") {
                        indstr       = "06.55 Mature beef cattle bought";
                        indicatValue = 655;

                        theOutput -> AddIndicator(1934, "19.34 Bought beef cattle", "no",
                                                  ((animalProduct *) aProduct) -> GetNumber());
                    }
                }

                break;

            default :
                if (importedProduct -> GetName() == "VET") {
                    indstr       = "06.33 Cost of vet and production control";
                    indicatValue = 633;
                }

                break;
        }
    }

    aProduct -> Setamount(0);

    if (imported > 0.0) {
        *aProduct + *importedProduct;
    }

    delete importedProduct;    // JB added this

    if (cost > 0.0) {
        theOutput -> AddIndicator(indicatValue, indstr, "Dkr", -cost);
    }

    return imported;
}

/*
 * Find a product with the same name as the input product and replace it with input.
 * It will hold nothing
 */
void products::GiveProductInformation(product * aProduct) {
    product * theProduct;

    if (aProduct -> GetObjType() == manureObj) {
        theProduct = GetProductElement(aProduct -> GetName(), true);
    } else {
        theProduct = GetProductElement(aProduct -> GetName(), false);
    }

    *aProduct = *theProduct;

    aProduct -> Setamount(0.0);
}

void products::AddElementProductList(product * aProduct) {
    ExistsProduct(aProduct);
    theProductList -> InsertLast(aProduct);
}

/*
 * find a product with a given name. If it cannot be found a nullptr price will be returned
 */
product * products::GetProductPtr(string aProductName,
                                  bool   isManure) {
    product * aProduct = nullptr;
    bool      found    = false;

    if (isManure) {
        linkList<manure>::PS aProductNode;
        specialManureList -> PeekHead(aProductNode);

        while ((aProductNode) &&!found) {
            aProduct = (product *) (aProductNode -> element);

            if (((aProduct -> GetName()) == aProductName)) {
                found = true;
            } else {
                aProductNode = aProductNode -> next;
            }
        }
    }

    linkList<product>::PS aProductNode;
    theProductList -> PeekHead(aProductNode);

    while ((aProductNode) &&!found) {
        aProduct = (product *) (aProductNode -> element);

        if (((aProduct -> GetName()) == aProductName)) {
            found = true;
        } else {
            aProductNode = aProductNode -> next;
        }
    }

    if (!found) {
        return nullptr;
    } else {
        return aProduct;
    }
}

/*
 * find a product with a given name. If the product does not exist fasset will halt
 */
product * products::GetProductElement(string aProductName,
        bool                                 isManure) {
    product * aProduct;

    aProduct = GetProductPtr(aProductName, isManure);

    if (!aProduct) {
        theMessage -> FatalError("products::GetProductElement - Cannot find product <", aProductName,
                                 ">");
    }

    return aProduct;
}

/*
 * finds a price with a given name. If it cannot be found a nullptr price will be returned
 */
price * products::GetPricePtr(string aPriceName) {
    price * aPrice = nullptr;
    bool    found  = false;

    linkList<price>::PS aPriceNode;
    thePriceList -> PeekHead(aPriceNode);

    while ((aPriceNode) &&!found) {
        aPrice = (price *) (aPriceNode -> element);

        string testString = aPrice -> Getname();

        if (testString == aPriceName) {
            found = true;
        } else {
            aPriceNode = aPriceNode -> next;
        }
    }

    if (!found) {
        return nullptr;
    } else {
        return aPrice;
    }
}
void products::InitializePrices(bsTime* astoptime)
{
   int  index;
   char priceSection[81];
   int SimulationInYearDuration = astoptime->GetYear() - theTime.GetYear();
	int aSimulationLength=SimulationInYearDuration+4;
	    fileAccess hd;

    hd.changeDir(globalSettings -> getInputDirectory());
  commonData * file=new commonData();
	file->readFile("prices.dat");
	strcpy(priceSection,"price");
   int first,num;
  file->getSectionsNumbers("price",&first,&num);
   for(index=first;index<(first+num);index++)
   {
      price* priceInstance = new price(priceSection,index,this);
      priceInstance->ReadParameters(file,aSimulationLength);
      AddElementPriceList(priceInstance);
   }
   delete file;
}
void products::AddElementPriceList( price* aPrice)
{
	ExistsPrice(aPrice);
	thePriceList->InsertLast(aPrice);
}
void products::ExistsPrice(price* somePrice)
{
	price* aPrice;
  	bool found=false;
  	linkList <price>::PS aPriceNode;
  	thePriceList->PeekHead(aPriceNode);
   while (aPriceNode!=NULL && !found)
   {
		aPrice=(price*) (aPriceNode->element);
		if (somePrice==aPrice)
		{
		found=true;
			theMessage->FatalError("products::ExistsPrice - Object <",(char*)(somePrice->Getname()).c_str(), "> exists allready as a reference");
      }
      if ( (aPrice->Getname())==(somePrice->Getname()))
      {
      	found=true;
			theMessage->FatalError("products::ExistsPrice - Object named <",(char*)(somePrice->Getname()).c_str(), "> exists allready");
      }
      aPriceNode=aPriceNode->next;
   }
}
void  products::UpdateProductPtr(void)
{
   linkList <product>::PS aProductNode;
   theProductList->PeekHead(aProductNode);

   while (aProductNode!=NULL)
   {
   	product* aProduct;
      aProduct=(product*)(aProductNode->element);
	   price* aPrice;

	 	aPrice=GetPricePtr(aProduct->Getname());
		if (aPrice!=NULL)
      	aProduct->SetaPrice(aPrice);
      aProductNode=aProductNode->next;  // BMP
   }
}

/*
 * return a price with a given name. If the prices does not exist fasset will halt
 */
price * products::GetPriceElement(string aPriceName) {
    price * aPrice;

    aPrice = GetPricePtr(aPriceName);

    if (!aPrice) {
        theMessage -> FatalError("products::GetPriceElement - Cannot find price <",  aPriceName, ">");
    }

    return aPrice;
}

/*
 * Checks if a products has already been added. It is compared by object, by name and feedcode.
 * If the product has been added fasset will halt
 *
 */
void products::ExistsProduct(product * someProduct) {
    product * aProduct;
    bool      found = false;

    linkList<product>::PS aProductNode;
    theProductList -> PeekHead(aProductNode);

    while ((aProductNode) &&!found) {
        aProduct = (product *) (aProductNode -> element);

        if (someProduct == aProduct) {
            theMessage -> FatalError("products::ExistsProduct - Product <", someProduct -> GetName(),
                                     "> exists Allready as a reference");
        }

        if (aProduct -> GetObjType() == feedItemObj) {
            if ((((feedItem *) aProduct) -> GetCode()) == (((feedItem *) someProduct) -> GetCode())) {
                char Indexstr[10];

                sprintf(Indexstr, "(%d)", ((feedItem *) someProduct) -> GetCode());
                theMessage -> FatalError("products::ExistsProduct - Product <", Indexstr,
                                         "> exists Allready with the same code");
            }
        } else if ((aProduct -> GetName()) == (someProduct -> GetName())) {
            theMessage -> FatalError("products::ExistsProduct - Product <", someProduct -> GetName(),
                                     "> exists Allready with the same name");
        }

        aProductNode = aProductNode -> next;
    }
}

/*
 * sets the amount in each products
 */
void products::resetProducts(void) {
    linkList<product>::PS aProductNode;
    theProductList -> PeekHead(aProductNode);

    while (aProductNode) {
        product * aProduct;

        aProduct = (product *) (aProductNode -> element);

        aProduct -> Setamount(0.0);

        aProductNode = aProductNode -> next;
    }
}

/*
 * returns a link list of products with the same type as ObjType
 */
linkList<product> * products::GetSubsetofProductList(int ObjType) {
    linkList<product> * aSubsetOfProdList;

    aSubsetOfProdList = new linkList<product>;

    linkList<product>::PS aProductNode;
    theProductList -> PeekHead(aProductNode);

    while (aProductNode) {
        product * aProduct;

        aProduct = (product *) (aProductNode -> element);

        if ((aProduct -> GetObjType() == ObjType)) {
            product * aCopyOfaProduct;

            aCopyOfaProduct = aProduct -> clone();

            aSubsetOfProdList -> InsertLast(aCopyOfaProduct);
        }

        aProductNode = aProductNode -> next;
    }

    return aSubsetOfProdList;
}

/*
 * Returns copy of a product if it is in a store, otherwise return nullptr
 */
product * products::GetCopyStoredFeedProduct(product * aProduct) {

#ifndef _STANDALONE

#ifndef __ECOSYSTEM__
    product * p;

    p = theBuildings -> GetCopyStoredFeedProduct(aProduct);

    return p;
#endif

#else
    return nullptr;
#endif

}

/*
 * Reads feed data from cattle feed tables (file must be output from Excel as tab-delimited text
 * Cannot use comma-delimited format as the first field often contains commas
 * NJH 21.06.00
 */
void products::InitializeFeed(string FeedFileName) {
    char       buffer[2000];
    fileAccess hd;

    hd.openFile(FeedFileName);

    fstream * file = hd.getFilePointer();

    file -> seekg(0);

	file -> getline(buffer, 2000, '\n');    // read file header and ignore it
    file -> getline(buffer, 2000, '\n');    // read file header and ignore it
    file -> getline(buffer, 2000, '\n');    // read file header and ignore it
    file -> getline(buffer, 2000, '\n');    // read file header and ignore it


    while (*file) {
        feedItem * temp_resource = new feedItem();

        if (temp_resource -> ReadParams(file)) {


            AddElementProductList(temp_resource);
        } else {
            delete temp_resource;
        }
    }

    file -> close();
}

/*
 * Returns pointer to cattlefeed, based on feed code
 */
product * products::GetProductElement(int feedCode,
        bool                              isManure) {
    product * aProduct = nullptr;
    bool      found    = false;

    linkList<product>::PS aProductNode;
    theProductList -> PeekHead(aProductNode);

    while ((aProductNode) &&!found) {
        aProduct = (product *) (aProductNode -> element);

        if (aProduct -> GetObjType() == feedItemObj) {
            if (((feedItem *) aProduct) -> GetCode() == feedCode) {
                found = true;
            }
        }

        aProductNode = aProductNode -> next;
    }

    if (!found) {
        return nullptr;
    } else {
        return aProduct;
    }
}

/*
 * Intepret standard feed code from crop or crop mixture
 */
int products::GetHarvestProductCode(linkList<plantItem> * HarvestList) {
    int  productCode    = 0;
    bool barley         = false,
         pea            = false,
         grass          = false,
         clover         = false,
         maize          = false,
         rape           = false,
         rye            = false,
         oat            = false,
         beet           = false,
         wheat          = false,
         potatoes       = false,
         beettop        = false,
         wheatstraw     = false,
         barleystraw    = false,
         rapestraw      = false,
         oatstraw       = false,
         ryestraw       = false,
         peastraw       = false,
         maizestraw     = false,
         potatotop      = false,
         barleypea      = false,
         soybean        = false,
         soybeanstraw   = false,
         sunflower      = false,
         sunflowerstraw = false,
         lucerne        = false;
    bool makeSilage     = false;
    bool makeHay        = false;

    // string s;
    for (int i = 0; i < HarvestList -> NumOfNodes(); i++) {
        plantItem * HarvestItem = HarvestList -> ElementAtNumber(i);

        if (!HarvestItem) {
            theMessage -> FatalError(
                "products::GetHarvestProductCode - about to call an object with a nullptr pointer");
        } else if (HarvestItem -> GetCode() == 1) {
            makeSilage = true;
        } else if (HarvestItem -> GetCode() == 2) {
            makeHay = true;
        }

        string cid = HarvestItem -> GetName();

        if ((cid == "WHEAT") || (cid == "WNWHT") || (cid == "SPWHT")) {    // SPWHT added MEL 2009
            wheat = true;
        } else if ((cid == "BARLEY") || (cid == "WNBRL")) {
            barley = true;
        } else if ((cid == "RAPE") || (cid == "WNRPE") || (cid == "SPRAPE")) {
            rape = true;
        } else if (cid == "OAT") {
            oat = true;
        } else if (cid == "RYE") {
            rye = true;
        } else if (cid == "PEA") {                                         // pea crop (not whole-crop)
            pea = true;
        } else if (cid == "MAIZE") {
            maize = true;
        } else if (cid == "BEET") {
            beet = true;
        } else if (cid == "POTATO") {
            potatoes = true;
        } else if (cid == "CLOVER") {
            clover = true;
        } else if ((cid == "GRASS") || (cid == "RYEGRASS") || (cid == "ITALRYEG")) {
            grass = true;
        } else if (cid == "CLOVERGRASS") {
            grass  = true;
            clover = true;
        } else if (cid == "BEETTOP") {
            beettop = true;
        } else if (cid == "WHEATSTRAW") {
            wheatstraw = true;
        } else if (cid == "BARLEYSTRAW") {
            barleystraw = true;
        } else if (cid == "RAPESTRAW") {
            rapestraw = true;
        } else if (cid == "OATSTRAW") {
            oatstraw = true;
        } else if (cid == "RYESTRAW") {
            ryestraw = true;
        } else if (cid == "PEASTRAW") {
            peastraw = true;
        } else if (cid == "MAIZESTRAW") {
            maizestraw = true;
        } else if (cid == "POTATOTOP") {
            potatotop = true;
        } else if (cid == "BARLEYPEA") {
            barleypea = true;
        } else if (cid == "SOYBEAN") {
            soybean = true;
        } else if (cid == "SOYBEANSTRAW") {
            soybeanstraw = true;
        } else if (cid == "SUNFLOWER") {
            sunflower = true;
        } else if (cid == "SUNFLOWERSTRAW") {
            sunflowerstraw = true;
        } else if (cid == "LUCERNE") {
            lucerne = true;
        }

        // s = cid; // For error checking
    }

    // NJH May 2009 all 'cereal && cerealstraw' changed to 'cereal || cerealstraw'. Forage harvesting creates only one product (usually the straw)
    // initially assume single crop
    if (wheat) {
        productCode = 203;
    } else if (wheatstraw) {
        if (makeSilage) {
            productCode = 590;
        } else if (HarvestList -> NumOfNodes() == 1) {
            productCode = 788;
        } else {
            theMessage -> FatalError("products::whole-crop wheat not in feed table");    // NJH May 2009, just in case
        }
    } else if (barley) {
        productCode = 201;
    } else if (barleystraw) {
        productCode = 781;
    } else if (barley && barleystraw) {
        if (makeSilage) {
            productCode = 583;
        } else {
            productCode = 483;
        }
    } else if (pea) {
        productCode = 216;
    } else if (peastraw) {
        productCode = 799;
    } else if (pea && peastraw) {
        if (makeSilage) {
            productCode = 478;
        } else {
            productCode = 499;
        }
    } else if (rape) {
        productCode = 213;
    } else if (rapestraw) {
        productCode = 781;    // should be something else but cannot find rape straw in feed table
    } else if (oat) {
        productCode = 202;
    } else if (beet) {
        productCode = 385;
    } else if (beettop) {
        productCode = 353;
    } else if (rye) {
        productCode = 207;
    } else if (grass) {
        if (makeSilage) {
            productCode = 565;
        } else if (makeHay) {
            productCode = 665;
        } else {
            productCode = 465;
        }
    } else if (lucerne) {
        if (makeSilage) {
            productCode = 501;
        } else if (makeHay) {
            productCode = 602;
        } else {
            productCode = 402;
        }
    } else if ((maize) && (!makeSilage)) {
        productCode = 204;
    } else if ((maizestraw) && (!makeSilage)) {
        productCode = 786;
    } else if ((maize && maizestraw) && (makeSilage))                                    // BMP changed from "&&" May 2007.  NJH changed it again May 2009
    {
        productCode = 593;
    } else if (potatoes) {
        productCode = 395;
    } else if (oatstraw) {
        productCode = 785;
    } else if (ryestraw) {
        productCode = 796;
    } else if (clover) {
        productCode = 473;
    } else if (grass && clover) {
        if (makeSilage) {
            productCode = 525;
        } else if (makeHay) {
            productCode = 625;
        } else {
            productCode = 423;
        }
    } else if ((barley && pea) || barleypea) {
        if (makeSilage) {
            productCode = 596;
        } else {
            productCode = 483;
        }
    } else if (potatotop) {
        productCode = 282;    // actually is beet top; do not have potato top in feed tables (but who is looking....?)
    } else if (soybean) {
        productCode = 214;
    } else if (soybeanstraw) {
        productCode = 155;
    } else if (sunflower) {
        productCode = 217;
    } else if (sunflowerstraw) {
        productCode = 165;
    }

    return productCode;
}

#ifdef NITROSCAPE

// !Recovers a pointer to a product of the given type and index from products.dat

/*
 * !
 * param aType The type of product (see typer.h)
 * param anIndex The index of the product required from products.dat
 */

// currently only used when using ecosystem alone (with ecosysteminterface class)
product * products::GetProductPtr(productObjType aType,
                                  int            anIndex) {
    product * aProduct;
    bool      found = false;

    linkList<product>::PS aProductNode;
    theProductList -> PeekHead(aProductNode);

    while ((aProductNode) &&!found) {
        aProduct = (product *) (aProductNode -> element);

        if ((aProduct -> GetObjType() == aType) && (aProduct -> GetIndex() == anIndex)) {
            found = true;
        } else {
            aProductNode = aProductNode -> next;
        }
    }

    if (!found) {
        return nullptr;
    } else {
        return aProduct;
    }
}
#endif


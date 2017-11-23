
#include "../../base/common.h"
#include "cropItalRyegrass.h"
#include "../../products/products.h"

/*
 * Temporary filler!! NJH May 2009
 */
cropItalRyegrass::cropItalRyegrass(string       aName,
                                   const int    aIndex,
                                   const base * aOwner,
                                   string       cropName):
								   cropRyegrass(aName,
                    aIndex,
                    aOwner,
                    cropName)
{

    // theMessage->FatalError("cropItalRyegrass:: this class has not been fully tested. Delete this line to test");
    PlantItemName = "ITALRYEG";    // These names maps to products.dat !
    StrawItemName = "ITALRYEG";    // These names maps to products.dat !
    WinterSeed    = 0;             // Is the present crop a wintercrop.
}

/*
 * Cut and return a plantItem     NJH March 2001
 * returns cut material in g/sq metre
 */
void cropItalRyegrass::Cut(plantItem * cutPlantMaterial,
                           double      cut_height) {
    if (GiveDryMatterVegTop() <= 0.0) {
        decomposable * Storage = new decomposable();
        decomposable * Straw   = new decomposable();

        Harvest(Storage, Straw);
        *cutPlantMaterial + *Storage;
        *cutPlantMaterial + *Straw;

        delete Storage;
        delete Straw;
    }
}

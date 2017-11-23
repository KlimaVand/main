/*
 *
 * Archive
 * Author     J�rgen Berntsen
 * Date     14/5-97
 * Revision
 * ----------------------------------------------------------------------------
 * Log
 *
 */

#ifndef LIVESTOC_H
#define LIVESTOC_H

#include "../livestock/animalStock.h"
#include "../fields/cropRotation.h"
#include "../building/buildings.h"
#include "dkCattle.h"
#include <vector>
/*
 *
 * Class: livestock
 * This class contains a linklist of animalstocks. The class should only be instantiated
 * once, and all common actions (daily DailyUpdate, gams comminucation, initializaton)
 * on the livestock is call through this class.
 *
 */
class livestock:
    public base
{

    /* Attributes */
    private:
        linkList<animalStock> * theStockList;    // Linked list of animals stocks
		
    public:

        /* Actions */
		vector<dk_cattle*>  theCattleonField;
    private:
        static void UpdateAnimals(animalStock * aAnimalStock) {
            aAnimalStock -> DailyUpdate();
        }

    public:
        livestock();

        ~livestock();

        void Initialize(string      filename,
                        bool        pigfarm,
                        bool        dairyfarm,
                        bool        beeffarm,
                        buildings * theBuildings);

        void ReceivePlan(string fileExtension);

        void DailyUpdate();

        double GetDE();

        linkList<animalStock> * getanimalStock() {
            return theStockList;
        }

        void GiveIndicator(int indicatorType);

        void FlushIndicator(indicatorTypes indicatorType);

        // ! tell housing about the manure production and heat generation of livestock
        void InformBuildings();

        void checkBalances(bool show);
};
extern livestock *    theLivestock;
#endif


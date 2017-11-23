/*
 *  Author    J�rgen Berntsen
 * Date    13/5-97
 * Revision    20/1-97 DailyUpdate added
 * Swinestk is necessary because the input to the model is number of pigs produced per
 * year and this must be adjusted to a daily number, based on the length of the production
 * cycle.
 *
 */

#ifndef SWINESTK_H
#define SWINESTK_H

#include "animalStock.h"
#include "swine.h"
#include "slaughterSwine.h"
#include "../building/PigHousing.h"
#include "../products/animalProduct.h"
#include "../building/buildings.h"

/*
 *
 * Class: swinestock
 * A swine stock generally has an instance of a swine and a slaughterSwine, representing
 * all swines and baconers in the swine stock. Most functions are called
 * from an instance of livestock.
 *
 */
class swineStock:
    public animalStock
{

    /* Attributes */
    protected:
        swine *          pregnantSwines;
        swine *          lactatingSwines;
        swine *          matingSwines;
        slaughterSwine * piglets;
        slaughterSwine * baconers;
        slaughterSwine * newSows;
        PigHousing *     aStable;
        animalProduct *  weanersInOutPrDay;
        animalProduct *  pigletsInOutPrDay;
        animalProduct *  pigletsPrDay;
        animalProduct *  baconersInOutPrDay;
        animalProduct *  baconersPrDay;
        animalProduct *  newsowsInOutPrDay;
        animalProduct *  newsowsPrDay;          // holds details of the daily amounts to be exported
        animalProduct *  castsowsPrDay;
        linkList<pig> *  pointyPigs;            // holds pointers to the pigs - used when operations done on all pigs
        double           SowProductionCycle;    // days between successive births

        /* Actions */

    private:
        swineStock & operator = (
                const swineStock & a);    // Dissable the compilers generation of default assignment operator.

        swineStock(const swineStock & a);    // Dissable the compilers generation of default copy constructor.

        // Constructor
        swineStock() {
            SowProductionCycle = 0.0;
            pregnantSwines     = nullptr;
            lactatingSwines    = nullptr;
            matingSwines       = nullptr;
            piglets            = nullptr;
            baconers           = nullptr;
            newSows            = nullptr;
            aStable            = nullptr;
            weanersInOutPrDay  = nullptr;
            pigletsInOutPrDay  = nullptr;
            pigletsPrDay       = nullptr;
            baconersInOutPrDay = nullptr;
            baconersPrDay      = nullptr;
            newsowsInOutPrDay  = nullptr;
            newsowsPrDay       = nullptr;
            castsowsPrDay      = nullptr;
            pointyPigs         = nullptr;
        }

        // write function for Gams
        void writeYearPlaces(string filename);

        void writeFeedComponents(string filename,
                                 int    produced);

        void writeFeedMin(string filename);

        void writeFeedMax(string filename);

        void writeSwineYield(string filename);

        void writeManureProduction(string filename);

        void writeManureNPK(string filename);

        void writeSwineFeed(string filename);

        void writeNotProducablePrice(string filename);

        void writeFeedPercentages(string filename,
                                  int    produced);

        void writePigLab(string filename);

    public:
        swineStock(string       aName,
                   const int    aIndex,
                   const base * aOwner);

        ~swineStock();

        void SetStable(PigHousing * aaStable) {
            aStable = aaStable;
        }

        // Get functions
        PigHousing * GetStable() {
            return aStable;
        }

        double GetNumberOfpregnantSwines() {
            return pregnantSwines -> GetNumberPrYear();
        }

        double GetNumberOflactatingSwines() {
            return lactatingSwines -> GetNumberPrYear();
        }

        double GetNumberOfmatingSwines() {
            return matingSwines -> GetNumberPrYear();
        }

        double GetNumberOfSwines() {
            return GetNumberOfpregnantSwines() + GetNumberOflactatingSwines() + GetNumberOfmatingSwines();
        }

        double GetNumberOfbaconers() {
            return baconers -> GetNumberPrYear();
        }

        double GetNumberOfPiglets() {
            return piglets -> GetNumberPrYear();
        }

        double GetNumber();

        double GetDE() {
            return pregnantSwines -> GetDE() + lactatingSwines -> GetDE() + matingSwines -> GetDE()
                   + piglets -> GetDE() + baconers -> GetDE();
        }

        void DailyUpdate();

        void ReceivePlan(string fileName);

        void ReceiveFeed(string fileName);

        void InitSwines(commonData *& file,
                        buildings *   theBuildings);

        void CheckNbalance();

        void ExportStock();

        virtual void EndBudget(bool show = false);

        double GetAnimalN() {
            return 0.0;
        }

        animalProduct * GetStockBought();

        animalProduct * GetStockSold();

        // ! send animal info on manure and heat production
        void InformBuildings();
};
#endif


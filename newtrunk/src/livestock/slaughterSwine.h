/*
 * * * Class "slaughterSwine"
 * Class for the slaughterSwine object. There is only one object for all slaughterSwines.
 * (c) J�rgen Berntsen, Danmarks JordbrugsForskning.
 * First version: 13/5-97
 * Revised: 16/5
 * NJH - Jan 2000 revised to cover piglets, finishers and replacement sows. Separate instance for each. Also reads data from file
 * (livestock.dat) rather than via hardcoding.
 * \
 */



#ifndef slaughterSwine_H
#define slaughterSwine_H
class slaughterSwine;

#include "pig.h"
#include "../products/animalProduct.h"


/*
 *
 * Class: slaughterSwine
 *
 */
class slaughterSwine:
    public pig
{

    /* Attributes */
    private:
        animalProduct * slaughterSwinePrDay;

        /* Actions */

    private:

    public:
        slaughterSwine();

        slaughterSwine(string       aName,
                       const int    aIndex,
                       const base * aOwner);

        ~slaughterSwine();

        void DailyUpdate();

        void ReceivePlan(commonData * data,
                         string       FileName);

        // double GetDE() {return NumberPrYear/30.0;};
        void SetNumberPrYear(double aNumberPrYear) {
            pig::SetNumberPrYear(aNumberPrYear);
        }

        virtual double GetTotalHeatProduction(double weight);

        virtual double CalcMaintenanceEnergy(double weight);

        double GetTotalME();
};
#endif


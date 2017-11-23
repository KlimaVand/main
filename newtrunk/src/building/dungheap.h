/*
 *
 * NJH - 2.7.99
 * ----------------------------------------------------------------------------
 * Log
 *
 */

#ifndef FYM_STORAGE_H
#define FYM_STORAGE_H

#include "manstore.h"
#include "building.h"
#include "../products/manure.h"
#include "../base/common.h"
#include "../base/budget.h"

/*
 *
 * Class: dungheap
 * instanciable class used to simulate a slurry-storage.
 *
 *
 */
class dungheap:
    public manurestore,
    public building
{
    private:
        bool          ventilated;    // true if air can access base of stack
        manurestore * surfaceLayer;

        // dungheap() {};

    public:
        dungheap(string        aName,
                 const int     aIndex,
                 const base *  aOwner,
                 commonData *& runData,
                 commonData *& common);

        dungheap(dungheap & s);

        virtual ~dungheap();

        void DailyUpdate();

        virtual void GiveIndicator(int indicatorType);

        /*
         * !
         * param volatAmmonia points to amount of Ammonia that have evapurated
         *  \param N2Emission points to amount of N2 that have evapurated
         *  \param N2OEmission points to amount of N2O that have evapurated
         *  \param CO2Emission points to amount of CO2 that have evapurated
         *   \param CH4Emission points to amount of CH4 that have evapurated
         */
        void VolatizeDung(double * volatAmmonia,
                          double * N2Emission,
                          double * N2OEmission,
                          double * CO2Emission,
                          double * CH4Emission);

        void RcvManure(const manure * someManure);

        friend ostream & operator <<(ostream & os,
                                     dungheap  s);

        void EndBudget(bool show);
};

/*
 *
 * End of Class manurestore
 *
 */
#endif


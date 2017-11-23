/*
 *
 *  Author   Lars Noboe Andersen & Poul Lassen
 * Date
 * Revision   Bjørn Molt Petersen, May-June 1997
 *
 */

#ifndef __TECHFARM_H
#define __TECHFARM_H

#include "techEquip.h"

/*
 *
 * Class: techFarm
 *
 */
class techFarm:
    public techEquip
{

    /* Attributes */
    protected:
        double percentNewValue;    // Factor used for calculating value primo
        double deprK;              // Constant used for calculation of depreciation percent
        double deprKHours;         // Constant used for calculation of depreciation percent
        double deprKLifetime;      // Constant used for calculation of depreciation percent
        int    reqWorker;
        int    age;
        double deprPercent;
        int    hoursPrimo;
        int    hoursUltimo;
        int    lifetimeHours;
        int    lifetimeYears;
        double maintenancePrm;
        double insurancePrm;
        double newValue;

        /* Actions */

    private:
        techFarm();    // Disable use of default constructor

    public:
        techFarm(string       aName,
                 const int    aIndex = -1,
                 const base * aOwner = nullptr);

        techFarm(const techFarm & atechFarm);

        virtual ~techFarm();

        virtual void ReadParameters(commonData *& file);

        void SethoursUltimo(int ahoursUltimo) {
            hoursUltimo = ahoursUltimo;
        }

        int GethoursPrimo(void) {
            return hoursPrimo;
        }

        int GethoursUltimo(void) {
            return hoursUltimo;
        }

        double GetmaintenancePrm(void) {
            return maintenancePrm;
        }

        double GetinsurancePrm(void) {
            return insurancePrm;
        }

        double GetdeprPercent(void) {
            return deprPercent;
        }

        double GetpercentNewValue(void) {
            return percentNewValue;
        }

        double GetnewValue(void) {
            return newValue;
        }

        void CalcDeprPercent();

        void UpdateAge();

        void UpdateHours(int aInvestStrategi);

        void UpdatePercentNewValue();

        techEconResp SendTechEconomic();
};
#endif


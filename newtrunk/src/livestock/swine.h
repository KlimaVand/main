/*
 * * * Class "Swine"
 * Class for the Swine object. There is only one object for all swines.
 * (c) J�rgen Berntsen, Danmarks JordbrugsForskning.
 * First version: 13/5-97
 * Revised: 11/6
 * Modified 4/11/99 by NJH
 * Describes a sow + suckling piglets
 * \
 */



#ifndef SWINE_H
#define SWINE_H

#include "pig.h"

/*
 *
 * Class: swine
 *
 */
class swine:
    public pig
{
    private:
        double pigletsPerLitter;
        double FEperPiglet;
        double maxParity;    // number of litters produced per sow

    private:

    public:
        swine();

        swine(string       aName,
              const int    aIndex,
              const base * aOwner);

        ~swine();

        void DailyUpdate();

        void ReceivePlan(commonData * data,
                         string       FileName);

        double GetpigletsPerLitter() {
            return pigletsPerLitter;
        }

        virtual double CalcMaintenanceEnergy(double weight);

        virtual void ReadParameters(commonData *& file);

        virtual double GetTotalHeatProduction(double weight);
};
#endif


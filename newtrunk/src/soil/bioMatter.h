// Soil biomatter pool
// (c) Bj�rn Molt Petersen, Danmarks JordbrugsForskning 1998-2000

#ifndef BIOMATT_H
#define BIOMATT_H

#include "matter.h"

class bioMatter:
    public matter
{
    protected:
        double   lackOfN, cnRatioDefault, minCNratio, maintenance, deathRate, utilizationEfficiency;
        cnMatter intermedCO2;

    public:
        bioMatter(string       Pname,
                  const int    Index,
                  const base * owner);

        bioMatter(const bioMatter & source);

        virtual bioMatter * clone() const;

        virtual void ReadParameters(commonData *& file);

        virtual void Update(double environmentEffect);

        virtual void CatchInflux(cnMatter & content);

        virtual void SetDeathRate(double d) {
            deathRate = d;
        }

        virtual void SetMaintenanceRate(double m) {
            maintenance = m;
        }

        virtual double GetDeathRate() {
            return deathRate;
        }

        virtual double GetMaintenanceRate() {
            return maintenance;
        }

        virtual double GetUtilizationEfficiency() {
            return utilizationEfficiency;
        }

        virtual void SetDeathAndMaintence(double val);

        virtual double GetCNcrit();

        virtual double GetDefaultCNratio() {
            return cnRatioDefault;
        }

        virtual double PotentialNitrogenMineralisation();

        virtual double ResidualCarbon();

        virtual void FinalExport(double ratio);

        virtual cnMatter GetCarbon();

        virtual double GetCarbonC();

        virtual nitrogen GetNitrogen();

        virtual double GetNitrogenN();

        virtual double GetNitrogenN15();

        virtual bool IsBiomass() const {
            return true;
        }

        virtual void SetCarbon(cnMatter c);

        virtual double GetTurnoverRatio() {
            return deathRate / (maintenance + deathRate);
        }

        virtual double PotentialCarbonTurnover() {
            return carbon.c * (maintenance + deathRate);
        }

        // virtual void TestCNratio();
        virtual bool ExtraTurnover(double Fraction);
};
#endif


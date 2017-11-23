// Organic matter in soil layer
// (c) Bj�rn Molt Petersen, DIAS
// Completely revised Maj 2002



#ifndef ORGNCMTT
#define ORGNCMTT

#include "../base/base.h"
#include "../products/cnmatter.h"
#include "../products/organicProduct.h"
#include "../products/nitrogen.h"
#include "../base/commonData.h"

class matter;


class organicMatter:
    public base
{
    private:
        organicMatter & operator = (const organicMatter & OM);

        // Dissable the compilers generation of default assignment operator.
        enum { maxPools = 7 };

        // Parameters
        double carbon_avalibility, ncSlope, ncIntercept, maxAOM2, exchangeRate, NH4allwaysLeft, beta, temperatureEffect,
               waterEffect, clayContent, clayParameter1, clayParameter2, clayParameter3, clayParameter4, clayParameter5,
               Rzs, Kirschbaum1, Kirschbaum2, KirschbaumTopt, Arrhenius, VantHoffQ10, refT, temperatureAdjust,
               clayContentLimit, W1, W2Offset, W3Offset, W4, Y1, Y2, m, clayEffect1, clayEffect2;
        cnMatter todayCO2;
        int      tResponseType, waterResponseType;
        double   clayRateLimit;
        bool     useNCfractionation;
        nitrogen NO3, NH4, NH4near;
        matter * poolList[maxPools];

        void updateClayEffects(double clayCont);

        double TemperatureEffect(double temp,
                                 int    responseType);

        double SoilWaterEffect(double water,
                               int    responseType);

        void AddMatter(string   type,
                       cnMatter amount);

        double FromAtomExecessToInternal(double excess);

        void SetFractionation(organicProduct * prod);

        double GetRatio(int poolName);

        double GetE(int poolName);

        double GetfSMB2();

    public:
        organicMatter(string       Pname,
                      const int    Index,
                      const base * owner);

        // Constructor
        organicMatter(const organicMatter & source);

        // source - Source object
        virtual ~organicMatter();

        void Initialize(double       clay,
                        double       carbon,
                        commonData * data,
                        int          index,
                        commonData * soilData);

        // Initializes the organic pool object
        void AddProduct(organicProduct * product);

        // Add an organic product to the organic matter
        // product - Organic product object
        double Update(double     temp,
                      double     pFValue,
                      nitrogen * nitrate,
                      nitrogen * ammonium);

        // Performs mineralisation of the organic matter pools in the soil layer.
        // temp          - Soil temperature [�C]
        // pFValue       - Soil water status as a pF-value
        // nitrate_mob   - Nitrate in the mobile water phase [g N/m�/d]
        // nitrate_imob  - Nitrate in the immobile water phase [g N/m�/d]
        // ammonium_mob  - Ammonium in the mobile water phase [g N/m�/d]
        // ammonium_imob - Ammonium in the immobile water phase [g N/m�/d]
        // Access functions
        double GetCarbon();

        // Return carbon in organic matter pools [g C/m�]
        nitrogen GetNitrogen();

        double GetNitrogenN();

        double GetNitrogenN15();

        // Return nitrogen in organic matter pools [g N/m�]
        double GetPoolCarbon(int name);

        // Return carbon in organic matter pools with a specific name [g C/m�]
        // name - Pool name
        nitrogen GetPoolNitrogen(int name);

        double GetPoolNitrogenN(int name);

        double GetPoolNitrogenN15(int name);

        // Return nitrogen in organic matter pools with a specific name [g N/m�]
        // name - Pool name
        // organicMatterPool * GetOMPool(string name);
        // Returns a pointer to a pool with a specific name
        // name - name of organic matter pool
        void InitStructure(commonData *& data);

        matter * GetPoolPointer(int poolName);

        void ExchangeNitrogen(nitrogen * nit);

        cnMatter GetCarbonInPool(int s);

        double GetCarbonInPoolC(int s);

        cnMatter GetAllSMBCarbon();

        nitrogen GetNitrogenInPool(int s);

        double GetNitrogenInPoolN(int s);

        double GetNitrogenInPoolN15(int s);

        double GetTotalCarbonC();

        double GetTotalNitrogenN();

        double GetTotalNitrogenN15();

        nitrogen GetTotalNitrogen();

        void CO2emmision(cnMatter co2);

        void SetPoolConnectionFraction(string * s,
                                       int      nr,
                                       double   fracC,
                                       double   fracN);

        void SetRzs(double r) {
            Rzs = r;
        }

        void SetExchangeRate(double e) {
            exchangeRate = e;
        }



        double GetCarbonAvailability() {
            return carbon_avalibility;
        }

        double GetTotalSystemN();

        void EquilibrateN15(cnMatter & c);

        void SetMineralNitrogen(nitrogen nitrate,
                                nitrogen ammonium);

        void GetMineralNitrogen(nitrogen & nitrate,
                                nitrogen & ammonium);

        void KillSMB(double Fraction);

        double GetPoolDecompositionRate(int name);
};
#endif


/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// =============================================================================
// Organic matter in soil layer
// (c) Bjørn Molt Petersen, DIAS
// Completely revised Maj 2002
// =============================================================================

#ifndef ORGNCMTT
   #define ORGNCMTT

#include "../base/base.h"
#include "../products/cnmatter.h"
#include "../products/organicProduct.h"
#include "../products/nitrogen.h"

class matter;
class organicMatter : public base
{
  private:
     organicMatter& operator=(const organicMatter& OM); // Dissable the compilers generation of default assignment operator.

  enum {maxPools=30,maxOrgProds=200,max14CYears=500};

 // Parameters
      double
         nCycling,
         carbon_avalibility,
         ncSlope,
         ncIntercept,
         maxAOM2,
         exchangeRate,
         NH4allwaysLeft,
         beta,
         nitrificationRate,
         stepFactor,
         temperatureEffect,
         waterEffect,
         clayContent,
         clayParameter1,
         clayParameter2,
         clayParameter3,
         clayParameter4,
         clayParameter5,
         deltaCarbon,
         deltaIsotope,
         Rzs,
         Kirschbaum1,
         Kirschbaum2,
         KirschbaumTopt,
         Arrhenius,
         VantHoffQ10,
         refT,
         temperatureAdjust,
         clayContentLimit,
         clayRateLimit,
         MaxDeltaCarbon,
         MaxDeltaIsotope,
         HalfTime,
         W1,
         W2Offset,
         W3Offset,
         W4,
         Y1,
         Y2,
         m,
         clayEffect1,
         clayEffect2,
         Nresidual;
      cnMatter todayCO2;
      int


         Isotope,

         tResponseType,
         waterResponseType,
         ClayResponseType,
         numberOfPools,
         numberOfProducts;

      bool
         useSpringob,
         useNCfractionation,
         exclusivePreferenceNH4,
         TAGpercent,
         ReadTAG,
         C14FromFile,
         useDelta14Cvalues;

      nitrogen
         NO3,
         NH4,
         NH4near;

      double radioCarbon[max14CYears];
      matter * poolList[maxPools];
      organicProduct * prodList[maxOrgProds];

      double ClayEffect(double clay,int responseType);
      double TemperatureEffect(double temp,int responseType);
      double SoilWaterEffect(double water,int responseType);


      double GetFractionFromdelta(double delta);
      double FromAtomExecessToInternal(double excess);

      void SetFractionation(organicProduct * prod);
      double GetRatio(string poolName);
      double GetE(string poolName);
      double GetfSMB2();

 public:
 organicMatter(const char * Pname,const int Index,const base * owner);
 // Constructor
 organicMatter(const organicMatter& source);
 // Copy constructor
 //  source - Source object
 virtual ~organicMatter();
 // Destructor

 void Add(organicMatter * source, double fraction);

 void Initialize(double clay,
                 double carbon,
                 fstream * f,
                 int index,
                 char * modelFileName);
 // Initializes the organic pool object

 void AddProduct(organicProduct * product);
 // Add an organic product to the organic matter
 //  product - Organic product object

 double Update(double temp,
					double pFValue,
					nitrogen * nitrate,
					nitrogen * ammonium);
 // Performs mineralisation of the organic matter pools in the soil layer.
 //  temp          - Soil temperature [°C]
 //  pFValue       - Soil water status as a pF-value
 //  nitrate_mob   - Nitrate in the mobile water phase [g N/m²/d]
 //  nitrate_imob  - Nitrate in the immobile water phase [g N/m²/d]
 //  ammonium_mob  - Ammonium in the mobile water phase [g N/m²/d]
 //  ammonium_imob - Ammonium in the immobile water phase [g N/m²/d]

 // Access functions
 double GetCarbon();
 // Return carbon in organic matter pools [g C/m²]
 nitrogen GetNitrogen();
 // Return nitrogen in organic matter pools [g N/m²]
 double GetPoolCarbon(const char * name);
 // Return carbon in organic matter pools with a specific name [g C/m²]
 //  name - Pool name
 nitrogen GetPoolNitrogen(const char * name);
 // Return nitrogen in organic matter pools with a specific name [g N/m²]
 //  name - Pool name
// organicMatterPool * GetOMPool(char * name);
 // Returns a pointer to a pool with a specific name
 //  name - name of organic matter pool
      void InitStructure(char * filename);

      matter * GetPoolPointer(string poolName);
      void ExchangeNitrogen(nitrogen * nit);
      cnMatter GetCarbonInPool(string * s);
      cnMatter GetCarbonInPool(char * c);

      cnMatter GetTotalCarbon();
      void CO2emmision(cnMatter co2);

      void SetCarbonInPool(string * s,cnMatter c);
      void SetCarbonInPool(char * ch,cnMatter c){string s=ch;SetCarbonInPool(&s,c);};

      double GetTurnoverRatioSMB1(double clayStandard);
      double GetCarbonAvailability() {return carbon_avalibility;};
      bool ExistsPool(string * poolName);
      cnMatter GetTotalSystem();
      void EquilibrateN15(cnMatter & c);
      void SetMineralNitrogen(nitrogen nitrate,nitrogen ammonium);
      void GetMineralNitrogen(nitrogen &nitrate,nitrogen &ammonium);

      void KillSMB(double Fraction);
      void ModifyAOM(double Fraction,double TurnoverFactor);
};

#endif


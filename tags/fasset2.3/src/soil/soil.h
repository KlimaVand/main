#ifndef SOIL
 #define SOIL

#include "../base/base.h"
#include "soilProfile.h"
#include "soilLayer.h"
#include "soilSurface.h"
#include "../products/organicProduct.h"
#include "../products/decomposable.h"
#include "../base/budget.h"


class soil : public base
{
 private:
 //!assignment operator
 soil& operator=(const soil& s); // Dissable the compilers generation of default assignment operator.
 soil();
 protected:
  //!file streams for details of soil variables.
   fstream f;
 //! object describing compounds on the soil surface (e.g. undissolved fertiliser)
 soilSurface       * Surface;      		// Soil surface
 //! object describing the soil profile
 soilProfile       * Profile;       	// Soil profile


 // Parameters
 double evaporationDepth;           	// Evaporation extraction depth [mm]
// state variables
 double evapotranspiration;         	// Actual evapotranspiration [mm/d]
 double totalTranspiration;         	// the total water uptake by plant [mm/d]
 double soilEvaporation;               // Soil evaporation [mm/d]
 double AccumulatedWaterDrainage;      // Accumulated water drainage (mm)
 double AccumulatedNitrateDrainage;    // Accumulated nitrate drainage [g N/m2]
 double CO2Evolution;                  // Daily CO2 evolution (g-C m-2)

 nitrogen nitrateLeached;             	// Nitrate leached from soil profile [g N/m�/d]
 nitrogen ammoniumLeached;            	// Ammonium leached from soil profile [g N/m�/d]
 nitrogen denitrification;            	// Nitrogen lost by denitrification [g N/m�/d]
 nitrogen nitrification;               // Daily nitrification [g N/m�/d]
 nitrogen NitrogenNetMineralisation;  	// net nitrogen mineralisation [g/m2]
 nitrogen* nitrogenUptake;             // Nitrogen uptake by plant roots





 //! soil nitrogen budget
 budget Nbudget,
 //!soil water budget
	WaterBudget;            // budgets

 public:
 //!constructor with arguments
 soil(const char * Pname,const int Index,const base * owner);
 //! destructor
 ~soil();
 //!copy constructor
 soil(const soil& Soil);

 //! initialise soil module
 virtual void   Initialize(char * soilFileName);
 //!add one soil object to another
 /*!
 \param addSoil pointer to the soil object to be added
 \param  fraction weighting factor to be used when doing the addition
 */
 virtual void Add(soil* addSoil, double fraction);
 //!adds nitrogen to the soil
 /*!adds nitrogen to the soil in the form of nitrate and ammonium, with partitioning between the soil surface
 and the top layer of the soil.
 \param  surfNitrate  Nitrate to be added to soil surface (grams of nitrogen per metre squared)
 \param surfAmmonium  Ammonium to be added to soil surface (grams of nitrogen per metre squared)
 \param soilNitrate Nitrate to be added to top of soil profile (grams of nitrogen per metre squared)
 \param soilAmmonium Ammonium to be added to top of soil profile (grams of nitrogen per metre squared)
 */
 virtual void   AddNutrient(nitrogen surfNitrate,
									 nitrogen surfAmmonium,
									 nitrogen soilNitrate,
									 nitrogen soilAmmonium);
 //! and chloride to the soil
 virtual void   AddChloride(double amount) {Surface->AddChloride(amount);};
 //!returns the temperature at a given depth in the profile
 virtual double GetTemperature(double depth) {return Profile->GetTemperature(depth);};
 //!daily update for the soil
 /*!
 \param    EPotToSoil      - potential evapotranspiration allocated to soil
 \param   surplus         - water added to soil [mm/d]
  \param  airTemperature  - air temperature [C]
  \param  snowContent     - snow content
  \param  roots           - root descriptor list
  */
 virtual void Update(double surplus,
   						double airTemperature,
	   				   double EPotToSoil,
                     double snowContent,
					 linkList<rootStructure>* roots);
 //!returns number of soil layers and
 int    GetLayers() {return Profile->GetNumberOfLayers();};

 //! returned to the thickness of a given layer ( millimetres)
 double GetLayerThickness(int No)
 {
  if (No<Profile->GetNumberOfLayers())
   return Profile->GetLayerThickness(No);
  else
   return 0.0;
 };
 //!adds decomposable organic matter to the soil
 void AddDecomposable(decomposable* produc,
                      double * rootLengthList = NULL);
 // Water functions

 double GetWater() {return Profile->GetTotalWater();};
 double GetWater(double startDepth,double thickness)         {return Profile->GetTotalWater(startDepth,thickness);};
 double GetSurfaceWater()                                    {return Surface->GetWater();};
 double GetAvailWater(double startDepth,double thickness)    {return Profile->GetAvailWater(startDepth,thickness);};

 double GetFieldCapacity(double startDepth,double thickness) {return Profile->GetFieldCapacity(startDepth,thickness);};
 double GetWiltCapacity(double startDepth,double thickness)  {return Profile->GetWiltCapacity(startDepth,thickness);};
 double GetEvapotranspiration() {return evapotranspiration;};
 double GetSoilEvaporation() {return soilEvaporation;};
 double GetDrainage(double depth) {return Profile->GetDrainage(depth);};

 // Nitrogen functions

 nitrogen* GetNitrogenUptake() {return nitrogenUptake;};

 nitrogen GetAmmonium();
 nitrogen GetAmmonium(double startDep, double thick)
							 {return Profile->GetAmmonium(startDep,thick);};
 nitrogen GetNitrate();
 nitrogen GetNitrate(double startDep, double thick) {return Profile->GetNitrate(startDep,thick);};
 nitrogen GetMinNitrogen();
 nitrogen GetMinNitrogen(double startDep, double thick);
 nitrogen GetDenitrification() {return denitrification;};
 nitrogen GetN2OFromNitrification() {return Profile->GetN2OFromNitrification();};
 nitrogen GetN2OFromDenitrification() {return Profile->GetN2OFromDenitrification();};
 nitrogen GetTotalNitrogen();
 nitrogen GetTotalNitrogen(double startDep, double thick);
 nitrogen GetTotalNitrogen(double depth);
 nitrogen GetPoolNitrogen(const char * Name, double startDep, double thick)
							 {return Profile->GetPoolNitrogen(Name,startDep,thick);};
 nitrogen GetNitrogenNetMineralisation();

 // Carbon functions
 double GetTotalCarbon() {return Surface->GetCarbon()+
											Profile->GetCarbon();};
 double GetTotalCarbon(double startDep, double thick)
 								{return Profile->GetOrganicCarbon(startDep,thick);};


 double GetPoolCarbon(const char * Name, double startDep, double thick)
							 {return Profile->GetPoolCarbon(Name,startDep,thick);};

 double GetCO2Evolution() {return CO2Evolution;};

 // Diverse get functions
 double GetSoilMass(double startDep, double thick) // unit: g dry soil per m2
 								{return Profile->GetSoilMass(startDep,thick);};
 soilSurface* GetSoilSurface(){return  Surface;};
 soilProfile* GetSoilProfile() {return Profile;};


 double GetMaximumDepth() {return Profile->GetMaximumDepth();};


 double GetMaxRootDepth() {return Profile->GetMaxRootDepth();};

 // N functions
 nitrogen GetAmmoniumLeaching(double depth) {return Profile->GetAmmoniumLeaching(depth);};
 nitrogen GetNitrateLeaching(double depth) {return Profile->GetNitrateLeaching(depth);};
 nitrogen GetRunOff_N() {return Surface->GetRunOff_N();};

 double GetRunOff() {return Surface->GetRunOff();};

 //!incorporate compounds that are on the soil surface into the soil
 /*!
 \param depth maximum depth to which the compounds should be incorporated
 \param  frac  fraction of the compounds on the soil surface that should be incorporated
 */
 void   Incorporate(double depth, double frac, bool tillage, const char * tillagetype);
 //!adds an organic product to the soil surface or to the soil profile
 void   AddOrganicProduct(const organicProduct* product,
								  double * rootLengthList = NULL);

 void WriteOutput();



 void KillSMB(double Fraction,double Depth);
 void ModifyAOM(double Fraction,double TurnoverFactor, double depth);

 // Budgets
 void StartBudget();
 bool EndBudget(double *N, double *w);


};

#endif


/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// =============================================================================
// soillayr.hpp
// Soil layer
// (c) J�rgen E. Olesen, Statens Planteavlsfors�g
// E-mail: jeo@pvf.sp.dk
// Changes:
// JEO 23-06-1997: GetPoolCarbon and GetPoolNitrogen added
// JEO 08-07-1997: GetAmmoniumLeaching and GetNitrateLeaching added
// JEO 27-10-1997: New soil evaporation formula
// =============================================================================

#ifndef SOILLAYR
#define SOILLAYR

#include "../base/base.h"
#include "pFCurve.h"
#include "organicMatter.h"
#include "../products/organicProduct.h"
#include "../products/nitrogen.h"
#include "../base/budget.h"
#include "../base/bstime.h"
#include "../base/bstime.h"
#include "../data/systemData.h"
class soilLayer : public base
{

   protected:
   organicMatter * OrganicMatter;
   // Dynamic N variables
   nitrogen nitrate_mob;         // Nitrate in mobile water [gN/m2]
   nitrogen nitrate_imob;        // Nitrate in immobile water [gN/m2]
   nitrogen ammonium_mob;        // Ammonium in mobile water [gN/m2]
   nitrogen ammonium_imob;       // Ammonium in immobile water [gN/m2]

   // Dynamic variables
   double waterContent;        // Volumetric water content. including ice [mm]
   double iceContent;          // Ice content, expressed as the equivalent volumetric water content [mm]
   //double frozenPart;          // 0-1
   double temperature;		    // Soil temperature [�C]
   double Chloride_imob;       // Chloride concentration [gCl/m2]
   double Chloride_mob;        // Chloride concentration [gCl/m2]

   // Parameters
	double TillageEffectTemperature;       // Effect of tillage on organic matter turnover
   double drainageConstant;    // Drainage constant [d-1]
   double holdbackConstant;    // Holdback constant []
   double dryBulkDensity;      // Dry bulk density [g/m3]
   double porosity;            // Soil porosity [mm/mm]
   double fieldCapacity;       // Volumetric water content at field capacity [mm]
   double startDepth;          // Depth of top of soil layer [mm]
   double thickness;           // Thickness of soil layer [mm]
   double wiltCapacity;        // Volumetric water content at wilting point [mm]
   double clayContent;         // Clay content in soil [g/g]
   pF_Curve * pF;              // Soil water retention curve
   double nitrificationRate;   // Nitrification rate coefficient [d-1]
   double tortuosityLimit;     // Tortuosity limit []
   double tortuosityFactor;    // Tortuosity factor []
   double waterFlux;           // unit: mm

   // Temporary variables - BMP: are any of these obsolete ???!!!
   bool equilibrated;
   double waterUptake;         // unit: mm
   double CO2Evolution;		  // unit: g C/m2/d
   double mobility;
   double mobility1;
   nitrogen ammoniumLeaching;     // Ammonium leaching from layer in previous step [g/m�]
   nitrogen nitrateLeaching;      // Nitrate leaching from layer in previous step [g/m�]
   nitrogen N2OFromNitrification;   // Current n2o from nitrification [g/m2]
   nitrogen N2OFromDenitrification; // Current n2o from denitrification [g/m2]
   nitrogen Denitrification;        // Current denitrification [g/m2]

   // Others
   soilLayer * next;
   budget Nbudget ;
   budget N15budget ;
   budget WaterBudget;

private:
   soilLayer();                          // Dissable use of default constructor

public:
	int orgNumber;
   double AmmoniumBuffering();
   double Tortuosity();
   double FractionMobileWater();
   double TemperatureNitrificationEffect();
   double TemperatureDenitrificationEffect();
   double WaterNitrificationEffect();
   double WaterDenitrificationEffect();
   double AccesibleAmmonium();
   double MaxNitrogenFlux(double rootRadius,
                        double uptakeRate,
                        double rootLength,
                        double conc,
                        double diffusionCoef,
                        double bufferingCoef);

//public:
   soilLayer(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
   soilLayer(const soilLayer& Layer);
   ~soilLayer();

   virtual void   Initialize(double startDep,
                           double thick,
                           fstream * f);
   virtual void   Add(soilLayer* Layer, double fraction);
   virtual void   AddNext(soilLayer * layer);
   virtual void   AddNutrient(nitrogen addNitrate,nitrogen addAmmonium);
   virtual void   AddChloride(double Amount);
   virtual double MaxNitrogenFlux(bool nitrate, rootStructure* root);
   virtual double MaxTranspiration(rootStructure* root);
   virtual double MaxTranspiration(double rootRadius,
                                 double rootpF,
                                 double rootLength);
   virtual void   SubtractTranspiration(double &transpiration);
   virtual nitrogen SubtractNitrateUptake(double &nitrateUptake);
   virtual nitrogen SubtractAmmoniumUptake(double &ammoniumUptake);
   virtual double NitrogenApparancy(bool nitrate, rootStructure* root);
   virtual void   PutTemperature(double temp){temperature=temp;}; // Temperature in the top of this layer
   virtual double HeatCapacityFreezing();
   virtual double HeatCapacity();
   virtual double SpecWaterCapacity();
   virtual double HeatConductivity();
   virtual double GetTemperature(){return temperature;};
   virtual void   SubtractEvaporation(double evaporation);
   virtual double UpdateOrganicMatter(int layer,int NumOfSoilLayers);
   virtual double PotentialCarbonTurnover();
   virtual nitrogen UpdateNitrification();
   virtual void UpdateN2OFromDenitrification(double TotalCO2Emission);
   virtual nitrogen UpdateN2Production();
   virtual double RatioN2OEmissionNitri();
   virtual double RatioN2OEmissionDenitri(double TotalCO2Emission);
   virtual nitrogen GetN2OFromNitrification() {return N2OFromNitrification;};   // Current n2o from nitrification [g/m2]
   virtual nitrogen GetN2OFromDenitrification() {return N2OFromDenitrification;}; // Current n2o from denitrification [g/m2]
   virtual nitrogen GetDenitrification() {return Denitrification;};

   virtual void   AddProduct(organicProduct& product);
   virtual void   ExchangeSubstance(double * waterLeached,nitrogen * nitrogenLeached, nitrogen * ammoniumLeached, double* chlorideLeached);
   virtual void   EquilibrateNitrogen(double moveWater);
   virtual void   AddWater(double surplus,nitrogen nitrateLeached,nitrogen ammoniumLeached, double ChlorideLeached);
   virtual void   AddWaterAndSolutes(double surplus,nitrogen nitrateAdd,nitrogen ammoniumAdd, double ChlorideAdd);
	virtual void	RemoveWaterAndSolutes(double waterInFlow,
                            nitrogen nitrateAdd,
                            nitrogen ammoniumAdd,
                            double ChlorideAdd);
   virtual void   RemoveWater(double moveWater,
                            nitrogen * nitrateLeached,
                            nitrogen * ammoniumLeached,
                            double *ChlorideLeached,
                            bool downwardMovement=true);
   virtual void   RemoveWaterHourly(double moveWater,
                                    nitrogen * nitrateLeached,
                                    nitrogen * ammoniumLeached,
                                    double *ChlorideLeached,
                                    bool downwardMovement=true);
   virtual void   RemoveWaterSimple(double moveWater,
                            nitrogen * nitrateLeached,
                            nitrogen * ammoniumLeached,
                            double *ChlorideLeached,
                            bool downwardMovement=true);

   double         GetFieldCapacity() {return fieldCapacity;};
   void           SetFieldCapacity(double aCapacity) {fieldCapacity = aCapacity;};
   double         GetWaterFlux() {return waterFlux;};
   void           SetWaterFlux(double aWaterflux) {waterFlux = aWaterflux;};
   void           AddWaterFlux(double aWaterflux) {waterFlux += aWaterflux;};
   double         GetFrozenPart();
   //void           SetFrozenPart(double aFrozenPart) {frozenPart = max(0.0,aFrozenPart);};
   void           SetIceContent(double aIceContent);
   pF_Curve*      GetpF_Curve() {return pF;};
   double         GetWaterUptake() {return waterUptake;};
   double         GetDrainageConstant() {return drainageConstant;};
   double         GetPorosity() {return porosity;};
   double         WaterApparancy(rootStructure* root);
   double 			AvailableWater(rootStructure* root);
   double 			GetTotalWater() {return waterContent;};
   double 			GetIce() {return iceContent;};
   void 			   SetWater(double aWaterContent) { waterContent = aWaterContent;};
   nitrogen	      GetTotalNitrogen();
   double         IntervalFraction(double startDep, double thick)
                     {return (max(min(startDepth+thickness,startDep+thick)-max(startDepth,startDep),0.0)/thickness);};
   double 	      GetTotalWater(double startDepth,double thickness) {return waterContent*IntervalFraction(startDepth,thickness);};
   double         GetAvailWater() {return max(0.0,waterContent-iceContent-wiltCapacity);};
   double         GetAvailWater(double startDepth,double thickness) {return (max(0.0,waterContent-iceContent-wiltCapacity))*IntervalFraction(startDepth,thickness);};
   double         GetAvailCapacity(double startDepth,double thickness) {return (fieldCapacity-wiltCapacity)*IntervalFraction(startDepth,thickness);};
   double         GetFieldCapacity(double startDepth,double thickness) {return fieldCapacity*IntervalFraction(startDepth,thickness);};
   double         GetWiltCapacity(double startDepth,double thickness) {return wiltCapacity*IntervalFraction(startDepth,thickness);};
   double         GetPressurePotential();
   double FracAmmoniumInWater();
   nitrogen       GetAmmonium(double startDepth,double thickness);
   nitrogen       GetNitrate(double startDepth,double thickness);
   nitrogen       GetAmmonium() {return ammonium_mob+ammonium_imob;};
   nitrogen       GetNitrate() {return nitrate_mob+nitrate_imob;};
   double ammoniumBefore;
   double nitrateBefore;
   void			  BeforeAmmonium() {ammoniumBefore= ammonium_mob.n+ammonium_imob.n;};
   void		      BeforeNitrate() {nitrateBefore= nitrate_mob.n+nitrate_imob.n;};
   void			  PrintAmmonium(int layer) {
	      static ofstream AmmoniumLayer;
 static bool ammoniumDisolvedUsed=false;
 string name=theSystemData->getOutputDirectory()+"AmmoniumLayer.xls";
	if(ammoniumDisolvedUsed==false)
	{

		AmmoniumLayer.open(name.c_str(),ios::out);
		AmmoniumLayer<<theTime<<"\t";
		ammoniumDisolvedUsed=true;
		
	}
	else
	{
		AmmoniumLayer.open(name.c_str(),ios::app);
		if(layer==0)
			AmmoniumLayer<<theTime<<"\t";
	}
	AmmoniumLayer<<(ammonium_mob.n+ammonium_imob.n)-ammoniumBefore<<"\t";
	if(layer==49)
		AmmoniumLayer<<endl;
	AmmoniumLayer.close();
   };
   void		      PrintNitrate(int layer) {
	      static ofstream NitrateLayer;
 static bool NitLayer=false;
 string name=theSystemData->getOutputDirectory()+"NitrateLayer.xls";
	if(NitLayer==false)
	{

		NitrateLayer.open(name.c_str(),ios::out);
		NitrateLayer<<theTime<<"\t";
		NitLayer=true;
		
	}
	else
	{
		NitrateLayer.open(name.c_str(),ios::app);
		if(layer==0)
			NitrateLayer<<theTime<<"\t";
	}
	NitrateLayer<<(nitrate_mob.n+nitrate_imob.n)-nitrateBefore<<"\t";
	if(layer==49)
		NitrateLayer<<endl;
	NitrateLayer.close();
   };
   nitrogen       GetMobileNitrate() {return nitrate_mob;};
   nitrogen       GetMobileAmmonium() {return ammonium_mob;};
   nitrogen       GetOrganicNitrogen() {return OrganicMatter->GetNitrogen();};
   nitrogen       GetOrganicNitrogen(double startDepth, double thickness);
   nitrogen       GetPoolNitrogen(const char * Name) {return OrganicMatter->GetPoolNitrogen(Name);};
   nitrogen       GetPoolNitrogen(const char * Name,double startDepth,double thickness);
   double         GetChloride(double startDepth,double thickness);
   double         GetMobileChloride() {return Chloride_mob;};
   double         GetCarbon() {return OrganicMatter->GetCarbon();};
   double         GetOrganicCarbon(double startDepth, double thickness)  {return GetCarbon()*IntervalFraction(startDepth,thickness);};
   double         GetPoolCarbon(const char * Name)
                {return OrganicMatter->GetPoolCarbon(Name);};
   double         GetPoolCarbon(const char * Name,double startDepth,double thickness)
                {return OrganicMatter->GetPoolCarbon(Name)*IntervalFraction(startDepth,thickness);};
   double         GetSoilMass(double startDepth, double thick)  {return 0.01*thickness*dryBulkDensity*IntervalFraction(startDepth,thick);};
   double         GetStartDepth() {return startDepth;};
   void           SetStartDepth(double aDepth) {startDepth = aDepth;};
   void           SetEquilibrated(bool eq){equilibrated=eq;};
   void 			   SetMobileNitrate(nitrogen aMobileNitrate) { nitrate_mob = aMobileNitrate;};
   void 			   SetMobileAmmonium(nitrogen aMobileAmmonium) { ammonium_mob = aMobileAmmonium;};
   void 			   SetMobileChloride(double aMobileChloride) { Chloride_mob = aMobileChloride;};
   void 			   SetDryBulkDensity(double aDryBulkDensity) { dryBulkDensity = aDryBulkDensity;};       //MEL2008
   bool           GetEquilibrated(){return equilibrated;};
   double         GetCenterDepth();
   double         GetThickness() {return thickness;};
   double         GetEndDepth() {return startDepth+thickness;};
   void           SetThickness(double aThickness) {thickness = aThickness;};
   nitrogen       GetAmmoniumLeaching() {return ammoniumLeaching;};
   nitrogen       GetNitrateLeaching() {return nitrateLeaching;};
   void           ClearLeaching() {ammoniumLeaching.Clear(); nitrateLeaching.Clear();};
   void           AddLeaching(nitrogen NO3,nitrogen NH4) {ammoniumLeaching=ammoniumLeaching+NH4; nitrateLeaching=nitrateLeaching+NO3;};
   double         GetClayContent(){return clayContent;};
   double         GetDryBulkDensity(){return dryBulkDensity;};
   double 			GetWaterFilledPorosity();

   void           SetTillageEffect(int type,double effect);
   void           KillSMB(double Fraction){OrganicMatter->KillSMB(Fraction);};
   void           ModifyAOM(double Fraction,double TurnoverFactor){OrganicMatter->ModifyAOM(Fraction,TurnoverFactor);};

   virtual	soilLayer * Next() {return next;};
   void StartBudget();
   void EndBudget();
   double GetPoolDecompositionRate(const char * Name);
};

#endif

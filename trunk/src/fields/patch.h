/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/**
 * Simulates a single patch in a field, and acts as container class for soil and
 * crop objects.
 * (c) Bjrn Molt Petersen, Danmarks JordbrugsForskning
 * E-mail: bpe@pvf.sp.dk
 */

#ifndef PATCH_H
   #define PATCH_H

#include "../products/plantItem.h"
#include "fieldOperation/fieldOperationFields.h"
#include "fieldOperation/sowFields.h"
#include "fieldOperation/harvestFields.h"
#include "../products/water.h"
#include "../products/feedItem.h"
#include "fieldOperation/soilTreatFields.h"
//NJH
#include "../products/manure.h"
#include "ALFAM.h"
#ifdef NITROSCAPE
	#include "../Nitroscape/patchinterface.h"
#else
	#include "ecosystem.h"
#endif

class patch: public base
{
protected:
	typedef char str[id_len];
   double area;
   int type;
#ifdef NITROSCAPE
	patchInterfaceClass * thePatchInterface;
#else
   ecosystem * theEcosystem;
#endif
//NJH
   //!manure on soil surface
   manure * soilManure;
   //!manure on the crop canopy
   manure * cropManure;
   //!true if patch can be grazed
   bool grazable,
   //!true if this is a dung patch
	  dung,
	  //!true if this is an overlap patch
	  overlap;
	  //!date when patch was created
   bsTime *birthday;
   //!used to communicate data to and from grazing animals
   feedItem * my_resource;

   //! default constructor
   patch();                          // Dissable use of default constructor
   //!default assignment operator
	patch& operator=(const patch& p); // Dissable the compilers generation of default assignment operator
   //!pointer to an object that contains the ALFAM ammonia emission model
	ALFAM *anALFAM;
	char * soilFileName;

public:
	//!construct with arguments
	patch(const char * aName, const int aIndex, const base * aOwner);
	//!copy constructor
	patch(const patch& p);
	//!destructor
   ~patch();
   //!creates a clone of the current patch
   patch* clone() const;
   //!   Add one patch to another
   /*!
   \param fraction  Weighting factor to be used when adding the donor patch to the recipient patch
   */
   void Add(patch* addPatch, double fraction);
   //!handles all major field operations
   void HandleOp(fieldOperationFields * FOP);
   //!return the area of the patch
   double GetArea(){return area;};
   //!returned percentage contribution of clover to aboveground crop dry matter
   double CloverPercentage();
   //!set the area of patch*/
   void SetArea(double ar){area=ar;};
   //!adjust the area of the patch
   /*!
   \param adjust the number to be used when adjusting the area
   */
   void AdjustArea(double adjust){area=area*adjust;};
   // Budgets
   //!initialised the budgets of those elements or compounds that are being budgeted
   void StartBudget();
   //!check the budgets of those elements or compounds that are being budgeted
   bool EndBudget();
   //!return the total amount of nitrogen in the patch
   double NitrogenInSystem();
   //!return the total amount of nitrogen in the crops
   double NitrogenInCrops();
   //! return the total amount of nitrogen in the soil
   double NitrogenInSoil();

   // ------ Grazing functions - NJH version - fill in later

   //! set whether patch is grazable
   void Setgrazable(bool isGrazable) {grazable = isGrazable;};
   //!get pointer to object describing manure on the soil surface
   manure * GetsoilManure() {return soilManure;};
   //!it pointed to object describing manure on the crop surfaces
   manure * GetcropManure() {return cropManure;};
   //!calculate the gaseous loss of compounds
   void Volatilise();
   //!returns true if the patch is classed as a dung patch
   bool getdung() const {return dung;};
   //!it turns true if the patch is classed as an overlap patch
   bool getoverlap() const {return overlap;};
   //!return the date on which the patch was formed


   //!returns an object describing the animal feed available within the patch
   feedItem * Getmy_resource(){return my_resource;};


   //!calculates then returns an object describing the animal feed available within the patch
   /*!
   \param  bite_depth_factor depth to which animals graze, as a proportion of the canopy height
   \param min_stubble_ht minimum height of plant canopy of the grazing - limits the depth of grazing
   \param animalType identifies the type of animal that is grazing
   */
	feedItem * GetFeedResource(double bite_depth_factor, double min_stubble_ht, int animalType);
	//!returns the age of the patch in days
	int GetPatchAge();
	//!returns the total amount of standing dry matter in the patch, in kilograms
	double GetPatchStandingDMkg();
	//! returns the total amount of standing dry matter in the patch, in kilograms per hectare
	double GetPatchStandingDMkgPerHa();


   //!return the patch type
   int GetPatchType(){return type;};

   //!returns the amount of nitrogen fixed within the patch during the day (in grams per square metre)
//   double GetNFixationThisDay(){return nFixThisDay;};
   //------------ The below functions are not truly implemented @@@!!!!!!!!!!!!!!!!!!!!!!!!!
   //!Get amount grazed from this patch during the current period (kg)
   double GetGrazedDM();
   //!returns the daily dry matter production in kilograms per hectare
   //!complete indicator values
	void GiveIndicators();
   double GetDailyDMProduction();
   //! returns the dry matter growth rate of the patch the absence of nutrient or water limitations
   /*!
   \param radiation radiation in megajoules per metre squared
   \param temperature mean daily air temperature
   */
	double GetPotentialGrowthRate(double radiation, double temperature);

   void Initialise(string localSoilFileName, int anIndex, double anArea);

   double StandingWater(){return 0.0;}
   void UpdatePatches();
   void DepositManure(manure * animal_urine, manure * animal_dung);


	/*!Sets the crop variables that record the material grazed
		Called by each animal that grazes this patch
   \param bite_depth_factor  depth of biting as a proportion of canopy height
   \param  DMGrazed kilograms of dry matter grazed by the animal
   */
	void SetGrazed(double bite_depth_factor, double DMGrazed, double NGrazed);

#ifndef NITROSCAPE


   //! returns true if the main crop is ready for harvesting
	bool ReadyForHarvestOrLater(){return theEcosystem->GetReadyForHarvestOrLater();};
   //!returns whether irrigation is required
	bool IrrigationDemand(){return theEcosystem->GetIrrigationDemand();};
	//!Get mass of dry matter (kg DM)of patch that is above a certain cutting or grazing height (m)
	   double GetAvailablePatchStandingDMkg(double residualDM);
	   void KillAllCrops(){theEcosystem->KillAllCrops();}
	double GetAbovegroundCropN(){ return theEcosystem->GetAbovegroundCropN();}
   //!returns the maximum height of the vegetation in the patch
	double GetMaxPatchHeight(){return theEcosystem->GetMaxCropHeight();};

	void SetirrigationWater(water *irrigationWater){theEcosystem->SetirrigationWater(irrigationWater);};
   void AddDecomposable(decomposable* produc) {theEcosystem->AddDecomposable(produc);};
	void CropSoilExchange() {theEcosystem->CropSoilExchange();};
   void GiveEcoIndicators() {theEcosystem->GiveIndicators();};

	void AddNutrient(nitrogen surfNitrate,nitrogen surfAmmonium, nitrogen soilNitrate, nitrogen soilAmmonium)
	   {theEcosystem->AddNutrient(surfNitrate, surfAmmonium, soilNitrate, soilAmmonium);};
   void Sow(char * cropID, double SeedDensDryMatt,double RelativeDensity,double NitrogenInSeed, double CarbonInSeed)
   	{theEcosystem->Sow(cropID, SeedDensDryMatt, RelativeDensity, NitrogenInSeed, CarbonInSeed);};
   void Tillage(soilTreatFields * TillageOp){theEcosystem->Tillage(TillageOp);};
   void HarvestOperations(harvestFields *hrv) {theEcosystem->HarvestOperations(hrv);};

   //!calculates the animal feed available within the patch
   /*!
   \param my_resource object containing the information on the amount and composition of material available
   \param grazing_height height above soil surface, above which material is available for grazing
   \param min_stubble_ht minimum height of plant canopy of the grazing - limits the depth of grazing
   \param animalType identifies the type of animal that is grazing
   */
	void GetFeedResource(feedItem * my_resource, double grazing_height, double min_stubble_ht, int animalType)
		{theEcosystem->GetFeedResource(my_resource, grazing_height, animalType);};
   double GetStandingDMdensity(){return theEcosystem->GetStandingDMdensity();};
   //	double GetMaxCropHeight(){return theEcosystem->GetMaxCropHeight();};
	void SendGrazed(double grazing_height, double DMGrazed, double NGrazed){theEcosystem->SetGrazed(grazing_height, DMGrazed);};

   void outputCropDetails(fstream *afile) {theEcosystem->outputCropDetails(afile);};
	void clearPatchRecords(){theEcosystem->clearGrazingRecords();};
	void   AddChloride(double amount) {theEcosystem->AddChloride(amount);};

#else
	char * getSoilFileName(){return soilFileName;};
	void zeroOutputVariables();
	patchInterfaceClass * GetthePatchInterface(){return thePatchInterface;}
//   double GetTotalLAI(){return thePatchInterface->GetTotalLAI();};
   //! returns true if the main crop is ready for harvesting
	bool ReadyForHarvestOrLater(){return thePatchInterface->GetReadyForHarvestOrLater();};
   //!returns whether irrigation is required
	bool IrrigationDemand(){return thePatchInterface->GetIrrigationDemand();};
	//!Get mass of dry matter (kg DM)of patch that is above a certain cutting or grazing height (m)
	   double GetAvailablePatchStandingDMkg(double height);
	   void KillAllCrops(){thePatchInterface->KillAllCrops();};
	double GetAbovegroundCropN(){ return thePatchInterface->GetAbovegroundCropN();}
   //!it turns the maximum height of the vegetation in the patch
  	double GetMaxPatchHeight(){return thePatchInterface->GetMaxPatchHeight();};
   void SetirrigationWater(water *irrigationWater){thePatchInterface->SetirrigationWater(irrigationWater->GetAmount(),(irrigationWater->GetAmount()*irrigationWater->GetN_content()));};
   void AddDecomposable(decomposable* produc) {thePatchInterface->AddDecomposable(produc);};


	void AddNutrient(nitrogen surfNitrate,nitrogen surfAmmonium, nitrogen soilNitrate, nitrogen soilAmmonium)
	   {thePatchInterface->AddNutrient(surfNitrate.n, surfAmmonium.n, soilNitrate.n, soilAmmonium.n);};
   void Sow(char * cropID, char * cropName, double SeedDensDryMatt,double RelativeDensity,double NitrogenInSeed, double CarbonInSeed)
   	{thePatchInterface->Sow(cropID, cropName, SeedDensDryMatt, RelativeDensity, NitrogenInSeed, CarbonInSeed);};
   void Tillage(soilTreatFields * TillageOp){thePatchInterface->Tillage(TillageOp);};
   void HarvestOperations(harvestFields *hrv) {thePatchInterface->HarvestOperations(hrv);};
   //!calculates the animal feed available within the patch
   /*!
   \param my_resource object containing the information on the amount and composition of material available
   \param grazing_height not used in NitroScape
   \param min_stubble_ht  not used in NitroScape
   \param animalType  not used in NitroScape
   */
	void GetFeedResource(feedItem * my_resource, double grazing_height, double min_stubble_ht, int animalType)
		{thePatchInterface->GetFeedResource(my_resource);};
   double GetStandingDMdensity(){return thePatchInterface->GetStandingDMdensity();};
   //	double GetMaxCropHeight(){return thePatchInterface->GetMaxCropHeight();};
	void SendGrazed(double grazing_height, double DMGrazed, double NGrazed){thePatchInterface->SetGrazed(grazing_height, DMGrazed, NGrazed);};

	void AddNH3Emission(double theNH3){thePatchInterface->SetNH3Emission(thePatchInterface->GetNH3Emission()+theNH3);};


   //nullptred functions
	void AddChloride(double amount) {};
	void clearPatchRecords(){};
	void CropSoilExchange(){};
	void outputCropDetails(fstream *afile, bool header=false){}
	void GiveEcoIndicators(){};
   int getEcoModel(){ return thePatchInterface->GetIndex();};
   void setEcoModel(int Index){ thePatchInterface->SetIndex(Index);};
   patchInterfaceClass * getPatchInterface(){ return thePatchInterface;};
#endif

};

#endif




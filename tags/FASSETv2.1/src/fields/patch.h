/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Class "patch"
Simulates a single patch in a field, and acts as container class for soil and
crop objects.
(c) Bjørn Molt Petersen, Danmarks JordbrugsForskning
E-mail: bpe@pvf.sp.dk
\****************************************************************************/

#ifndef PATCH_H
   #define PATCH_H

#include <pplantit.h>
#include <fiop.h>
#include <fiopso.h>
#include <pwater.h>
#include <pfeedit.h>
//NJH
#include <pmanure.h>
#include <ALFAM.h>
#ifdef NITROSCAPE
	#include <ecosystinterface.h>
#else
	#include <ecosyst.h>
#endif

class patch: public base
{
protected:
	typedef char str[id_len];
   double area;
   int type;
#ifdef NITROSCAPE
	ecosystinterfaceClass * theEcosystemInterface;
#else
   ecosystem * theEcosystem;
#endif
//NJH
   //!manure on soil surface
   manure * soilManure;       //manure on soil surface
   //!manure on the crop canopy
   manure * cropManure;       //manure on crop canopy
   //!true if patch can be grazed
   bool grazable,					// true if patch can be grazed
   //!true if this is a dung patch
	  dung,      					//true if this is a dung patch
	  //!true if this is an overlap patch
	  overlap;                //true if this is an overlap patch
	  //!date when patch was created
   bsTime *birthday;          //date when patch was created
   //!used to communicate data to and from grazing animals
   feedItem * my_resource;    //used to communicate data to and from grazing animals

   //! default constructor
   patch();                          // Dissable use of default constructor
   //!default assignment operator
	patch& operator=(const patch& p); // Dissable the compilers generation of default assignment operator
   //!pointer to an object that contains the ALFAM ammonia emission model
	ALFAM *anALFAM;


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
   //!complete indicator values
   void GiveIndicators();
   //!returns whether irrigation is required
   bool IrrigationDemand();
   //! returns true if the main crop is ready for harvesting
   bool ReadyForHarvestOrLater();
   //!handles all major field operations
   void HandleOp(fieldOperationFields * FOP);
   //!it turns the total amount of nitrogen in the aboveground crop
//   double GetAbovegroundCropN();
   //!return the area of the patch
   double GetArea(){return area;};
   //!return the total leaf area index of patch
/*   double GetTotalLAI(){return totalgLAI;};
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
   //void GetResource(double bite_dep,grazing_resource * gr){};
   //! returns true if the patch is grazable
   bool Getgrazable() {return grazable;};
   //! set whether patch is grazable
   void Setgrazable(bool isGrazable) {grazable = isGrazable;};
   //!get pointer to object describing manure on the soil surface
   manure * GetsoilManure() {return soilManure;};
   //!it pointed to object describing manure on the crop surfaces
   manure * GetcropManure() {return cropManure;};
   //!calculate the gaseous loss of compounds
   void Volatilise();
   //!it turns the maximum height of the vegetation in the patch
	double GetMaxPatchHeight();
   //!returns true if the patch is classed as a dung patch
   bool getdung() const {return dung;};
   //!it turns true if the patch is classed as an overlap patch
   bool getoverlap() const {return overlap;};
   //!return the date on which the patch was formed
   bsTime * Getbirthday(){return birthday;}
   //!set whether a patch is a dung patch
   void setdung (bool adung){ dung = adung;};
   //!set whether a patch is an overlap patch
   void setoverlap (bool aoverlap){ overlap = aoverlap;};
   //!returns an object describing the animal feed available within the patch
   feedItem * Getmy_resource(){return my_resource;};
   //!set the date of formation of the patch
   void SetPatchBirthday(bsTime aTime);
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
	//!Sets the crop variables that record the material grazed
	/*!Sets the crop variables that record the material grazed
		Called by each animal that grazes this patch
   \param bite_depth_factor  depth of biting as a proportion of canopy height
   \param  DMGrazed kilograms of dry matter grazed by the animal
   */
	void SetGrazed(double bite_depth_factor, double DMGrazed);
	//!set the name of the patch
   void SetName(char * aName) {strcpy(Name,aName);};
   //!set the patch type
   void SetPatchType(int t){type=t;};
   //!return the patch type
   int GetPatchType(){return type;};
   //!writes details of the nitrogen in the patch to the console.  Used only in debugging.
   void WriteNitrogen(double f);
   //!returns the amount of nitrogen fixed within the patch during the day (in grams per square metre)
//   double GetNFixationThisDay(){return nFixThisDay;};
   //------------ The below functions are not truly implemented @@@!!!!!!!!!!!!!!!!!!!!!!!!!
   //!Get mass of dry matter (kg DM)of patch that is above a certain cutting or grazing height (m)
   double GetAvailablePatchStandingDMkg(double height);
   //!Get amount grazed from this patch during the current period (kg)
   double GetGrazedDM();
   //!returns the daily dry matter production in kilograms per hectare
   //!return the total leaf area index of patch
   double GetTotalLAI(){return theEcosystem->GetTotalLAI();};  //NJH March 2009 Joined from NixFASSET
   double GetDailyDMProduction();
   //! returns the dry matter growth rate of the patch the absence of nutrient or water limitations
   /*!
   \param radiation radiation in megajoules per metre squared
   \param temperature mean daily air temperature
   */
	double GetPotentialGrowthRate(double radiation, double temperature);
   //!zeroes all patch variables related to grazing
	void clearPatchRecords();
   void Initialise(char * soilFileName, int anIndex, double anArea);
#ifndef NITROSCAPE
	void SetFingerFlow(int afingerFlow) {theEcosystem->SetFingerFlow(afingerFlow);}
#endif
   double StandingWater(){return 0.0;}
   void UpdatePatches();
   void DepositManure(manure * animal_urine, manure * animal_dung);
   double GetAbovegroundCropN();
   void Initialise(char * soilFileName,int aIndex);
#ifdef NITROSCAPE
   void AddDecomposable(decomposable* produc) {theEcosystemInterface->AddDecomposable(produc);};
#else
   void AddDecomposable(decomposable* produc) {theEcosystem->AddDecomposable(produc);};
#endif
#ifdef NITROSCAPE
   void Incorporate(double depth, double frac){theEcosystemInterface->Incorporate(depth, frac);};
#else
   void Incorporate(double depth, double frac){theEcosystem->Incorporate(depth, frac);};
#endif
//   double GetTotalAbovegroundN();
   void SimpleGraze();
	void outputCropDetails(fstream *afile, bool header);
};

#endif

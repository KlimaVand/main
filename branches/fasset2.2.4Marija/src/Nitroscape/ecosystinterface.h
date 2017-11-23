#ifndef __ECOINTERFACE_H
  #define __ECOINTERFACE_H

#include "../fields/ecosystem.h"
#include "../Nitroscape/harvInfoClass.h"

//!Interface class of an individual ecosystem
class ecosystinterfaceClass: public base
{
   private:
   	double totalLAI;
      bool ReadyForHarvestOrLater;
      double IrrigationDemand;
      double AddedChloride;
      double irrigationWater;
      double irrigationN;
      bool killAllCrops;  //needs to be reset daily
      double AbovegroundCropN;
      double MaxCropHeight;
      double StandingDMdensity;
		double AvailableStandingDM;
		double AvailableStandingDMHeight;
      double dryMatterGrazed;
      double grazedHt;

//! from N fertiliser addition
      double thesurfNitrate, thesurfAmmonium, thesoilNitrate, thesoilAmmonium;

//! tillage
      string theType;                                          // Tillage Type
      double theDepth;                                         // Tillage Depth
      double theFractionIncorporated;                          // Fraction of surface organic matter incorporated

//! harvesting
      linkList <harvInfoClass> *harvestStuff;
      int harvestCode;
      double propGrainHarvested;
      double propStrawHarvested;


      ecosystem *theEcosystem;
      // Copy Constructor
      ecosystinterfaceClass(const ecosystinterfaceClass& aecosystinterfaceClass) {};

   public:
      // Default Constructor
      ecosystinterfaceClass(int ecoNum, char * soilFileName, int aIndex = 0, double anArea = 0.0);
      // Destructor
       ~ecosystinterfaceClass(){};
	void DoDailyEco();
	void AddDecomposable(char *decompName, double decompAmount, double decompNH4_N, double decompNO3_N, double decompOrg_N, double decompC);
   void Incorporate();
   double GetTotalLAI(){return totalLAI;};
   void SetTotalLAI(double aVal){totalLAI = aVal;};
   bool GetReadyForHarvestOrLater(){return ReadyForHarvestOrLater;};
   void SetReadyForHarvestOrLater(){theEcosystem->GetReadyForHarvestOrLater();};
   void SetIrrigationDemand(double aVal){IrrigationDemand = aVal;};
   double GetIrrigationDemand(){return IrrigationDemand;};
	void KillAllCrops(){killAllCrops=true;};
	double GetMaxPatchHeight(){return MaxCropHeight;};
   double GetStandingDMdensity(){return StandingDMdensity;};
   double GetAvailableStandingDM(){return AvailableStandingDM;};
   void SetAvailableStandingDMHeight(double aVal){AvailableStandingDMHeight=aVal;}
   double GetMaxCropHeight() {return MaxCropHeight;};
	double GetAbovegroundCropN(){return AbovegroundCropN;}
	double GetpropGrainHarvested(){return propGrainHarvested;}
   void SetpropGrainHarvested(double aVal){propGrainHarvested=aVal;}
	double GetpropStrawHarvested(){return propStrawHarvested;}
   void SetpropStrawHarvested(double aVal){propStrawHarvested=aVal;}
	linkList <harvInfoClass> * GetharvestStuff(){return harvestStuff;}

   void HarvestOperations(int harvestCode, double propGrainHarvested, double propStrawHarvested);
	void AddNutrient(double surfNitrate,double surfAmmonium, double soilNitrate, double soilAmmonium)
   {theEcosystem->AddNutrient(surfNitrate,surfAmmonium, soilNitrate, soilAmmonium);};
	void Incorporate(double incorpDepth, bool tillage, const char * tillagetype){theEcosystem->Incorporate(incorpDepth,1.0, tillage,tillagetype);};

   void SetirrigationWater(double amountWater, double amountN);
   void Tillage(double theDepth, double theFractionIncorporated);
	void GetFeedResource(feedItem * my_resource, double grazing_height, double min_stubble_ht, int animalType);
	void SetGrazed(double grazing_height, double DMGrazed);

   plantItem*  createHarvestProduct(char *productName, double amountDM, double amountN, double amountC);
	void SetHarvestProduct(plantItem* aProduct, int code);
	void Sow(int seedIndex, double amountSeedDM, double NitrogenInSeed);
   void UpdateStatus();
	void AllocateCrop(char * seedName, char *cropCode);
	void AddOrganic(int theType, double theamount, double theDepth, double theTotalN, double theNH4N,
		double theNO3N, double thepercentDM, double theC);
   void SetFertilisation(double theNH4N, double theNO3N);
};


#endif

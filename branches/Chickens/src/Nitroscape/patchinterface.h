#ifndef __PATCHINTERFACE_H
  #define __PATCHINTERFACE_H

#include "../base/base.h"

#include "../products/feedItem.h"
#include "../products/nitrogen.h"
#include "../products/plantItem.h"
#include "../Nitroscape/harvInfoClass.h"
#include "../fields/fieldOperation/harvestFields.h"
#include "../fields/fieldOperation/soilTreatFields.h"
//! Stores the data describing a. the status of the ecosystem model liked to the patch and b. the details of the operations to be performed by that ecosystem
class patchInterfaceClass: public base
{
   private:

      // Copy Constructor
      patchInterfaceClass(const patchInterfaceClass& apatchInterfaceClass) {};

//!data from ECO to FARM
//!the total leaf area index - input from ecosystem
   	double totalLAI;
      //!is true if the crop can be harvested
      bool ReadyForHarvestOrLater;
      //! amount of water (mm) required to bring the soil moisture to Field capacity
      double IrrigationDemand;
      //!amount of water (mm) applied via irrigation
      double irrigationWater;
      //!amount of nitrogen applied in irrigation water (grams per square metre)
      double irrigationN;
      //! height of the crop(metres)
      double MaxCropHeight;
      //! total amount of dry matter grazed by animals (grams per square metre)
      double dryMatterGrazed;
      //!stores the amount of grain dry matter and nitrogen currently present in the crops (tonnes)
		plantItem * MainProduct[MaxPlants];
      //!stores the amount of Straw and leaf dry matter and nitrogen currently present in the crops  (tonnes)
      plantItem * StrawProduct[MaxPlants];
      //!not currently used
      plantItem * ForageProduct[MaxPlants];

//!Data from FARM to ECO
//! from sowing
		//! identifier code for the crop to be sown e.g. grass, clover, wheat.
      //!the identifier code is the index for the category called 'seed' in the file products.dat
		int thecropID;
      //!sowing density of the crop (grams of dry matter per square metre)
      double amountSeedDM;
      //!amount of nitrogen applied in the seed (grams per square metre)
      double seedN;

//! from N fertiliser addition
		//!the amount of nitrate nitrogen applied (kg per ha)
      double theNitrate;
		//!the amount of ammonium nitrogen applied (kg per ha)
      double theAmmonium;

//! tillage
		//!identifier code for the type of tillage operation to be undertaken
      //!the identifier code is the index for the operations in opnames.h
      int theType;                                          // Tillage Type
      //!the depth to which the tillage should be performed (metres)
      double theDepth;                                         // Tillage Depth
      //!the proportion of surface material that is buried in the tillage process
      double theFractionIncorporated;                          // Fraction of surface organic matter incorporated


//! harvesting
		//!identifier code for the type of harvesting operation to be undertaken
      //!the identifier code is the index for the operations in opnames.h
      int harvestCode;
      //!the proportion of the grain that will be harvested (the remainder returns to the soil as waste)
      double propGrainHarvested;
      //!the proportion of the straw that will be harvested (the remainder returns to the soil as waste)
      double propStrawHarvested;

//! adding organic matter
		//!identifier code for the type of organic matter to be applied
      //!if the identifier code is less than 100, the organic matter is in the form of manure
		//!if the identifier code is more than 100, the organic matter is in the form of plant material
		double decompType;
      //!the amount of material applied (grams FW per square metre)
      double decompAmount;
      //! amount of ammonium nitrogen applied (grams per square metre)
      double decompNH4_N;
      //!amount of nitrate nitrogen applied (grams per square metre)
      double decompNO3_N;
      //!amount of organic nitrogen applied (grams per square metre)
      double decompOrg_N;
      //!amount of organic carbon applied (grams per square metre)
      double decompC;
      //!the percentage dry matter in the manure
      double decompPercentDM;

      //!is true if the crops should be killed (e.g. by pesticides)
      bool killAllCrops;
//!   ammonia emission (kg per day) from fertiliser and manure applications
      double NH3Emission;

   public:
      // Default Constructor
      patchInterfaceClass(){};
      //!constructor with arguments
      /*!
      \param aName optional name
      \param aIndex contains the identifier number of the patch
      \param pointer to the patch owning interface instance
      */
		patchInterfaceClass(const char * aName, const int aIndex, const base * aOwner);
       ~patchInterfaceClass(){};
/*   double GetTotalLAI(){return totalLAI;};
   void SetTotalLAI(double aVal){totalLAI = aVal;};*/
   //! retrieve value
   bool GetReadyForHarvestOrLater(){return ReadyForHarvestOrLater;};
   //! set value
   void SetReadyForHarvestOrLater(bool aVal){ReadyForHarvestOrLater = aVal;};
   void SetIrrigationDemand(double aVal){IrrigationDemand = aVal;};
   void SetNH3Emission(double aVal){NH3Emission=aVal;}

   //! retrieve value
   double GetIrrigationDemand(){return IrrigationDemand;};
	void KillAllCrops(){killAllCrops=true;};
   //! retrieve value
   double GetStandingDMdensity(double area);
   //! retrieve value
   double GetAvailableStandingDM(double height);
   //! retrieve value
	double GetpropGrainHarvested(){return propGrainHarvested;}
   //! set value
   void SetpropGrainHarvested(double aVal){propGrainHarvested=aVal;}
   //! retrieve value
	double GetpropStrawHarvested(){return propStrawHarvested;}
   //! set value

   void SetpropStrawHarvested(double aVal){propStrawHarvested=aVal;}
   //!return dryMatterGrazed in tonnes
   double GetdryMatterGrazed(){return dryMatterGrazed;};
   //!Set dry Matter Grazed in tonnes
   void SetdryMatterGrazed(double aVal);
   //! retrieve value
   int GetthecropID(){return thecropID;};
   //! retrieve value
	double GetamountSeedDM(){return amountSeedDM;}
   //! set value
	void SetamountSeedDM(double aVal){amountSeedDM=aVal;}
   //! retrieve value
   double GetseedN(){return seedN;}

   //! retrieve value
   int GettheType(){ return theType;}
   //! set value
   void SettheType(int aVal){ theType = aVal;}
   //! retrieve value
   double GettheDepth(){return theDepth;}
   //! retrieve value
   double GettheFractionIncorporated(){return theFractionIncorporated;}

   //! retrieve value
	double GetMaxPatchHeight() {return MaxCropHeight;};

   //! retrieve value
   double GetdecompType(){return decompType;}
   //! retrieve value
   double GetdecompAmount(){return decompAmount;};
   //! retrieve value
   double GetdecompNH4_N(){return decompNH4_N;};
   //! retrieve value
   double GetdecompNO3_N(){return decompNO3_N;};
   //! retrieve value
   double GetdecompOrg_N(){return decompOrg_N;}
   //! retrieve value
   double GetdecompC(){return decompC;}
   double GetdecompPercentDM(){return decompPercentDM;}
   //! set value
   void SetdecompAmount(double aVal){decompAmount=aVal;};

   //! retrieve value
	double GettheNitrate(){return theNitrate;}
   //! retrieve value
   double GettheAmmonium(){return theAmmonium;}

   //! retrieve value
   int GetharvestCode(){return harvestCode;}
   //! set value
   void SetharvestCode(int aVal){harvestCode=aVal;}

	double GetNH3Emission(){return NH3Emission;}


   //!performs a harvest operation
   /*!
   \param hrv pointer to an object containing the details of the harvest operation
   */
   void HarvestOperations(harvestFields *hrv);
   //!adds manure or plant material to the soil
   /*!
   \param produc pointer to an object containing details of the organic matter added
   */
   void AddDecomposable(decomposable* produc);
   //!adds fertiliser nitrogen to the soil. Currently all nitrogen is added to the surface
   /*!
   \param surfNitrate nitrate nitrogen added to the surface (kg per ha)
   \param surfAmmonium ammonia nitrogen added to the surface (kg per ha)
   \param soilNitrate nitrate nitrogen added to the soil (kg per ha)
   \param soilAmmonium  ammonia nitrogen added to the soil (kg per ha)
   */
	void AddNutrient(double surfNitrate,double surfAmmonium, double soilNitrate, double soilAmmonium);
   //! sets the amount of water and nitrogen to be applied via irrigation
   /*!
   \param amountWater amount of water to be applied (millimetres)
   \param amountN amount of nitrogen to be applied
   */
   void SetirrigationWater(double amountWater, double amountN){irrigationWater = amountWater;irrigationN=amountN;};
   //!sows a crop
   /*!
   \param cropID crop identifier code
   \param cropName full name of the crop
   \param SeedDensDryMatt amount of seed applied(grams per meter squared)
   \param RelativeDensity the standard sowing density
   \param NitrogenInSeed the nitrogen applied in the seed (grams per metre squared)
   */
   void Sow(char * cropID, char * cropName, double SeedDensDryMatt,double RelativeDensity,double NitrogenInSeed);
   //!performs a tillage operation
   /*!
   \param TillageOp pointer to the class describing the tillage operation
   */
   void Tillage(soilTreatFields * TillageOp);
   //!incorporates organic matter on the soil surface into the soil
   /*!
   \param depth depths to which the organic matter should be incorporated (millimetres)
   \param frac fraction of the material on the soil surface that should be incorporated
   */
	void Incorporate(double depth, double frac, bool tillage, char * tillagetype);
   //!sets the height and amount of dry matter grazed
   /*!
   \param grazing_height heights to which the grazing occurred (millimetres)
   \param DMGrazed  amount of dry matter grazed (grams per metre squared)
   */
	void SetGrazed(double grazing_height, double DMGrazed);

   //!creates a harvest product
   /*!
   \param aName name to be given to the product
   \param amountDM amount of dry matter in the product (kilograms)
   \param amountN amount of nitrogen in the product(kilograms)
   \param amountC amount of carbon in the product (kilograms)
   */
   plantItem*  createHarvestProduct(char *aName, double amountDM, double amountN, double amountC);
   //! allocate a product to a particular type
   /*!
   \param aProduct pointer to the class describing the product
   \param code product identifying code (1 = grain,tuber etc, 2 = straw)
   */
	void SetHarvestProduct(plantItem* aProduct, int code);
   //!logs the status of an ecosystem
   /*!
   \param ReadyForHarvestOrLater is greater than zero if any crop present is ready for harvesting
   \pram  IrrigationDemand the amount of water necessary (in millimetres) to return the soil to field capacity
   \param theHarvestProducts linked list of pointers to the class containing information about the harvestable crop
   */
   void SetEcoStatus(int theReadyForHarvestOrLater,double theIrrigationDemand, linkList <harvInfoClass> *theHarvestProducts);
   //!returns the amount of above ground crop nitrogen (grams per metre squared). Not currently used.
	double GetAbovegroundCropN();
   //!allocates a crop name on the basis of the crop identifier
   /*!
   \param crop_id identifier of the crop
   \param cropName on exiting the routine, contains the name of the crop
   */
	void AllocateCrop(char * crop_id, char * cropName);
   //!zeros all output variables
	void zeroOutputVariables();
   //!calculates the animal feed available within the patch
   /*!
   \param my_resource object containing the information on the amount and composition of material available
   */
   void GetFeedResource(feedItem * aresource);


};

#endif


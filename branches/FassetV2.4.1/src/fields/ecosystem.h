/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

#ifndef ECOSYS_H
   #define ECOSYS_H

#include "../soil/soil.h"
#include "crops/crop.h"
#include "../products/plantItem.h"
#include "../products/water.h"
#include "../products/feedItem.h"
//NJH
#include "../products/manure.h"
#include "fieldOperation/animalFertFields.h"
#include "fieldOperation/harvestFields.h"
#include "fieldOperation/irrigateFields.h"
#include "fieldOperation/mineralFertFields.h"
#include "fieldOperation/manualLabourFields.h"
#include "fieldOperation/sowFields.h"
#include "fieldOperation/soilTreatFields.h"
#include "fieldOperation/plantCareFields.h"
#include "fieldOperation/makeGrazable.h"
#include "../tools/fileAccess.h"
#include "../base/cloneList.h"
static fileAccess scenarielog;
static fileAccess waterDetails;
static int scenarielogNumber=-1;
class ecosystem: public base
{


protected:

	typedef char str[id_len];
//!pointer to the patches  soil object
	soil * aSoil;              // Present soil object
	//!amount of irrigation water
   water irrigationWater;

   double
   //!amount of nitrogen fixed during the day
	nFixThisDay,
	//!total amount of dry matter grazed
	  totalGrazed_DM,
	  //!total amount nitrogen grazed
	  totalGrazed_N,
	  //!percentage of dry matter grazed (may be obsolete)
	  grazingEffiency ,
	  //! depth below which it is assumed that nitrate cannot be subsequently recovered by the crop (mm)
	  leachingDepth,
	  //!depth below which nitrogen to use the system  (mm)
	  balanceDepth,
	  //!area of ecosystem (ha)
	  area,                   // Present area of ecosystem, ha
     nLoad,
	  k1;

   // Dynamic variables
   //!water content in the interception layer (millimetres)
   double waterContent;       // Water content in interception layer [mm]
   //!Snow content in Snow Reservoir (millimetres)
   double snowContent;        // Water content in snow reservoir [mm]
   //!total green leaf area index
   double totalgLAI;

   // Parameters



   //!Base temperature for snowfall [�C]
   double tempSnowFall1;      // Base temperature for snowfall [�C]
   //! High temperature for snowfall [�C]
   double tempSnowFall2;      // High temperature for snowfall [�C]
   //!Melt rate for snow [mm/(�Cd)]
   double snowMeltRate1;      // Melt rate for snow [mm/(�Cd)]
   //! Melt rate for snow [mm/(MJ/m2)]
   double snowMeltRate2;      // Melt rate for snow [mm/(MJ/m2)]
   bool autoGraze;            // Temporary - remove !!!
   //! water budget
   budget WaterBudget;            // budgets
   //!list of pointers to the crop objects present
   cloneList<crop>* theCrops; // Present list of crop objects
   //!list of pointers to the crop product objects that have yet to be gathered
   linkList<plantItem>* cutPlantProducts; // List of straw items not yet gathered


   //! default constructor
   ecosystem();                          // Dissable use of default constructor
   //!default assignment operator
	ecosystem& operator=(const ecosystem& p); // Dissable the compilers generation of default assignment operator
  //!Perform addition of water and evaporation from interception layer
//!updates harvest area indicators
   void AddHarvestedAreaToIndicators(const char * cid, double a);
//!updates harvest yield indicators
   void AddYieldToIndicators(const char * cid, double y,double n);
   //!an expected yield to indicate the list
  // void AddExpectedYieldToIndicators(const char * cid, double y);
/*!
\param  surplus           - Water to be added to interception layer and resulting drainage from the layer [mm]
\param temperature       - Mean air temperature [�C]
\param globalRadiation   - Global solar radiation [MJ/m�]
\param ePotGreenLeaves   - Potential evapotranspiration from green leaves [mm]
\param ePotDeadLeaves    - Potential evapotranspiration from dead leaves [mm]
\param ePotSoil          - Potential evapotranspiration from soil [mm]
\param interCeptCapacity - Interception capacity [mm]
 */
   void UpdateInterception(double * surplus,
          						double   temperature,
			 	            	double   globalRadiation,
			 			         double * ePotGreenLeaves,
			 			         double   ePotDeadLeaves,
								 double * ePotSoil,
			 			         double   interceptCapacity,
						   double * greenEvap);
   //!adds  dead root matter to the soil pools
   void TransferRootMatter(decomposable * rootMatter,double * RootLengthList);
   //! returns the proportion of light intercepted by green leaf area
   double fPAR(double k, double gLAI);
   //!calculates the fraction of potential evapotranspiration that is partitioned to the soil
   double FractionEPotToSoil(double gLAI, double yLAI);
   //!calculates the fraction of potential evapotranspiration that is partitioned to the green leaves
   double FractionEPotToGreenLeaves(double gLAI);
   //!calculates the fraction of potential evapotranspiration that is partitioned to the yellow leaves
   double FractionEPotToYellowLeaves(double gLAI, double yLAI);
   //!checks to ensure the nitrogen released by the soil to the plants does not exceed demand or is less than zero
   void CheckNfromSoil(nitrogen &NfromSoil,double NitrogenDemand);
   //!checks for an error in the calculation of transpiration
   void CheckTranspirationRatio(double &TranspirationRatio);
   //!calculates the relative effects of carbon dioxide concentration on the transpiration rate. Returns 1 at ppmCO2=377
   double CO2ConcentrationFactor(double CO2conc);
   //!call up the appropriate crop model
   /*!
   \param  crop_id contains a character identifier for the crop
   */
   crop * AllocateCrop(char * crop_id);


public:
	//!construct with arguments
	ecosystem(const char * aName, const int aIndex, const base * aOwner);
	//!copy constructor
	ecosystem(const ecosystem& p);
	//!destructor
   ~ecosystem();
   //!creates a clone of the current ecosystem
   ecosystem* clone() const;

   //!initialises the soil module
   /*!
   \param soilFileName name of the file containing soil initialisation parameters
   \param aIndex value to be given to the soil object's  Index parameter (currently set to parent field's Index value)
   }*/
   void Initialise(char * soilFileName, int theIndex = 0, double anArea = 0.0);
   //!performs all major interactions between crops and soil
   void CropSoilExchange();
   //!complete indicator values
   void GiveIndicators();

   //!returns whether irrigation is required
   bool GetIrrigationDemand();
   //! returns true if the main crop is ready for harvesting
   bool GetReadyForHarvestOrLater();
   //!handles all major field operations
//   void HandleOp(fieldOperationFields * FOP);
   //!it turns the total amount of nitrogen in the aboveground crop
   double GetAbovegroundCropN();


   //!returned percentage contribution of clover to aboveground crop dry matter
   double CloverPercentage();

   // Budgets
   //!initialised the budgets of those elements or compounds that are being budgeted
   void StartBudget();
   //!check the budgets of those elements or compounds that are being budgeted
   bool EndBudget();

   //!return the total amount of nitrogen in the crops
   double NitrogenInCrops();
   //! return the total amount of nitrogen in the soil
   double NitrogenInSoil();



   //!it turns the maximum height of the vegetation in the ecosystem
	double GetMaxCropHeight();
   //!calculates then returns an object describing the animal feed available within the ecosystem
   /*!
   \param  bite_depth_factor depth to which animals graze, as a proportion of the canopy height
   \param min_stubble_ht minimum height of plant canopy of the grazing - limits the depth of grazing
   \param animalType identifies the type of animal that is grazing
   */
	void GetFeedResource(feedItem * my_resource, double grazing_height, int animalType);

	//!returns the standing dry matter density in the ecosystem, in grams/m**2
	double GetStandingDMdensity();
	//!Sets the crop variables that record the material grazed
	/*!Sets the crop variables that record the material grazed
		Called by each animal that grazes this ecosystem
   \param bite_depth_factor  depth of biting as a proportion of canopy height
   \param  DMGrazed kilograms of dry matter grazed by the animals
   \param NGrazed kilograms of N grazed
   */
	void SetGrazed(double grazing_height, double DMGrazed);


   //------------ The below functions are not truly implemented @@@!!!!!!!!!!!!!!!!!!!!!!!!!


   //!Get amount grazed from this ecosystem during the current period (kg)
   double GetGrazedDM();
   //!returns the daily dry matter production in kilograms per hectare
   double GetDailyDMProduction();
   //! returns the dry matter growth rate of the ecosystem the absence of nutrient or water limitations
   /*!
   \param radiation radiation in megajoules per metre squared
   \param temperature mean daily air temperature
   */
	double GetPotentialGrowthRate(double radiation, double temperature);
   //!zeroes all ecosystem variables related to grazing
	void clearGrazingRecords();
   void AddDecomposable(decomposable* produc) {aSoil->AddDecomposable(produc);};

   void   AddNutrient(nitrogen surfNitrate,nitrogen surfAmmonium, nitrogen soilNitrate, nitrogen soilAmmonium)
   		 {aSoil->AddNutrient(surfNitrate,surfAmmonium,soilNitrate,soilAmmonium);};
	void   AddChloride(double amount) {aSoil->AddChloride(amount);};
	void SetirrigationWater(water *aWater);
   void Sow(char * cropID, double SeedDensDryMatt,double RelativeDensity,double NitrogenInSeed, double CarbonInSeed);
      //!handles all major field operations
   void HarvestOperations(harvestFields * hrv);
   void HarvestOp(harvestFields * hrv);
   void ForageHarvestOp(harvestFields * hrv);
   void Gathering(harvestFields * hrv);
   //! terminates all crops present and adds their organic matter to the soil
   void KillAllCrops();
   void Tillage(soilTreatFields *FOP);

   //!simple grazing routine
   void SimpleGraze();

	void outputCropDetails(fstream *afile);


};

#endif

#ifdef NITROSCAPE
/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef MMSClass_H
   #define MMSClass_H





#include "../base/common.h"
#include "../base/bstime.h"
#include "../products/products.h"
#include "../base/message.h"

#include "../building/buildings.h"
#include "../base/climate.h"
#include "../event/eventControl.h"

#include "../base/controlParameters.h"
#include "../technics/technics.h"
#include "../manager/manureManager.h"
#include "../tools/fileAccess.h"
#include "../Nitroscape/pixiData.h"
#include "../legislation/legislation.h"
#include <string>


#include "../Nitroscape/harvInfoClass.h"
#include "../Nitroscape/patchinterface.h"
using namespace std;
//! Describes the manure management system and the livestock
class MMSClass:base
{
   private:

   struct emission_sources
   {
   	int location;
   //!The emission of ammonia from the individual animal houses (kilograms NH3-N per day)
      double NH3Emission;
   //!The emission of N2O from the individual manure storage (kilograms N2O-N per day)
      double N2OEmission;
   //!The emission of NO from the individual manure storage (kilograms NO-N per day)
      double NOEmission;
      string name;
   }
   houseEmission[20],
   storeEmission[20];

	eventControl *myEventControl;
 //  int ecoNums[100];
	int simYear;

   int farmNo;

   //!Zeros all patch output variables
	void zeroPatchOutputVariables();

typedef char string100[100];


   products * myProducts;
   message * myMessage;

   IndicatorOutput * myOutput;

   technics * myTechnics;
   buildings * myBuildings;
   legislate * myLegislation;
	climate * myClimate;
   controlParameters * myControlParameters;
   manureManager *  myManure_manager;

   bsTime *stopTime;
   bsTime *myTime;

   string100
         myInputDirectory,
         myOutputDirectory,
         myClimateDirectory,
         myClimateFileName,
         mybaseOutputDir;

	public:
//!Default constructor
   MMSClass(int aNum) {farmNo=aNum;}
   //!destructor
	~MMSClass(){Destroy_MMS();};
	eventControl * getEeventControl(){return myEventControl;};
   //!connects patches to the patch interfaces
   /*!
   \param aEventControl pointer to the instance of the manure management system class
   */
	void InitialiseInterface(eventControl *aEventControl);
   //!Zero is all farm-scale variables
	void zeroFarmOutputVariables();



   //!Returns the ammonia emission from a particular animal house (kilograms N per day)
   /*!
   \param houseNo index of the animal house for which the ammonia emissions is required
   */
	double GetMMSHouseAmmonia(int houseNo);
   //!Returns the ammonia emission from all animal houses (kilograms N per day)
	double GetMMSHouseAmmonia();
   //!Returns the N2O emission from a particular animal house (kilograms N per day)
   /*!
   \param houseNo index of the animal house for which the ammonia emissions is required
   */
	double GetMMSHouseN2O(int houseNo);
   //!Returns the N2O emission from all animal houses (kilograms N per day)
	double GetMMSHouseN2O();
   //!Returns the NO emission from a particular animal house (kilograms N per day)
   /*!
   \param houseNo index of the animal house for which the ammonia emissions is required
   */
	double GetMMSHouseNO(int houseNo);
   //!Returns the NO emission from all animal houses (kilograms N per day)
	double GetMMSHouseNO();

   //!Returns the ammonia emission from a particular manure store (kilograms N per day)
   /*!
   \param storeNo index of the manure storage for which the ammonia emissions is required
   */
	double GetMMSStoreAmmonia(int storeNo);
   //!Returns the ammonia emission from all manure stores (kilograms N per day)
	double GetMMSStoreN2O();
   //!Returns the N2O emission from a particular manure store (kilograms N per day)
   /*!
   \param storeNo index of the manure storage for which the ammonia emissions is required
   */
	double GetMMSStoreN2O(int storeNo);
   //!Returns the N2O emission from all manure stores (kilograms N per day)
	double GetMMSStoreAmmonia();
   //!Returns the NO emission from a particular manure store (kilograms N per day)
   /*!
   \param storeNo index of the manure storage for which the ammonia emissions is required
   */
	double GetMMSStoreNO(int storeNo);
   //!Returns the NO emission from all manure stores (kilograms N per day)
	double GetMMSStoreNO();
   //!Returns the number of animal houses on the farm
   int GetNumberOfHouses(){return theBuildings->GettheStables()->NumOfNodes();}
   //!Returns the number of manure storage facilities on the farm
   int GetNumberOfStores(){return theBuildings->GettheStables()->NumOfNodes();}

	//!initialises the manure management system model
   /*!
   \param inputDirectory name of the directory from which input data and parameters should be obtained
   \param climateDirectory name of the directory containing the climate data file
   \param climateFileName name of the file containing the climate data
   \param climateFileType type of climate file
   \param outputDirectory name of the output directory
   */
//	void initialise_MMS(string inputDirectory, string climateDirectory, string climateFileName, int climateFileType, string outputDirectory);
	void initialise_MMS(int farmNo,string orgPath);
	//!calls the daily routine of the manure management system model
	void DoMMSDaily();
   //!cleans up the memory occupied by the manure management system
	void Destroy_MMS();
   //!sets global variables to point to local classes
	void SetMMSGlobals();
	//!Returns the number of fields
   int GetNumberFields(){return myEventControl->getCropRotation()->GetFields()->NumOfNodes();}
   //!Returns the name of the input directory
   char *GetMyInputDirectory() {return myInputDirectory;}
   //!Returns the name of the output directory
   char *GetMyOutputDirectory() {return myOutputDirectory;}
   //!Returns the name of the directory containing the climate file
   char *GetMyClimateDirectory() {return myClimateDirectory;}
   //!Returns the name of the climate file
   char *GetMyClimateFileName() {return myClimateFileName;}
   //!Not used?
   char *GetMybaseOutputDir() {return mybaseOutputDir;}
   //!Returns true if the simulation has been completed
   bool GetfinishedRun();
  	//!Returns the farm identification number
	int GetfarmNo() {return farmNo;}
//   SetecoNum(int index, int Val){ecoNums[index]=Val;}
  // GetecoNum(int index) {return ecoNums[index];}
	//!returns number of patches on the farm
	   int GetNumOfPatches();

	   //!logs the status of an ecosystem
	   /*!
	   \param patchNo number of the relevant ecosystem
	   \param ReadyForHarvestOrLater is greater than zero if any crop present is ready for harvesting
	   \pram  IrrigationDemand the amount of water necessary (in millimetres) to return the soil to field capacity
	   \param theHarvestProducts linked list of pointers to the class containing information about the harvestable crop
	   */
		void SetEcoStatus(int patchNo, double ReadyForHarvestOrLater,
	      double IrrigationDemand, linkList <harvInfoClass> *theHarvestProducts);
	   //!checks whether a tillage operation should be performed. If so returns true
	   /*!
	   \param patchNo number of the relevant ecosystem
	   \param theType tillage identifier
	   \param theDepth depths to which tillage is performed
	   \param theFractionIncorporated the fraction of any organic matter on the soil surface that is incorporated during tillage
	   */
		bool GetMMSTillage(int patchNo, double *theType, double *theDepth, double *theFractionIncorporated);
	   //!checks whether either manure or plant residues should be applied to the soil. If so returns true
	   /*!
	   \param patchNo number of the relevant ecosystem
	   \param theType the type of material added. If less than 100, the material is manure, is greater than 100 it is plant residues
	   \param theamount the amount of material added (tonnes fresh weight per hectare)
	   \param theDepth the depth into the soil to which the material is applied
	   \param theTotalN the total nitrogen applied (kilogram per hectare)
	   \param theNH4N the ammonium nitrogen applied (kilograms per hectare)
	   \param theNO3N the nitrate nitrogen applied (kilograms per hectare)
	   \param thepercentDM the dry matter content (percentage)
	   \param theC the Carbon applied (kilograms per hectare)
	   */
		bool GetMMSAddOrganic(int patchNo, double *theType, double *theamount, double *theDepth, double *theTotalN, double *theNH4N,
			double *theNO3N, double *thepercentDM, double *theC);
	   //!checks whether mineral nitrogen fertiliser should be applied to the soil. If so returns true
	   /*!
	   \param patchNo number of the relevant ecosystem
	   \param theNH4N the ammonium nitrogen applied (kilograms per hectare)
	   \param theNO3N the nitrate nitrogen applied (kilograms per hectare)
	   */
		bool GetMMSDoFertilisation(int patchNo, double *theNH4N, double *theNO3N);
	   //! checks whether a crop should be sown. If so, returns true
	   /*!
	   \param patchNo number of the relevant ecosystem
	   \param theSeedCode identifier of the crop to be sown
	   \param theSeedDM the amount of seed applied (kilograms dry matter per hectare)
	   \param theSeedN  the amount of nitrogen applied in seed (kilograms per hectare)
	   */
		bool GetMMSDoSowing(int patchNo, double *theSeedCode, double *theSeedDM, double *theSeedN);

	   //!checks whether a harvesting operation should be performed. If so, returns true
	   /*!
	   \param patchNo number of the relevant ecosystem
		\param theharvestCode harvest type identifier
	   \param thepropGrainHarvested proportion of the grain that is harvested and removed
	   \param thepropStrawHarvested proportion of the straw that is harvested and removed
	   */
		bool GetMMSDoHarvesting(int patchNo, double *theharvestCode, double *thepropGrainHarvested, double *thepropStrawHarvested);
	   //!Returns the ammonia emission from fertiliser and manure applied to a particular patch (kilograms N per day)
	   /*!
	   \param patchNo number of the relevant ecosystem
		*/
		double GetMMSPatchAmmonia(int patchNo);
	   //!Returns the ammonia emission from fertiliser and manure applied to fields (kilograms N per day)
		double GetMMSSpreadAmmonia();
};

#endif

#endif
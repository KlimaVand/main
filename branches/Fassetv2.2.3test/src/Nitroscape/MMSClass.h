/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef MMSClass_H
   #define MMSClass_H



#include "../data/systemData.h"
#include "../data/outputData.h"
#include "../base/common.h"
#include "../base/bstime.h"
#include "../products/products.h"
#include "../base/message.h"
#include "../legislation/legislation.h"
#include "../building/thebuild.h"
#include "../base/climate.h"
#include "../event/eventControl.h"
#include "../base/timeUse.h"
#include "../base/controlParameters.h"
#include "../technics/technics.h"
#include "../manager/manureManager.h"
#include "../tools/fileAccess.h"
#include "../data/farmData.h"
#include "../data/environData.h"
#include "../data/outputData.h"
#include "../data/commonData.h"
#include <string>


#include "../Nitroscape/harvInfoClass.h"
#include "../Nitroscape/patchinterface.h"
using namespace std;
//! Describes the manure management system and the livestock
class MMSClass
{
   private:

   //!The emission of ammonia from the individual animal houses (kilograms NH3-N per day)
   double houseNH3Emission[20];
   //!The emission of ammonia from the individual manure storage (kilograms NH3-N per day)
   double storeNH3Emission[20];


	eventControl *myEventControl;
	int simYear;

   //!Zeros all patch output variables
	void zeroPatchOutputVariables();

typedef char string100[100];

	linkList<patchInterfaceClass> * theInterfacePatches;
   products * myProducts;
   message * myMessage;
	timeUse* mytimeConsumption;
   IndicatorOutput * myOutput;
   legislate * myLegislation;
   technics * myTechnics;
   buildings * myBuildings;
	climate * myClimate;
   controlParameters * myControlParameters;
   manureManager *  myManure_manager;
   outputData* myOutputData;
   systemData *mySystemData;
   environData *myEnvironData;
   farmData *myFarmData;
   bsTime stopTime;
   bsTime *myTime;

   string100
         myInputDirectory,
         myOutputDirectory,
         myClimateDirectory,
         myClimateFileName,
         mybaseOutputDir;

	public:
//!Default constructor
   MMSClass(){};
   //!destructor
	~MMSClass(){Destroy_MMS();};

   //!connects patches to the patch interfaces
   /*!
   \param aEventControl pointer to the instance of the manure management system class
   */
	void InitialiseInterface(eventControl *aEventControl);
   //!Zero is all farm-scale variables
	void zeroFarmOutputVariables();

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
   //!Returns the ammonia emission from a particular animal house (kilograms N per day)
   /*!
   \param houseNo index of the animal house for which the ammonia emissions is required
   */
	double GetMMSHouseAmmonia(int houseNo);
   //!Returns the ammonia emission from all animal houses (kilograms N per day)
	double GetMMSHouseAmmonia();
   //!Returns the ammonia emission from a particular manure store (kilograms N per day)
   /*!
   \param storeNo index of the manure storage for which the ammonia emissions is required
   */
	double GetMMSStoreAmmonia(int storeNo);
   //!Returns the ammonia emission from all manure stores (kilograms N per day)
	double GetMMSStoreAmmonia();
   //!Returns the number of animal houses on the farm
   int GetNumberOfHouses(){return theBuildings->GettheStables()->NumOfNodes();}
   //!Returns the number of manure storage facilities on the farm
   int GetNumberOfStore(){return theBuildings->GettheStables()->NumOfNodes();}

	//!initialises the manure management system model
   /*!
   \param inputDirectory name of the directory from which input data and parameters should be obtained
   \param climateDirectory name of the directory containing the climate data file
   \param climateFileName name of the file containing the climate data
   \param climateFileType type of climate file
   \param outputDirectory name of the output directory
   */
//	void initialise_MMS(string inputDirectory, string climateDirectory, string climateFileName, int climateFileType, string outputDirectory);
	void initialise_MMS(systemRunData aRunData);
	//!calls the daily routine of the manure management system model
	void DoMMSDaily();
   //!cleans up the memory occupied by the manure management system
	void Destroy_MMS();
   //!sets global variables to point to local classes
	void SetMMSGlobals();

   int GetNumberFields(){return myEventControl->GettheCropRotation()->GetFields()->NumOfNodes();}

};

#endif


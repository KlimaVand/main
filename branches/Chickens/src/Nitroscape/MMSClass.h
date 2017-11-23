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
#include "../base/timeuse.h"
#include "../base/controlParameters.h"
#include "../technics/technics.h"
#include "../manager/manureManager.h"
#include "../tools/fileAccess.h"
#include "../data/farmData.h"
#include "../data/environData.h"
#include "../data/outputData.h"
#include "../data/commonData.h"
#include "../data/pixiData.h"
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
	void initialise_MMS(systemRunData aRunData);
	//!calls the daily routine of the manure management system model
	void DoMMSDaily();
   //!cleans up the memory occupied by the manure management system
	void Destroy_MMS();
   //!sets global variables to point to local classes
	void SetMMSGlobals();
	//!Returns the number of fields
   int GetNumberFields(){return myEventControl->GettheCropRotation()->GetFields()->NumOfNodes();}
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

};

#endif


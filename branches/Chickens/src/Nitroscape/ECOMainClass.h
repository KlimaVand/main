/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef ECOMAINClass_H
   #define ECOMAINClass_H



#include "../data/systemData.h"
#include "../data/outputData.h"
#include "../base/common.h"
#include "../base/bstime.h"
#include "../products/products.h"
#include "../base/message.h"
#include "../base/climate.h"
#include "../base/timeUse.h"
#include "../base/controlParameters.h"
#include "../tools/fileAccess.h"
#include "../data/environData.h"
#include "../data/outputData.h"
#include "../data/commonData.h"
#include <string>


#include "../Nitroscape/harvInfoClass.h"
#include "../Nitroscape/ecosystinterface.h"
using namespace std;

//! Describes the manure management system and the livestock
class ECOMainClass
{
   private:

   //!The emission of ammonia from the individual animal houses (kilograms NH3-N per day)
   double houseNH3Emission[20];
   //!The emission of ammonia from the individual manure storage (kilograms NH3-N per day)
   double storeNH3Emission[20];


typedef char string100[100];

linkList <ecosystinterfaceClass> *theEcoInterfaces;

   IndicatorOutput * myOutput;
   products * myProducts;
   message * myMessage;
	timeUse* mytimeConsumption;
	climate * myClimate;
   controlParameters * myControlParameters;
   outputData* myOutputData;
   systemData *mySystemData;
   environData *myEnvironData;

   string100
         myInputDirectory,
         myOutputDirectory,
         myClimateDirectory,
         myClimateFileName,
         mybaseOutputDir;

	public:
//!Default constructor
   ECOMainClass(){};
   //!destructor
	~ECOMainClass(){Destroy_ECOs();};

//!Initialises the ecosystem models
/*!
\param numEcosysts Number of ecosystem models to initialise
\param direct Input data directory
\param climateDir Climate data directory
\param climateFileName Climate data file name
\param outputDir Output data directory
*/
//void initialise_ECO(int numEcosysts, systemRunData aRunData, int *startYear, int *stopYear);
void initialise_ECO(int numEcosysts, systemRunData aRunData);
void DoDaily_ECO();
void Destroy_ECOs();
 //!Interrogates an individual ecosystem model and requests its status
 /*!
 \param ecoNum Identification number of the ecosystem to be interrogated
 \param TotalLAI Leaf Area Index of vegetation in the ecosystem
 \param ReadyForHarvestOrLater Equals 1.0 if any crop present is sufficiently mature to be harvested, otherwise equals 0.0
 \param IrrigationDemand Amount of water necessary to bring the soil to field capacity (millimetres)
 \param theHarvestProducts List of pointers to the class that describes the dry matter, nitrogen and carbon in harvestable material
 */
void GetEcoStatus(int ecoNum, double *TotalLAI, double *ReadyForHarvestOrLater,
      double *IrrigationDemand, linkList <harvInfoClass> *theHarvestProducts);

   //!Requests a particular ecosystem to conduct a tillage operation
   /*!
   \param  ecoNum Identification number of the targeted ecosystem
   \param  theType Number identifying the type of operation (e.g. ploughing, harrowing)
   \param  theDepth The depth to which the tillage operation  should occur (millimetres)
   \param  theFractionIncorporated The fraction of any surface material (e.g. crop residues) that would be incorporated by the tillage operation
   */
  void SetECOTillage(int ecoNum, double theType, double theDepth, double theFractionIncorporated);
   //!Applies organic material to a particular ecosystem
   /*!
   \param  ecoNum Identification number of the targeted ecosystem
   \param  theType Number identifying the type of operation (e.g. muck spreading)
   \param  theamount The fresh weight of organic material applied (grams per square metre)
   \param  theDepth The depth into the soil that the organic material is applied (millimetres)
   \param  theTotalN The total nitrogen applied (grams per square metre)
   \param  theNH4N  The ammonium nitrogen applied (grams per square metre)
   \param  theNO3N The nitrate nitrogen applied (grams per square metre)
   \param  thepercentDM The percentage dry matter in fresh weight
   \param  theC The amount of carbon applied (grams per square metre)
   */
   void SetECOAddOrganic(int ecoNum, double theType, double theamount, double theDepth, double theTotalN, double theNH4N,
         double theNO3N, double thepercentDM, double theC);
   //!Add mineral fertiliser to a particular ecosystem
   /*!
   \param  ecoNum Identification number of the targeted ecosystem
   \param  theNH4N  The ammonium nitrogen applied (grams per square metre)
   \param  theNO3N The nitrate nitrogen applied (grams per square metre)
   */
   void SetECOFertilisation(int ecoNum, double theNH4N, double theNO3N);
   //!Sow a plant species into a particular ecosystem
   /*!
   \param  ecoNum Identification number of the targeted ecosystem
   \param  theSeedCode Unique code to indicate the plant species to be sown
   \param theSeedDM Amount of dry matter in the seed (grams per square metre)
   \param theSeedN Amount of nitrogen in the seed (grams per square metre)
   */
   void SetECODoSowing(int ecoNum, double theSeedCode, double theSeedDM, double theSeedN);
   //!Instructs a particular ecosystem to harvest the crop present
   /*!
   \param  ecoNum Identification number of the targeted ecosystem
   \param  theharvestCode Unique code to identify the type of harvesting (e.g. combine harvesting with straw shredding)
   \param  thepropGrainHarvested The proportion of the grain harvested (the remainder is returned to the soil as waste)
   \param  thepropStrawHarvested The proportion of the straw harvested (the remainder is returned to the soil as waste)
   */
   void SetECODoHarvesting(int ecoNum, double theharvestCode, double thepropGrainHarvested, double thepropStrawHarvested);
	void SetECOGlobals();


/*   char *GetMyInputDirectory() {return myInputDirectory;}
   char *GetMyOutputDirectory() {return myOutputDirectory;}
   char *GetMyClimateDirectory() {return myClimateDirectory;}
   char *GetMyClimateFileName() {return myClimateFileName;}
   char *GetMybaseOutputDir() {return mybaseOutputDir;}*/
};

#endif


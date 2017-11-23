/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifdef __BCplusplus__
#include "../data/systemData.h"
#include "../data/outputData.h"
#include "../base/common.h"
#include "../base/bstime.h"
#include "../products/products.h"
#include "../base/message.h"
#include "../legislation/legislation.h"
#include "../base/climate.h"
#include "../base/timeuse.h"
#include "../base/controlParameters.h"
#include "../technics/technics.h"
#include "../tools/fileAccess.h"
#include "../data/farmData.h"
#include "../data/environData.h"
#include "../data/outputData.h"
#include "../data/commonData.h"
#include "../Nitroscape/ecosystinterface.h"
#include "../Nitroscape/ECOMainClass.h"
#endif
#ifndef __BCplusplus__

#include "data/systemData.h"
#include "data/outputData.h"
#include "base/common.h"
#include "base/bstime.h"
#include "products/products.h"
#include "base/message.h"
#include "legislation/legislation.h"
#include "base/climate.h"
#include "base/timeUse.h"
#include "base/controlParameters.h"
#include "technics/technics.h"
#include "tools/fileAccess.h"
#include "data/farmData.h"
#include "data/environData.h"
#include "data/outputData.h"
#include "data/commonData.h"
#include "data/systemData.h"
#include "../Nitroscape/ECOMainClass.h"
#endif
using namespace std;


extern "C"
{

//!Initialises the ecosystem models
/*!
\param numEcosysts Number of ecosystem models to initialise
\param direct Input data directory
\param climateDir Climate data directory
\param climateFileName Climate data file name
\param outputDir Output data directory
*/
//void ECOMainClass::initialise_ECO(int numEcosysts, systemRunData aRunData,  int *startYear, int *stopYear)
void ECOMainClass::initialise_ECO(int numEcosysts, systemRunData aRunData)
{

   	mySystemData = new systemData(false);
	   mySystemData->SetSystemRunData(aRunData);
//   *startYear=mySystemData->getStartYear();
//   *stopYear=mySystemData->getStopYear();
	strcpy(myInputDirectory,mySystemData->getInputDirectory().c_str());
	strcpy(myOutputDirectory,mySystemData->getOutputDirectory().c_str());
   strcpy(myClimateDirectory,mySystemData->getClimateDirectory().c_str());
   strcpy(myClimateFileName,mySystemData->getClimateFileName().c_str());
   myOutputData = new outputData();
   myEnvironData = new environData();;

   int ClimateFileType = mySystemData->getClimateFileType();
   int EpotCalculationMethod = 0;

   int VolatModel=0;
//	myTime = new bsTime(1, 8, mySystemData->getStartYear());
//   stopTime.SetTime(31,7,mySystemData->getStartYear());

   bool
   UseReductionParameters=false, 		 // Default determined by theControlParameters
   MeasuredSoilTemperature=false,       // Default determined by theControlParameters
   WriteSoilTemperature=false,
   SimpleTemperatureModel=false,        // Default determined by theControlParameters
   // WriteDetails,                  // Default determined by theControlParameters
   GrassWeed=false,                     // Default determined by theControlParameters
   Organic=false, 					    	 // Default determined by theControlParameters
   ForcedOperations=false,              // Default determined by theControlParameters
   ForcedHarvest=false;

   myControlParameters = new controlParameters();

   EpotCalculationMethod=mySystemData->getEpotCalculationMethod();

   myControlParameters->SetvolatModel(VolatModel);
   int wfm=mySystemData->getWaterFlowMethodInt();
   myControlParameters->SetWaterFlowMethod(wfm);
   if(mySystemData->getSimpleTemperatureModelFile()==true)
   {
      SimpleTemperatureModel=mySystemData->getSimpleTemperatureModel();
      myControlParameters->SetSimpleTemperatureModel(SimpleTemperatureModel);
   }
   if(mySystemData->getMeasuredSoilTemperatureFile()==true)
   {
      MeasuredSoilTemperature=mySystemData->getMeasuredSoilTemperature();
      myControlParameters->SetMeasuredSoilTemperature(MeasuredSoilTemperature);
   WriteSoilTemperature=mySystemData->getWriteSoilTemperature();
   myControlParameters->SetWriteSoilTemperature(WriteSoilTemperature);
   }
   double HarvestWaste=0.05;
   HarvestWaste=mySystemData->getHarvestWaste();
   if(mySystemData->getHarvestWasteFile()==true)
   {
    myControlParameters->SetHarvestWaste(HarvestWaste); // 0.05 - 0.10 may be a sensible range
   }


   double StrawLeft= mySystemData->getStrawLeft();
   myControlParameters->SetStrawLeft(StrawLeft);
      myControlParameters->SetGrassWeed(GrassWeed);

   char FN1[100],FN2[100], FN3[100], FN4[100], FN5[100];
      strcpy(FN1,myOutputDirectory);
      strcat(FN1,"ECOwarnings.txt");
      strcpy(FN2,myOutputDirectory);
      strcat(FN2,"ECOlogfile.txt");
      strcpy(FN3,myOutputDirectory);
      strcat(FN3,"ECOdebug.txt");      //debug file name
      strcpy(FN4,myOutputDirectory);
      strcat(FN4,"fieldfile1.txt");
      strcpy(FN5,myOutputDirectory);
      strcat(FN5,"fieldfile2.txt");

   myOutput                    = new IndicatorOutput;
   myProducts                  = new products;
   myMessage                   = new message;
   myClimate                   = new climate(ClimateFileType,EpotCalculationMethod);
   mytimeConsumption				 = new timeUse();
	myProducts->InitializeProducts("products.dat");


   SetECOGlobals();

   myClimate->InitClimate(myClimateDirectory,myClimateFileName);
   myMessage->InitMessage(FN1,FN2,FN3,FN4,FN5);

   char filenameCSV[100];
#ifndef __BCplusplus__
   sprintf(filenameCSV,"%s/INDICATX.XLS",myOutputDirectory);
#else
   sprintf(filenameCSV,"%s\\INDICATX.XLS",myOutputDirectory);
#endif
   theOutput->Initialize(filenameCSV);

   if (chdir(myOutputDirectory)!=0)
#ifdef __ANSICPP__
	#ifdef BUILDER
	  mkdir(myOutputDirectory);
	#endif
	#ifdef UNIX
	  mkdir(myOutputDirectory,'-p');
	#endif
#ifdef CYGWIN
	  mkdir(myOutputDirectory,'-p');
#endif
#ifdef __BCplusplus__
	  mkdir(myOutputDirectory);
	#endif                                     				// make output directory if not present
#else
//	#ifdef BUILDER
	  mkdir(myOutputDirectory);
#endif
   if (chdir(myInputDirectory)!=0)
		myMessage->FatalError("Event:: input directory ",myInputDirectory," not found!");

      theEcoInterfaces = new linkList <ecosystinterfaceClass>;
      for (int i=0; i<numEcosysts; i++)
      {
         ecosystinterfaceClass * anEcoInterface = new ecosystinterfaceClass(i,"JB4-M.dat",1.0);
         anEcoInterface->UpdateStatus();
         theEcoInterfaces->InsertLast(anEcoInterface);
      }
	delete mySystemData;
}
   //!Executes each ecosystem's daily routine in turn
   void ECOMainClass::DoDaily_ECO()
   {
		SetECOGlobals();
      theClimate->Update();          //Load today's climate
      for (int i=0; i<theEcoInterfaces->NumOfNodes(); i++)
      {
         ecosystinterfaceClass * anEcoInterface = theEcoInterfaces->ElementAtNumber(i);
         anEcoInterface->DoDailyEco();
      }
      theOutput->DailyUpdate();
   }

   void ECOMainClass::Destroy_ECOs()
   {
      delete myProducts;
      delete myMessage;
      delete myOutput;
      delete myClimate;
		delete timeConsumption;
      delete myControlParameters;
      delete myOutputData;
      delete myEnvironData;
      delete theEcoInterfaces;
   }



 //!Interrogates an individual ecosystem model and requests its status
 /*!
 \param ecoNum Identification number of the ecosystem to be interrogated
 \param TotalLAI Leaf Area Index of vegetation in the ecosystem
 \param ReadyForHarvestOrLater Equals 1.0 if any crop present is sufficiently mature to be harvested, otherwise equals 0.0
 \param IrrigationDemand Amount of water necessary to bring the soil to field capacity (millimetres)
 \param theHarvestProducts List of pointers to the class that describes the dry matter, nitrogen and carbon in harvestable material
 */
	void ECOMainClass::GetEcoStatus(int ecoNum, double *TotalLAI, double *ReadyForHarvestOrLater,
      double *IrrigationDemand, linkList <harvInfoClass> *theHarvestProducts)
      {
        ecosystinterfaceClass * anEcoInterface = theEcoInterfaces->ElementAtNumber(ecoNum);
        *TotalLAI = anEcoInterface->GetTotalLAI();
        if (anEcoInterface->GetReadyForHarvestOrLater())
      	*ReadyForHarvestOrLater= 1.0;
        else
      	*ReadyForHarvestOrLater= 0.0;
      	*IrrigationDemand = anEcoInterface->GetIrrigationDemand();
//	      IrrigationWater = anEcoInterface->GetIrrigationWater();
//   	   IrrigationN = anEcoInterface->GetIrrigationN();
//double IrrigationWater, double IrrigationN,
/*         *AbovegroundCropN = anEcoInterface->GetAbovegroundCropN();
         *MaxCropHeight = anEcoInterface->GetMaxCropHeight();
         *StandingDMdensity = anEcoInterface->GetStandingDMdensity();*/
         theHarvestProducts->ForgetAll();
         int numHarvProducts = anEcoInterface->GetharvestStuff()->NumOfNodes();
         for (int i=0;i<numHarvProducts; i++)
         {
         	harvInfoClass * aharvInfoClass = anEcoInterface->GetharvestStuff()->ElementAtNumber(i);
            theHarvestProducts->InsertLast(aharvInfoClass);
         }
     }
   //!Requests a particular ecosystem to conduct a tillage operation
   /*!
   \param  ecoNum Identification number of the targeted ecosystem
   \param  theType Number identifying the type of operation (e.g. ploughing, harrowing)
   \param  theDepth The depth to which the tillage operation  should occur (millimetres)
   \param  theFractionIncorporated The fraction of any surface material (e.g. crop residues) that would be incorporated by the tillage operation
   */
  void ECOMainClass::SetECOTillage(int ecoNum, double theType, double theDepth, double theFractionIncorporated)
   {
		SetECOGlobals();
      ecosystinterfaceClass * anEcoInterface = theEcoInterfaces->ElementAtNumber(ecoNum);
      int aType = (int) theType;
      anEcoInterface->Tillage(aType, theDepth, theFractionIncorporated);

   }
   //      int KillAllCrops;  //needs to be reset daily

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
   void ECOMainClass::SetECOAddOrganic(int ecoNum, double theType, double theamount, double theDepth, double theTotalN, double theNH4N,
         double theNO3N, double thepercentDM, double theC)
   {
		SetECOGlobals();
      ecosystinterfaceClass * anEcoInterface = theEcoInterfaces->ElementAtNumber(ecoNum);
      anEcoInterface->AddOrganic(theType, theamount, theDepth, theTotalN, theNH4N,
         theNO3N, thepercentDM, theC);

   }
   //!Add mineral fertiliser to a particular ecosystem
   /*!
   \param  ecoNum Identification number of the targeted ecosystem
   \param  theNH4N  The ammonium nitrogen applied (grams per square metre)
   \param  theNO3N The nitrate nitrogen applied (grams per square metre)
   */
   void ECOMainClass::SetECOFertilisation(int ecoNum, double theNH4N, double theNO3N)
   {
		SetECOGlobals();
      ecosystinterfaceClass * anEcoInterface = theEcoInterfaces->ElementAtNumber(ecoNum);
      anEcoInterface->SetFertilisation(theNH4N, theNO3N);
   }

   //!Sow a plant species into a particular ecosystem
   /*!
   \param  ecoNum Identification number of the targeted ecosystem
   \param  theSeedCode Unique code to indicate the plant species to be sown
   \param theSeedDM Amount of dry matter in the seed (grams per square metre)
   \param theSeedN Amount of nitrogen in the seed (grams per square metre)
   */
   void ECOMainClass::SetECODoSowing(int ecoNum, double theSeedCode, double theSeedDM, double theSeedN)
   {
		SetECOGlobals();
      ecosystinterfaceClass * anEcoInterface = theEcoInterfaces->ElementAtNumber(ecoNum);
      anEcoInterface->Sow((int)theSeedCode, theSeedDM, theSeedN);
   }
   //!Instructs a particular ecosystem to harvest the crop present
   /*!
   \param  ecoNum Identification number of the targeted ecosystem
   \param  theharvestCode Unique code to identify the type of harvesting (e.g. combine harvesting with straw shredding)
   \param  thepropGrainHarvested The proportion of the grain harvested (the remainder is returned to the soil as waste)
   \param  thepropStrawHarvested The proportion of the straw harvested (the remainder is returned to the soil as waste)
   */
   void ECOMainClass::SetECODoHarvesting(int ecoNum, double theharvestCode, double thepropGrainHarvested, double thepropStrawHarvested)
   {
		SetECOGlobals();
      ecosystinterfaceClass * anEcoInterface = theEcoInterfaces->ElementAtNumber(ecoNum);
      anEcoInterface->HarvestOperations(theharvestCode, thepropGrainHarvested, thepropStrawHarvested);
   }

}

void ECOMainClass::SetECOGlobals()
{
   theProducts = myProducts;
	theMessage	= myMessage;
	theOutput	= myOutput;
	theClimate = myClimate;
	theControlParameters = myControlParameters;
   theOutputData = myOutputData;
   theEnvironData = myEnvironData;
   timeConsumption = mytimeConsumption;
//   theTime = *myTime;
}


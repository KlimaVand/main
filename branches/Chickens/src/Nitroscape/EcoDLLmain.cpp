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
#endif
using namespace std;

bsTime theTime;
products * theProducts;
message * theMessage;
timeUse* timeConsumption;
IndicatorOutput * theOutput;
systemData * theSystemData;
legislate * theLegislation;
technics * theTechnics;
climate * theClimate;
controlParameters * theControlParameters;
outputData * theOutputData;
farmData * theFarmData;
environData * theEnvironData;

linkList <ecosystinterfaceClass> *theEcoInterfaces;


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
   __declspec(dllexport) void initialise_ECO(int numEcosysts, char * direct, char *climateDir, char * climateFileName, char * outputDir, int day, int month, int year)
   {

   	theSystemData = new systemData(false);
      
      typedef char string100[100];
      string100
         OutputDirectory;
      theMessage                   = new message;
      if (direct)
      {
         if (!chdir(direct)==0)
            theMessage->FatalError("Directory not found! Program terminating ");
         char buffer[MAX_TXT];
         getcwd(buffer, MAX_TXT);
         cout << buffer << endl;
      }
      theProducts                  = new products;
      theOutput                    = new IndicatorOutput;
      theLegislation               = new legislate;
      timeConsumption              = new timeUse;
      theTechnics                  = new technics("technics",0); // BMP
   //   theClimate                   = new climate(ClimateFileType,EpotCalculationMethod); // BMP
      theClimate                   = new climate(); // NJH
      theControlParameters = new controlParameters;

      theTime.SetTime(day,month,year);

      strcpy(OutputDirectory,outputDir);
      string100 FN1,FN2, FN3, FN4, FN5;
      strcpy(FN1,OutputDirectory);
      strcat(FN1,"ECOwarnings.txt");
      strcpy(FN2,OutputDirectory);
      strcat(FN2,"ECOlogfile.txt");
      strcpy(FN3,OutputDirectory);
      strcat(FN3,"ECOdebug.txt");      //debug file name
      strcpy(FN4,OutputDirectory);
      strcat(FN4,"fieldfile1.txt");
      strcpy(FN5,OutputDirectory);
      strcat(FN5,"fieldfile2.txt");
      theMessage->InitMessage(FN1,FN2,FN3,FN4,FN5);

      theProducts->InitializeProducts("products.dat");
      theClimate->InitClimate(climateDir,climateFileName);
      theOutput->Initialize("indicatx.xls");
      theEcoInterfaces = new linkList <ecosystinterfaceClass>;
      for (int i=0; i<numEcosysts; i++)
      {
         ecosystinterfaceClass * anEcoInterface = new ecosystinterfaceClass(i,"JB4-M.dat",0,1.0);
         anEcoInterface->UpdateStatus();
         theEcoInterfaces->InsertLast(anEcoInterface);
      }
   }

   //!Executes each ecosystem's daily routine in turn
   __declspec(dllexport) void DoDaily_ECO()
   {
      if((theTime.GetDay()==17) && (theTime.GetMonth()==9) && (theTime.GetYear()==1987))
      cout << " ";
      cout << theTime << endl;;
      theClimate->Update();          //Load today's climate
      for (int i=0; i<theEcoInterfaces->NumOfNodes(); i++)
      {
         ecosystinterfaceClass * anEcoInterface = theEcoInterfaces->ElementAtNumber(i);
         anEcoInterface->DoDailyEco();
      }
      theOutput->DailyUpdate();
      theTime.AddOneDay();
   }

   __declspec(dllexport)   void Destroy_ECO()
   {
      delete theProducts;
      delete theMessage;
      delete timeConsumption;
      delete theOutput;
      delete theLegislation;
      delete theTechnics;
      delete theClimate;
      delete theControlParameters;
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
	__declspec(dllexport) void GetEcoStatus(int ecoNum, double *TotalLAI, double *ReadyForHarvestOrLater,
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
   __declspec(dllexport) void SetECOTillage(int ecoNum, double theType, double theDepth, double theFractionIncorporated)
   {
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
   __declspec(dllexport)
   void SetECOAddOrganic(int ecoNum, double theType, double theamount, double theDepth, double theTotalN, double theNH4N,
         double theNO3N, double thepercentDM, double theC)
   {
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
   __declspec(dllexport)
   void SetECOFertilisation(int ecoNum, double theNH4N, double theNO3N)
   {
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
   __declspec(dllexport)
   void SetECODoSowing(int ecoNum, double theSeedCode, double theSeedDM, double theSeedN)
   {
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
   __declspec(dllexport)
   void SetECODoHarvesting(int ecoNum, double theharvestCode, double thepropGrainHarvested, double thepropStrawHarvested)
   {
      ecosystinterfaceClass * anEcoInterface = theEcoInterfaces->ElementAtNumber(ecoNum);
      anEcoInterface->HarvestOperations(theharvestCode, thepropGrainHarvested, thepropStrawHarvested);
   }

}




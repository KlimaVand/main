/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "../data/farmData.h"
#include "eventControl.h"
#include "../base/climate.h"
#include "../base/timeUse.h"
#include "../data/systemData.h"
#include "../base/controlParameters.h"

#ifndef __BCplusplus__
	#include <errno.h>
	#include <sys/stat.h>
	#include <sys/types.h>
#endif

	#include <errno.h>
	#include <sys/stat.h>
	#include <sys/types.h>
//CK

/****************************************************************************\
  Default Constructor
\****************************************************************************/
eventControl::eventControl()
   : base()
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
eventControl::eventControl(const char * aName, const int aIndex, const base * aOwner)
   : base(aName, aIndex, aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Destructor
\****************************************************************************/
eventControl::~eventControl()
{
   if (theOperational_manager)
      delete theOperational_manager;
   if (theCropRotation)
  	   delete theCropRotation;
   if (theLivestock)
    delete theLivestock;
}

/**
 * Only called by constructor, to avoid redundant code
 */
void eventControl::InitVariables()
{
   theLivestock           = NULL;
   theCropRotation        = new cropRotation;
	theOperational_manager = new operationsManager() ; // added by NJH
   CapitalInterest        = 0.0;                             // default interest on capital
   MinFieldArea           = 0.0;                             // lowest field area
   ScaleBuildings			  = 0;
   Nbudget.SetNames("farm","N"); // ???!!!
}

/****************************************************************************\
\****************************************************************************/
void eventControl::Initialize(bsTime * stopTime, char* inputDir, char * climateDir, char * climateFilename, char * outputPath)
{
	abort = false;
   char filenameCSV[100];
   char filenameField[100];
   char filenameCattle[100];

#ifndef __BCplusplus__
   sprintf(filenameCSV,"%s/INDICATX.XLS",outputPath);
   sprintf(filenameField,"%s/Fielddata.txt",outputPath);   //@@@
   sprintf(filenameCattle,"%s/Cattledata.txt",outputPath); //@@@
#else
   sprintf(filenameCSV,"%s\\INDICATX.XLS",outputPath);
   sprintf(filenameField,"%s\\Fielddata.txt",outputPath);   //@@@
   sprintf(filenameCattle,"%s\\Cattledata.txt",outputPath); //@@@
#endif
   theOutput->Initialize(filenameCSV);

   int Watering = 1;
   int daysBetweenIrrigation = 0;
   int irrigationDelay = 0;
   //inserted by NJH Oct 2007
   int SelltheStrawProducts = 0;
   int SelltheNonRoughageProducts = 0;
   //
   bool pigfarm = false;
   bool dairyfarm = false;
   bool beeffarm = false;
   int FingerFlow = 0;
   string FarmID = "";

   CapitalInterest=theFarmData->getCapitalInterest();
   MinFieldArea=theFarmData->getMinFieldArea();
   Watering=theFarmData->getWatering();
   daysBetweenIrrigation=theFarmData->getDaysBetweenIrrigation();
   irrigationDelay=theFarmData->getIrrigationDelay();
   FingerFlow=theFarmData->getFingerFlow();
   ScaleBuildings=theFarmData->getScaleBuildings();
   SelltheStrawProducts=theFarmData->getSelltheStrawProducts();
   SelltheStrawProducts=theFarmData->getSelltheNonRoughageProducts();

      //
#ifdef TUPLE
   FarmID=theFarmData->FarmDatas.FarmID;
   theControlParameters->SetFarmID(FarmID);
#endif

   pigfarm=theFarmData->getpigfarm();
   dairyfarm=theFarmData->getdairyfarm();
   beeffarm=theFarmData->getbeeffarm();

   if (pigfarm ||dairyfarm || beeffarm)
      theLivestock = new livestock;

   theProducts->InitializeProducts("products.dat");//,(dairyfarm || beeffarm));
   theProducts->InitializePrices(stopTime);
   theProducts->InitializeFeed("FeedTable2004.dat");
   theClimate->InitClimate(climateDir,climateFilename);
   int returnValue = chdir(inputDir);
   if(returnValue==false)
   	   theMessage->Warning("could not change dir in eventControl::Initialize");
   theCropRotation->Initialize(1,Watering,daysBetweenIrrigation,irrigationDelay,
                               FingerFlow,0,MinFieldArea);
   theProducts->UpdateProductPtr();
   theTechnics->Initialize("operatio.dat","techeqp.dat");
   theBuildings->InitializeAll("building.dat", pigfarm, dairyfarm, beeffarm);
   if (theLivestock)
      theLivestock->Initialize("livestock.dat",pigfarm,dairyfarm,beeffarm,theBuildings, filenameCattle);
   theLegislation->Initialize("legislat.dat");
   theOperational_manager->Initialize(theLivestock,theCropRotation,theBuildings);    //added by NJH 1.5.00

   //inserted by NJH Oct 2007
   theProducts->SetSelltheStrawProducts(SelltheStrawProducts);
   theProducts->SetSelltheNonRoughageProducts(SelltheNonRoughageProducts);
}


/****************************************************************************\
\****************************************************************************/
void eventControl::ReceivePlan(char * fileExtension, char *inputDir)
{
   char fileName[13];
   strcpy(fileName,"Fields.");
   strcat(fileName,fileExtension);

   theCropRotation->ReceivePlan(fileName);
   if (theLivestock)
   {
      theLivestock->ReceivePlan(fileExtension);
   }
   theOperational_manager->ReceivePlan(fileExtension,inputDir);
   //scale buildings after call to theOperational_manager->ReceivePlan, so know which buildings are in use

   if ((theLivestock)&&(!ScaleBuildings==0))
         theBuildings->ScaleBuildings();
}

/****************************************************************************\
Has not been used since 2002 - the LP side of the model is not updatet
\****************************************************************************/
void eventControl::BasicLPParameters()
{
#ifdef __BCplusplus__
   chdir("\\STRATEGI\\LP_MODEL");
#endif
   fstream * f = new fstream;;
	f->open("MINLAND.INC",ios::out);
   *f << "MINLAND The minimum size of land on one field with one crop" << endl;
   *f << "/ " << MinFieldArea << " /" << endl;
   f->close();
   delete f;
}

/****************************************************************************\
Has not been used since 2002 - the LP side of the model is not updatet
\****************************************************************************/
void eventControl::CalcLP(int periods)
{
   string numberString[] = {"zero ","one ","two ","three ","four "};
   cout << "Creating input files for " << numberString[periods] << "period LP-model" << endl;					// Transfer data to LP model
   if (periods>1)
   	BasicLPParameters();

   theLivestock->CalcLP(periods);

   theBuildings->CalcLP();
   theLegislation->CalcLP();
}
/**
 * Handle the daily updates for climate, indicate, operationsManager, Livestock, CropRotation, buildigns
 */
void eventControl::DailyUpdate()
{

   theClimate->Update();          //Load today's climate
	theOutput->DailyUpdate();
   theOperational_manager->daily_update();
   if (theLivestock)
      theLivestock->DailyUpdate();
   theCropRotation->DailyUpdate();
   theBuildings->DailyUpdate();
   theOperational_manager->GetStatus(); // NJH added this November 2006
#ifndef NITROSCAPE
   if((theTime.GetDay()==5) || (theTime.GetDay()==15) || (theTime.GetDay()==25))
   	cout << ".";
#endif
   //cout << endl;
/*     filehandle = theMessage->GiveDebugHandle();
      *filehandle << endl ;
    theMessage->CloseDebugHandle();
  */
}

/****************************************************************************\
\****************************************************************************/
void eventControl::GiveIndicator(indicatorTypes indicatorType, int yearNumber)
{
   if (indicatorType==economicIndicator)
   {
      theOutput->AddIndicator(economicIndicator,"05.70 Ha premium","Dkr",theCropRotation->GetTotalPremium()); // BMP added �
      theOutput->AddIndicator(economicIndicator,"03.01 Value of arable land","Dkr",theCropRotation->GetValueofLand()); // BMP added �
   	theOutput->AddIndicator(economicIndicator,"06.10 Plant production misc. costs","Dkr",-theCropRotation->GetDiversePlant()); // BMP �
	   if (theLivestock)
      {
      	double animalUnits = theLivestock->GetDE();
         theOutput->AddStateIndicator(economicIndicator,"01.02 DE (old)","no",animalUnits);
         if (theCropRotation->GetTotalArea()>0)
	         theOutput->AddStateIndicator(economicIndicator,"01.03 DE/ha (old)","no/ha",animalUnits/theCropRotation->GetTotalArea());
	      theLivestock->GiveIndicator(indicatorType);
      }
	      economics * theEconomics = new economics;
		theEconomics->SetInterest(CapitalInterest);
	   theEconomics->GiveIndicator();
	   delete theEconomics;

   }
   if (indicatorType==environmentalIndicator && theLivestock)
      theLivestock->FlushIndicator(indicatorType);
//   theOutput->AddStateIndicator(economicIndicator,"98.91 current year"," ",theTime.GetYear());
   theOutput->FlushIndicator(indicatorType,yearNumber);
}

/****************************************************************************\
\****************************************************************************/
void eventControl::Simulation(int runNumber, bsTime stopTime, int useGams, int NumberOfFixedYears, int GenerateFixedPlans,
                              char * inputDir, char * outputDir, char * climateDir,
                              char * climateFilename, string economicIndicatorDate, string environmentalIndicatorDate)
{
	
   int yearNumber = 0;
   bsTime economicIndicatorTime, environmentalIndicatorTime, yearStopTime;

   economicIndicatorTime.SetTime(economicIndicatorDate);               				// default 1/9
   environmentalIndicatorTime.SetTime(environmentalIndicatorDate);      				// default 1/9
   if (economicIndicatorTime<=theTime)     economicIndicatorTime.AddOneYear();                  // added by JB
   if (environmentalIndicatorTime<=theTime) environmentalIndicatorTime.AddOneYear();

   cout << "Input directory    " << inputDir << endl;
   cout << "Output directory   " << outputDir << endl;
   cout << "Climate directory  " << climateDir << endl;
   cout << "Climate file       " << climateFilename << endl;

   if (chdir(outputDir)!=0)
#ifndef __BCplusplus__
	#ifdef BUILDER
	  mkdir(outputDir);
	#endif
	#ifdef UNIX
	  mkdir(outputDir,'-p');
	#endif
#ifdef CYGWIN
	  mkdir(outputDir,'-p');
#endif
#ifdef __BCplusplus__
	  mkdir(outputDir);
	#endif                                     				// make output directory if not present
#else
//	#ifdef BUILDER
	  mkdir(outputDir);
/*	#else
	  cout << "Error - directory not found " << endl;
	  cout << errno << endl;
	  cout << "Press any key to exit " << endl;
	  char dum;
	  cin >> dum;
	  exit(99);
	#endif*/
#endif
   if (chdir(inputDir)!=0)
		theMessage->FatalError("Event:: input directory ",inputDir," not found!");
   Initialize(&stopTime,inputDir,climateDir,climateFilename,outputDir);

   if (!abort)
   {
      bool onceOnly=false;
      // Yearly outerloop begins
      while((theTime < stopTime)&& (!onceOnly))
      {
         cout << endl;
         if(runNumber<=9)
         	cout<<  " Run "<<"0"<< runNumber;
         else
         	cout<<  " Run "<< runNumber;
         if(yearNumber<=9)
         	cout<<  " Year "<<"0"<< yearNumber;
         else
         	cout<<  " Year "<< yearNumber;

         if (useGams && (yearNumber>=NumberOfFixedYears))
         {
/*            CalcLP(3);
			theManager->RunLP(3,outputDir);
			CalcLP(1);
			theManager->RunLP(1,outputDir);
			cout << endl << "Receiving production plans generated by LP-model" << endl;	// Initialize modules with results from LP model
			chdir("\\STRATEGI\\LP_MODEL");
			ReceivePlan("lp1",inputDir);
			RenameFiles(outputDir,inputDir,GenerateFixedPlans,yearNumber);
            chdir(inputDir);*/
         }
         else
         {
            // Initialize modules with results from fixed plan
            char fileExtension[5]; //CK
            fileExtension[0] = '\0';
            if (yearNumber<100)
               sprintf(fileExtension,"f%02d",yearNumber);
            else
               sprintf(fileExtension,"f%03d",yearNumber);
            cout << endl << "Receiving fixed production plans" << endl;
            ReceivePlan(fileExtension,inputDir);
         }

         cout << endl
              << "Simulating production ...................................."
              << endl << "Progress              ";

         // innerloop of production
         yearStopTime = theTime;
         yearStopTime.AddOneYear();
         while((theTime < yearStopTime)&& (!onceOnly))
         {
            onceOnly = false;    //enable if you want to pass through the daily loop only once per year
            // theProducts->SellPlantItems(); !!!???
            DailyUpdate();
            // if (!theLivestock) theProducts->SellPlantItems();  //sell plant products only if no animals present!! NJH April 2001
            theTime.AddOneDay();
            if (theTime==economicIndicatorTime)
            {
               theTechnics->YearlyCalc();
               GiveIndicator(economicIndicator,yearNumber);
               theTechnics->Reset();
               theLegislation->Update();                       // problems with date's???
            }
            if (theTime==environmentalIndicatorTime)
               GiveIndicator(environmentalIndicator,yearNumber);
         }

         // Reset before next year
         theProducts->resetProducts();
         theProducts->YearlyUpdate();
         yearNumber++;
         timeConsumption->outputtimeUse();
         economicIndicatorTime.AddOneYear();
         environmentalIndicatorTime.AddOneYear();
      }  // yearly outerloop ends
   }
/*
	EndSimulation();
   Above line creates memory problems sometimes - see soilProfile::GetMaximumDepth. Removed for now - BMP 11. November 2006 ??? !!!!
*/
}

/****************************************************************************\
\****************************************************************************/
void eventControl::EndSimulation(bool show)
{
   if (theLivestock)
      theLivestock->checkBalances(show);
   theBuildings->EndBudget(show);
   theCropRotation->EndBudget();
}




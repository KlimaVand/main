/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"

#include "eventControl.h"
#include "../base/climate.h"

#include "../base/settings.h"
#include "../base/controlParameters.h"
#include "../tools/fileAccess.h"
#ifndef __BCplusplus__
	#include <errno.h>
	#include <sys/stat.h>
	#include <sys/types.h>
#endif

/****************************************************************************\
  Default Constructor
\****************************************************************************/
eventControl::eventControl()
   : base()
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
   ScaleBuildings			  = 0;
   Nbudget.SetNames("farm","N"); // ???!!!
}

/****************************************************************************\
\****************************************************************************/
void eventControl::Initialize(bsTime * stopTime, char* inputDir, char * climateDir, char * climateFilename, char * outputPath)
{

   char filenameCSV[100];
   char filenameField[100];


#ifndef __BCplusplus__
   sprintf(filenameCSV,"%s/INDICATX.XLS",outputPath);
   sprintf(filenameField,"%s/Fielddata.txt",outputPath);   //@@@

#else
   sprintf(filenameCSV,"%s\\INDICATX.XLS",outputPath);
   sprintf(filenameField,"%s\\Fielddata.txt",outputPath);   //@@@

#endif
   theOutput->Initialize(filenameCSV);

   bool Irrigation = false;
   int daysBetweenIrrigation = 0;
   int irrigationDelay = 0;
   //inserted by NJH Oct 2007
   bool SelltheStrawProducts = 0;
   bool SelltheNonRoughageProducts = 0;
   bool WithoutFields=false;
   //
   bool pigfarm = false;
   bool dairyfarm = false;
   bool beeffarm = false;
   bool poultryfarm = false;
   int FingerFlow = 0;
   string FarmID = "";
   fileAccess hd;
   hd.changeDir(globalSettings->getInputDirectory());
   commonData data;
   data.setCritical(false);
   data.readFile("farm.dat");
   data.FindSection("farm");
   data.FindItem("Irrigation",&Irrigation);

   data.FindItem("daysBetweenIrrigation",&daysBetweenIrrigation);
   data.FindItem("irrigationDelay",&irrigationDelay);
   data.FindItem("FingerFlow",&FingerFlow);
   data.FindItem("ScaleBuildings",&ScaleBuildings);
   data.FindItem("SelltheStrawProducts",&SelltheStrawProducts);
   data.FindItem("SelltheNonRoughageProducts",&SelltheNonRoughageProducts);
   data.FindItem("WithoutFields",&WithoutFields);


      //
#ifdef TUPLE
   FarmID=theFarmData->FarmDatas.FarmID;
   theControlParameters->SetFarmID(FarmID);
#endif

   data.FindSection("livestock");
   data.FindItem("pigfarm",&pigfarm);
   data.FindItem("dairyfarm",&dairyfarm);
   data.FindItem("beeffarm",&beeffarm);
   data.FindItem("poultryfarm",&poultryfarm);

   if (pigfarm ||dairyfarm || beeffarm || poultryfarm)
      theLivestock = new livestock;

   theProducts->InitializeProducts("RunProducts.dat", "StandardProducts.dat");//,(dairyfarm || beeffarm));
   theProducts->InitializePrices(stopTime);
   theProducts->InitializeFeed("FeedTable2004.dat");
   theClimate->InitClimate(climateDir,climateFilename);
   //change back to input directory, just in case the climate directory was elsewhere

   hd.changeDir(inputDir);
   theCropRotation->Initialize(WithoutFields,Irrigation,daysBetweenIrrigation,irrigationDelay, FingerFlow);
   theProducts->UpdateProductPtr();
   theTechnics->Initialize("operatio.dat","techeqp.dat");
   theBuildings->InitializeAll("RunBuildings.dat", pigfarm, dairyfarm, beeffarm);
   if (theLivestock)
      theLivestock->Initialize("livestock.dat",pigfarm,dairyfarm,beeffarm,theBuildings);

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
   	{
	   cout << "."<<flush;
   	}
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

      theOutput->AddIndicator(301,"03.01 Value of arable land","Dkr",theCropRotation->GetValueofLand()); // BMP added �
      double DiversePlant=-theCropRotation->GetDiversePlant();
   	theOutput->AddIndicator(610,"06.10 Plant production misc. costs","Dkr",DiversePlant); // BMP �
	   if (theLivestock)
      {
      	double animalUnits = theLivestock->GetDE();
         theOutput->AddStateIndicator(102,"01.02 DE (old)","no",animalUnits);
         if (theCropRotation->GetTotalArea()>0)
	         theOutput->AddStateIndicator(103,"01.03 DE/ha (old)","no/ha",animalUnits/theCropRotation->GetTotalArea());
	      theLivestock->GiveIndicator(indicatorType);
      }
	      economics * theEconomics = new economics;
	   theEconomics->GiveIndicator();
	   delete theEconomics;

   }
   if (indicatorType==environmentalIndicator && theLivestock)
      theLivestock->FlushIndicator(indicatorType);
//   theOutput->AddStateIndicator("98.91 current year"," ",theTime.GetYear());
   theOutput->FlushIndicator(indicatorType,yearNumber);
}

/****************************************************************************\
\****************************************************************************/
void eventControl::Simulation(int runNumber, bsTime stopTime,
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


            // Initialize modules with results from fixed plan
            char fileExtension[4];
            fileExtension[0] = '\0';
            if (yearNumber<100)
               sprintf(fileExtension,"f%02d",yearNumber);
            else
               sprintf(fileExtension,"f%03d",yearNumber);
            cout << endl << "Receiving fixed production plans" << endl;
            ReceivePlan(fileExtension,inputDir);


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

            }
            if (theTime==environmentalIndicatorTime)
               GiveIndicator(environmentalIndicator,yearNumber);
         }

         // Reset before next year
         theProducts->resetProducts();
         theProducts->YearlyUpdate();
         yearNumber++;

         economicIndicatorTime.AddOneYear();
         environmentalIndicatorTime.AddOneYear();

   }
/*
	EndSimulation();
   Above line creates memory problems sometimes - see soilProfile::GetMaximumDepth. Removed for now - BMP 11. November 2006 ??? !!!!
*/
}





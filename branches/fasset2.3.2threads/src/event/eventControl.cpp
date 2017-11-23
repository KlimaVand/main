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
void eventControl::print()
{
	systemData.print();
}
eventControl::eventControl(const eventControl& aEventControl)
{
	systemData=aEventControl.systemData;
		run=aEventControl.run;
		removeOldData=aEventControl.removeOldData;
	InputDirectory1=aEventControl.InputDirectory1;
	OutputDirectory1=aEventControl.OutputDirectory1;
	ClimateDirectory=aEventControl.ClimateDirectory;
	ClimateFileName=aEventControl.ClimateFileName;

	if(aEventControl.theLivestock)
   	theLivestock=new livestock(*aEventControl.theLivestock);
	else
		theLivestock=NULL;
   	EconomicIndicatorDate=aEventControl.EconomicIndicatorDate;

   	EnvironmentalIndicatorDate=aEventControl.EnvironmentalIndicatorDate;
   	if(aEventControl.theCropRotation)
   		theCropRotation=new cropRotation();
   	else
   		theCropRotation=NULL;

   	if(aEventControl.theOperational_manager)
   		theOperational_manager=new operationsManager(*aEventControl.theOperational_manager);
   	else
   	   	theOperational_manager=NULL;

	 ScaleBuildings=aEventControl.ScaleBuildings;
	 stopTime=aEventControl.stopTime;
	Nbudget=aEventControl.Nbudget;

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
   if(cleanUpCalled==true)
   delete theMessage[id];
}

/**
 * Only called by constructor, to avoid redundant code
 */
void eventControl::InitVariables()
{
	EnvironmentalIndicatorDate="1/1";
	EconomicIndicatorDate="1/1";
   theLivestock           = NULL;
   theCropRotation        = new cropRotation;
	theOperational_manager = new operationsManager() ; // added by NJH
   ScaleBuildings			  = 0;
   Nbudget.SetNames("farm","N"); // ???!!!
   cleanUpCalled=false;
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
   theOutput[id]->Initialize(filenameCSV);

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
   hd.changeDir(globalSettings[id]->getInputDirectory());
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
   FarmID=theFarmdata.FarmDatas.FarmID;
   theControlParameters[id]->SetFarmID(FarmID);
#endif

   data.FindSection("livestock");
   data.FindItem("pigfarm",&pigfarm);
   data.FindItem("dairyfarm",&dairyfarm);
   data.FindItem("beeffarm",&beeffarm);
   data.FindItem("poultryfarm",&poultryfarm);

   if (pigfarm ||dairyfarm || beeffarm || poultryfarm)
      theLivestock = new livestock;

   theProducts[id]->InitializeProducts("RunProducts.dat", "StandardProducts.dat");//,(dairyfarm || beeffarm));
   theProducts[id]->InitializePrices(stopTime);
   theProducts[id]->InitializeFeed("FeedTable2004.dat");
   theClimate[id]->InitClimate(climateDir,climateFilename);
   //change back to input directory, just in case the climate directory was elsewhere

   hd.changeDir(inputDir);

   theCropRotation=new cropRotation();

   theCropRotation->Initialize(WithoutFields,Irrigation,daysBetweenIrrigation,irrigationDelay, FingerFlow);
   theProducts[id]->UpdateProductPtr();
   theTechnics[id]->Initialize("operatio.dat","techeqp.dat");
   theBuildings[id]->InitializeAll("RunBuildings.dat", pigfarm, dairyfarm, beeffarm);
   if (theLivestock)
      theLivestock->Initialize("livestock.dat",pigfarm,dairyfarm,beeffarm,theBuildings[id]);

   theOperational_manager->Initialize(theLivestock,theCropRotation,theBuildings[id]);    //added by NJH 1.5.00

   //inserted by NJH Oct 2007
   theProducts[id]->SetSelltheStrawProducts(SelltheStrawProducts);
   theProducts[id]->SetSelltheNonRoughageProducts(SelltheNonRoughageProducts);
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
         theBuildings[id]->ScaleBuildings();
}




/**
 * Handle the daily updates for climate, indicate, operationsManager, Livestock, CropRotation, buildigns
 */
void eventControl::DailyUpdate()
{


   theClimate[id]->Update();          //Load today's climate
	theOutput[id]->DailyUpdate();

   theOperational_manager->daily_update();
   if (theLivestock)
      theLivestock->DailyUpdate();
   theCropRotation->DailyUpdate();
   theBuildings[id]->DailyUpdate();
   theOperational_manager->GetStatus(); // NJH added this November 2006
#ifndef NITROSCAPE
   if((theTime[id].GetDay()==5) || (theTime[id].GetDay()==15) || (theTime[id].GetDay()==25))
   	{
	   cout << "."<<flush;
   	}
#endif
   //cout << endl;
/*     filehandle = theMessage[id]->GiveDebugHandle();
      *filehandle << endl ;
    theMessage[id]->CloseDebugHandle();
  */
}

/****************************************************************************\
\****************************************************************************/
void eventControl::GiveIndicator(indicatorTypes indicatorType, int yearNumber)
{
   if (indicatorType==economicIndicator)
   {

      theOutput[id]->AddIndicator(301,"03.01 Value of arable land","Dkr",theCropRotation->GetValueofLand()); // BMP added �
      double DiversePlant=-theCropRotation->GetDiversePlant();
   	theOutput[id]->AddIndicator(610,"06.10 Plant production misc. costs","Dkr",DiversePlant); // BMP �
	   if (theLivestock)
      {
      	double animalUnits = theLivestock->GetDE();
         theOutput[id]->AddStateIndicator(102,"01.02 DE (old)","no",animalUnits);
         if (theCropRotation->GetTotalArea()>0)
	         theOutput[id]->AddStateIndicator(103,"01.03 DE/ha (old)","no/ha",animalUnits/theCropRotation->GetTotalArea());
	      theLivestock->GiveIndicator(indicatorType);
      }
	      economics * theEconomics = new economics;
	   theEconomics->GiveIndicator();
	   delete theEconomics;

   }
   if (indicatorType==environmentalIndicator && theLivestock)
      theLivestock->FlushIndicator(indicatorType);
//   theOutput[id]->AddStateIndicator("98.91 current year"," ",theTime[id].GetYear());
   theOutput[id]->FlushIndicator(indicatorType,yearNumber);
}

/****************************************************************************\
\****************************************************************************/
void eventControl::operator()()
           	{

Start();
           	}
void eventControl::Start()
{







	systemData.setOutputWarning(false);
	threadID->setID(std::this_thread::get_id());
	id=threadID->getID(std::this_thread::get_id());
	cout<<id<<" is id "<<run<<" is run"<<endl;
int StartYear=0;
int StopYear=0;
int day=1;
int month=8;
int ClimateFileType = -1;
int EpotCalculationMethod = 0;

int VolatModel=1;
int singleField=-1;
int fertiliserUnit=0;
//  bool SellPlantProducts=false;
bool
   UseReductionParameters=false, 		 // Default determined by theControlParameters
   MeasuredSoilTemperature=false,       // Default determined by theControlParameters
   AirToSoilTemperature=false,       // Default determined by theControlParameters
   WriteSoilTemperature=false,       // Default determined by theControlParameters
   SimpleTemperatureModel=false,        // Default determined by theControlParameters
   // WriteDetails,                  // Default determined by theControlParameters
   GrassWeed=false,                     // Default determined by theControlParameters
   Organic=false, 					    	 // Default determined by theControlParameters
   ForcedOperations=false,              // Default determined by theControlParameters
   ForcedHarvest=false,
   fieldsFlexSize=false,                // When true, fields are allowed to shrimp or expand
   uniSizeFields=false;                 // When true, all fields are 1 ha big

	systemData.setCritical(true);
	systemData.print();

	            systemData.FindSection("run",run);
	            systemData.FindItem("StartYear",&StartYear);

	            systemData.FindItem("StopYear",&StopYear);

	            systemData.FindItem("InputDirectory",&InputDirectory1 );
	            cout<<InputDirectory1<<endl;
	            systemData.FindItem("OutputDirectory",&OutputDirectory1 );
				#ifndef UNIX
				if(OutputDirectory1.substr(OutputDirectory1.size()-1,1).compare("\\"))
					OutputDirectory1=OutputDirectory1+"\\";
#else
				if(OutputDirectory1.substr(OutputDirectory1.size()-1,1).compare("/"))
					OutputDirectory1=OutputDirectory1+"/";
#endif
#ifndef UNIX
if(InputDirectory1.substr(InputDirectory1.size()-1,1).compare("\\"))
	InputDirectory1=InputDirectory1+"\\";
#else
if(OutputDirectory1.substr(OutputDirectory1.size()-1,1).compare("/"))
	OutputDirectory1=OutputDirectory1+"/";
#endif
	            globalSettings[id]=new settings();
	            globalSettings[id]->setInputDir(InputDirectory1);
	            globalSettings[id]->setOutputDir(OutputDirectory1);
	            globalSettings[id]->run=run;

	            if(removeOldData==true)
	            	   {
	            		   fileAccess remove;
	            		   string path=globalSettings[id]->getOutputDirectory();
	            		   remove.deleteEveryFileInFolder(path);
	            	   }


	            theControlParameters[id] = new controlParameters;

	            systemData.FindItem("ClimateFileName",&ClimateFileName );
	            systemData.setCritical(false);
	            if(!systemData.FindItem("ClimateDirectory",&ClimateDirectory ))
	            {
	            	string instring = ClimateFileName;
	            				//cout<<instring<<endl;
	            	#ifndef UNIX
	            				int loc = instring.rfind("\\",instring.length());          //separate directory and file names
	            	#endif
	            	#ifdef UNIX
	            				int loc = instring.rfind("/",instring.length());          //separate directory and file names
	            	#endif
	            				if (loc>0)
	            				{
	            					ClimateDirectory=instring.substr(0,loc);

	            					ClimateFileName=instring.substr(loc+1);
	            				}
	            				else
	            				{

	            					cout<<instring<<endl;
	            					exit(99);
	            				}

	            }
	            if(!systemData.FindItem("ClimateFileType",&ClimateFileType ))
	            {
	            	int size=ClimateFileName.size();

	            				if(ClimateFileName.substr(size-3, size).compare("CLM"))
	            					ClimateFileType=0;
	            				else if(ClimateFileName.substr(size-3, size).compare("clm"))
	            					ClimateFileType=0;
	            				else if(ClimateFileName.substr(size-3, size).compare("KML"))
	            					ClimateFileType=1;
	            				else if(ClimateFileName.substr(size-3, size).compare("kml"))
	            					ClimateFileType=1;

	            }



	            systemData.FindItem("EnvironmentalIndicator",&EnvironmentalIndicatorDate);

				systemData.FindItem("EpotCalculationMethod",&EpotCalculationMethod);

				systemData.FindItem("EconomicIndicatorDate",&EconomicIndicatorDate);

				systemData.FindItem("VolatModel",&VolatModel);


	            theControlParameters[id]->SetvolatModel(VolatModel);

	            if(systemData.FindItem("SimpleTemperatureModel",&SimpleTemperatureModel)==true)
	            {

	            	theControlParameters[id]->SetSimpleTemperatureModel(SimpleTemperatureModel);
	            }
	            if(systemData.FindItem("AirToSoilTemperature",&AirToSoilTemperature)==true)
	            {

	            	theControlParameters[id]->SetAirToSoilTemperature(AirToSoilTemperature);
	            }
	            if(systemData.FindItem("uniSizeFields",&uniSizeFields)==true)
	            {

	            	theControlParameters[id]->SetFieldsUniSize(uniSizeFields);
	            }
	            if(systemData.FindItem("fieldsFlexSize",&fieldsFlexSize)==true)
	            {

	            	theControlParameters[id]->SetFieldsFlexSize(fieldsFlexSize);
	            }

	            if(systemData.FindItem("SingleField",&singleField)==true)
	            {
	               theControlParameters[id]->SetSingleField(singleField);
	               globalSettings[id]->DetailsData.PlantDetails=true;
	               globalSettings[id]->DetailsData.SoilDetails=true;
	            }
	            if(systemData.FindItem("fertiliserUnit",&fertiliserUnit)==true)
	            {

					theControlParameters[id]->SetFertiliserUnit(fertiliserUnit);
	            }
	            if(systemData.FindItem("MeasuredSoilTemperature",&MeasuredSoilTemperature)==true)
	           	{

	            	theControlParameters[id]->SetMeasuredSoilTemperature(MeasuredSoilTemperature);

	           	}


	            if(systemData.FindItem("WriteSoilTemperature",&WriteSoilTemperature)==true)
	            {

						theControlParameters[id]->SetWriteSoilTemperature(WriteSoilTemperature);
	           	}
	            if(systemData.FindItem("ForcedOperations",&ForcedOperations)==true)
	            {

						theControlParameters[id]->SetForcedOperations(ForcedOperations);
	            }


	            systemData.FindItem("ForcedOperations",&ForcedHarvest);
	            theControlParameters[id]->SetForcedHarvest(ForcedHarvest);
	            double HarvestWaste=0.05;

	            systemData.FindItem("HarvestWaste",&HarvestWaste);
	            if(systemData.FindItem("HarvestWaste",&HarvestWaste)==true)
	            {
	             theControlParameters[id]->SetHarvestWaste(HarvestWaste); // 0.05 - 0.10 may be a sensible range
	            }


	            double StrawLeft= 0.45;

	            systemData.FindItem("StrawLeft",&StrawLeft);

	            theControlParameters[id]->SetStrawLeft(StrawLeft);
	            if (systemData.FindItem("ForcedOperations",&ForcedOperations)==true)
	            	theControlParameters[id]->SetUseReductionParameters(UseReductionParameters);
	            if (systemData.FindItem("ForcedOperations",&ForcedOperations)==true)
	            	theControlParameters[id]->SetGrassWeed(GrassWeed);
	            if (systemData.FindItem("ForcedOperations",&ForcedOperations)==true)
	             	theControlParameters[id]->SetOrganicFarm(Organic);

	               theTime[id].SetTime(day,month,StartYear);


	               cout << "Simulation starts: " << theTime << endl;

	               stopTime.SetTime(day,month,StopYear);       			// JB added this
	               cout << "Simulation stops:  " << stopTime << endl;

	               char FN1[200],FN2[200], FN3[200];
	//               chdir(OutputDirectory);
	               strcpy(FN1,OutputDirectory1.c_str());
	               strcat(FN1,"warnings.txt");
	               strcpy(FN2,OutputDirectory1.c_str());
	               strcat(FN2,"logfile.txt");
	               strcpy(FN3,OutputDirectory1.c_str());
	               strcat(FN3,"debug.txt");      //debug file name
               char string200[200];
                strcpy(string200, OutputDirectory1.c_str());


	               theProducts[id]                  = new products;
	               theOutput[id]                    = new IndicatorOutput;

	               theMessage[id]                   = new message;


	               theTechnics[id]                  = new technics("technics",0); // BMP
	               theClimate[id]                   = new climate(ClimateFileType,EpotCalculationMethod); // BMP
	               theBuildings[id]        	        = new buildings;
	               theManure_manager[id]            = new manureManager();


	               theMessage[id]->InitMessage(FN1,FN2,FN3);
	               globalSettings[id]->readParameter();
	               cout<<"Starting Simulation" <<endl;






	Simulation(globalSettings[id]->run, stopTime,(char*)InputDirectory1.c_str(),(char*)OutputDirectory1.c_str(),(char*)ClimateDirectory.c_str(),
			                           (char*)ClimateFileName.c_str(), (char*) EconomicIndicatorDate.c_str(), (char*) EnvironmentalIndicatorDate.c_str());

}

void eventControl::CleanUp()
{
	if(theProducts[id])
	 delete theProducts[id]; //if gcc tries to free this part of memory it failes. It looks like it have already been free but this is the only part where it is?????
	if(theOutput[id])
			               delete theOutput[id];
	if(theTechnics[id])
			               delete theTechnics[id];
	if(theClimate[id])
			               delete theClimate[id];
	if(theManure_manager[id])
			               delete theManure_manager[id];
	if(theControlParameters[id])
			               delete theControlParameters[id];
	if(globalSettings[id])
						   delete globalSettings[id];
	cleanUpCalled=true;

}
void eventControl::Settup(commonData data,int index, bool remove)
{ //base aBase;


	systemData=data;
	systemData.print();
	run=index;
	removeOldData=remove;
	//      ScenarioFileName,


}
void eventControl::Simulation(int runNumber, bsTime stopTime,
                              char * inputDir, char * outputDir, char * climateDir,
                              char * climateFilename, string economicIndicatorDate, string environmentalIndicatorDate)
{

   int yearNumber = 0;
  bsTime economicIndicatorTime, environmentalIndicatorTime, yearStopTime;

   economicIndicatorTime.SetTime(economicIndicatorDate);               				// default 1/9
   environmentalIndicatorTime.SetTime(environmentalIndicatorDate);      				// default 1/9
   if (economicIndicatorTime<=theTime[id])     economicIndicatorTime.AddOneYear();                  // added by JB
   if (environmentalIndicatorTime<=theTime[id]) environmentalIndicatorTime.AddOneYear();

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
		theMessage[id]->FatalError("Event:: input directory ",inputDir," not found!");
   Initialize(&stopTime,inputDir,climateDir,climateFilename,outputDir);


      bool onceOnly=false;
      // Yearly outerloop begins
      while((theTime[id] < stopTime)&& (!onceOnly))
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
         yearStopTime = theTime[id];
         yearStopTime.AddOneYear();
         while((theTime[id]< yearStopTime)&& (!onceOnly))
         {
            onceOnly = false;    //enable if you want to pass through the daily loop only once per year
            // theProducts[id]->SellPlantItems(); !!!???
            DailyUpdate();
            // if (!theLivestock) theProducts[id]->SellPlantItems();  //sell plant products only if no animals present!! NJH April 2001
            theTime[id].AddOneDay();
            if (theTime[id]==economicIndicatorTime)
            {
               theTechnics[id]->YearlyCalc();
               GiveIndicator(economicIndicator,yearNumber);
               theTechnics[id]->Reset();

            }
            if (theTime[id]==environmentalIndicatorTime)
               GiveIndicator(environmentalIndicator,yearNumber);
         }

         // Reset before next year
         theProducts[id]->resetProducts();
         theProducts[id]->YearlyUpdate();
         yearNumber++;

         economicIndicatorTime.AddOneYear();
         environmentalIndicatorTime.AddOneYear();

   }
/*
	EndSimulation();
   Above line creates memory problems sometimes - see soilProfile::GetMaximumDepth. Removed for now - BMP 11. November 2006 ??? !!!!
*/
}





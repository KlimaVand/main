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
#endif
#ifndef __BCplusplus__

#include "data/systemData.h"
#include "data/outputData.h"
#include "base/common.h"
#include "base/bstime.h"
#include "products/products.h"
#include "base/message.h"
#include "legislation/legislation.h"
#include "building/thebuild.h"
#include "base/climate.h"
#include "event/eventControl.h"
#include "base/timeUse.h"
#include "base/controlParameters.h"
#include "technics/technics.h"
#include "manager/manureManager.h"
#include "tools/fileAccess.h"
#include "data/farmData.h"
#include "data/environData.h"
#include "data/outputData.h"
#include "data/commonData.h"
#include "data/systemData.h"
#include "tools/compare.h"
#endif
using namespace std;
#ifndef NITROSCAPE
bsTime theTime;
products * theProducts;
message * theMessage;
timeUse* timeConsumption;
IndicatorOutput * theOutput;
systemData * theSystemData;
legislate * theLegislation;
technics * theTechnics;
buildings * theBuildings;
climate * theClimate;
controlParameters * theControlParameters;
manureManager *  theManure_manager;
outputData * theOutputData;
farmData * theFarmData;
environData * theEnvironData;
//static fstream * fs; //scenario log file @@@


/****************************************************************************\
\****************************************************************************/
void Display()
{
   int NW=theMessage->NumOfWarnings();
   if (NW>0)
   {
      cout << endl << endl << "The simulation has resulted in " << NW; // BMP �
      if (NW>1)
         cout << " warnings.";
      else
         cout << " warning.";
      cout << " See 'WARNINGS.TXT'";
   }
   else
      cout << endl << endl << "Simulation completed.";
   cout << endl << endl;
}

/****************************************************************************\
\****************************************************************************/

int modified()
{


	   //base aBase;
	   bsTime stopTime;

	   std::string   InputDirectory1;
	   std::string   OutputDirectory1;
	   std::string   ClimateDirectory;
	   std::string    ClimateFileName;
	//      ScenarioFileName,
   	std::string   baseOutputDir;
	   std::string EconomicIndicatorDate="1/1";
	   std::string EnvironmentalIndicatorDate="1/1";
	   std::string WaterFlowMethod="";

	   int StartYear=0;
	   int StopYear=0;
	   int day=1;
	   int month=8;
	   int ClimateFileType = -1;
	   int EpotCalculationMethod = 0;

	   int VolatModel=0;
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

	   int index;
	   cout << "Farm ASSEsment Tool v2.3 alpha" << endl << endl;




	   // Used to read info that is common  for all runs in system.dat. Can be overwritten later in the individual run definitions
	   theSystemData =new systemData(true);

	   int first=theSystemData->getJumpStart();
	   int num=theSystemData->getJumpStop();

	  	for(index=first;index<=num;index++)
	   {
	  		theSystemData->setCurrentRun(index);

	  		theOutputData = new outputData();
	  		theFarmData = new farmData();
	  		theEnvironData = new environData();;
	  		if(theSystemData->getremoveOldData()==true)
	        	 {
	        		 fileAccess remove;
	        		 string path=theSystemData->getOutputDirectory();
	        		 remove.deleteEveryFileInFolder(path);
	        	 }
	            theControlParameters = new controlParameters;

	            StartYear=theSystemData->getStartYear();
	            StopYear=theSystemData->getStopYear();
	            InputDirectory1=theSystemData->getInputDirectory();
	            OutputDirectory1=theSystemData->getOutputDirectory();
	            ClimateDirectory=theSystemData->getClimateDirectory();
				EnvironmentalIndicatorDate=theSystemData->getEnvironmentalIndicator();// aBase.GetParameter("EnvironmentalIndicator",EnvironmentalIndicatorDate);

				ClimateFileType =theSystemData->getClimateFileType();

				EpotCalculationMethod=theSystemData->getEpotCalculationMethod();

	            EconomicIndicatorDate=theSystemData->getEconomicIndicatorDate();

	            VolatModel=theSystemData->getVolatModel();
               ClimateFileName= theSystemData->getClimateFileName();
	            theControlParameters->SetvolatModel(VolatModel);

	            if(theSystemData->getSimpleTemperatureModelFile()==true)
	            {
	            	SimpleTemperatureModel=theSystemData->getSimpleTemperatureModel();
	            	theControlParameters->SetSimpleTemperatureModel(SimpleTemperatureModel);
	            }
	            if(theSystemData->getAirToSoilTemperatureFile()==true)
	            {
	            	AirToSoilTemperature=theSystemData->getAirToSoilTemperature();
	            	theControlParameters->SetAirToSoilTemperature(AirToSoilTemperature);
	            }
	            if(theSystemData->getuniSizeFieldsFile()==true)
	            {
	            	uniSizeFields=theSystemData->getuniSizeFields();
	            	theControlParameters->SetFieldsUniSize(uniSizeFields);
	            }
	            if(theSystemData->getuniSizeFieldsFile()==true)
	            {
	            	fieldsFlexSize=theSystemData->getFieldsFlexSize();
	            	theControlParameters->SetFieldsFlexSize(fieldsFlexSize);
	            }
	            singleField=theSystemData->getsingleField();
	            if(theSystemData->getsingleFieldFile()==true)
	            {
	               theControlParameters->SetSingleField(singleField);
	               theOutputData->DetailsData.setPlantDetails(true);
	               theOutputData->DetailsData.setSoilDetails(true);
	            }
	            if(theSystemData->getfertiliserUnitFile()==true)
	            {
	            	fertiliserUnit=theSystemData->getfertiliserUnit();
					theControlParameters->SetFertiliserUnit(fertiliserUnit);
	            }
	            if(theSystemData->getMeasuredSoilTemperatureFile()==true)
	           	{
	            	MeasuredSoilTemperature=theSystemData->getMeasuredSoilTemperature();
	            	theControlParameters->SetMeasuredSoilTemperature(MeasuredSoilTemperature);

	           	}


	            if(theSystemData->getMeasuredSoilTemperatureFile()==true)
	            {
						WriteSoilTemperature=theSystemData->getWriteSoilTemperature();
						theControlParameters->SetWriteSoilTemperature(WriteSoilTemperature);
	           	}
	            if (theSystemData->getForcedOperationsFile()==true)
	            {
	            	ForcedOperations=theSystemData->getForcedOperations();
						theControlParameters->SetForcedOperations(ForcedOperations);
	            }

	            ForcedHarvest =theSystemData->getForcedHarvest();
	            theControlParameters->SetForcedHarvest(ForcedHarvest);
	            double HarvestWaste=0.05;
	            HarvestWaste=theSystemData->getHarvestWaste();
	            if(theSystemData->getHarvestWasteFile()==true)
	            {
	             theControlParameters->SetHarvestWaste(HarvestWaste); // 0.05 - 0.10 may be a sensible range
	            }


	            double StrawLeft= theSystemData->getStrawLeft();
	            theControlParameters->SetStrawLeft(StrawLeft);
	            if (theSystemData->getForcedOperationsFile()==true)
	            	theControlParameters->SetUseReductionParameters(UseReductionParameters);
	            if (theSystemData->getForcedOperationsFile()==true)
	            	theControlParameters->SetGrassWeed(GrassWeed);
	            if (theSystemData->getForcedOperationsFile()==true)
	             	theControlParameters->SetOrganicFarm(Organic);
	            baseOutputDir=baseOutputDir+OutputDirectory1;



	               theTime.SetTime(day,month,StartYear);


	               cout << "Simulation starts: " << theTime << endl;

	               stopTime.SetTime(day,month,StopYear);       			// JB added this
	               cout << "Simulation stops:  " << stopTime << endl;

	               char FN1[200],FN2[200], FN3[200], FN4[200], FN5[200];
	//               chdir(OutputDirectory);
	               strcpy(FN1,OutputDirectory1.c_str());
	               strcat(FN1,"warnings.txt");
	               strcpy(FN2,OutputDirectory1.c_str());
	               strcat(FN2,"logfile.txt");
	               strcpy(FN3,OutputDirectory1.c_str());
	               strcat(FN3,"debug.txt");      //debug file name
	               strcpy(FN4,OutputDirectory1.c_str());
	               strcat(FN4,"fieldfile1.txt");
	               strcpy(FN5,OutputDirectory1.c_str());
	               strcat(FN5,"fieldfile2.txt");

                  char string200[200];
                   strcpy(string200, OutputDirectory1.c_str());


	               theProducts                  = new products;
	               theOutput                    = new IndicatorOutput;
	               theMessage                   = new message;
	               timeConsumption              = new timeUse;
	               theLegislation               = new legislate;
	               theTechnics                  = new technics("technics",0); // BMP
	               theClimate                   = new climate(ClimateFileType,EpotCalculationMethod); // BMP
	               theBuildings        	        = new buildings;
	               theManure_manager            = new manureManager();
	               eventControl * aEventControl = new eventControl;

	               theMessage->InitMessage(FN1,FN2,FN3,FN4,FN5);
	               cout<<"Starting Simulation" <<endl;

	               aEventControl->Simulation(index, stopTime,(char*)InputDirectory1.c_str(),(char*)OutputDirectory1.c_str(),(char*)ClimateDirectory.c_str(),
	                           (char*)ClimateFileName.c_str(), (char*) EconomicIndicatorDate.c_str(), (char*) EnvironmentalIndicatorDate.c_str());

	               Display();

	               delete theProducts; //if gcc tries to free this part of memory it failes. It looks like it have already been free but this is the only part where it is?????

	               delete theOutput;
	               delete timeConsumption;
	               delete theMessage;
	               delete theLegislation;
	               delete theTechnics;
	               delete theClimate;
	               delete theManure_manager;
	               delete aEventControl;
	               delete theOutputData;
	               delete theFarmData;
	               delete theEnvironData;

	            delete theControlParameters;
	         }
	   return 0;
}


int main(int argc,char *argv[])  //NJH, 28.2.07
{

	modified();
	return true;
}
#endif


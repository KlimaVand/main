/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/


#ifdef __BCplusplus__
#include "../base/settings.h"

#include "../base/common.h"
#include "../base/bstime.h"
#include "../products/products.h"
#include "../base/message.h"

#include "../building/thebuild.h"
#include "../base/climate.h"
#include "../event/eventControl.h"
#include "../base/controlParameters.h"
#include "../technics/technics.h"
#include "../manager/manureManager.h"
#include "../tools/fileAccess.h"

#include "../base/commonData.h"

#endif
#ifndef __BCplusplus__



#include "base/common.h"
#include "base/bstime.h"
#include "products/products.h"
#include "base/message.h"

#include "building/thebuild.h"
#include "base/climate.h"
#include "event/eventControl.h"

#include "base/controlParameters.h"
#include "technics/technics.h"
#include "manager/manureManager.h"
#include "tools/fileAccess.h"


#include "base/commonData.h"


#include "tools/compare.h"
#endif
#include <time.h>
using namespace std;
#ifndef NITROSCAPE
bsTime theTime;
products * theProducts;
message * theMessage;

IndicatorOutput * theOutput;

technics * theTechnics;
buildings * theBuildings;
climate * theClimate;
controlParameters * theControlParameters;
manureManager *  theManure_manager;

settings * globalSettings;
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

	   std::string EconomicIndicatorDate="1/1";
	   std::string EnvironmentalIndicatorDate="1/1";

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

	   int index;
	   cout << "Farm ASSEsment Tool v2.3.2" << endl << endl;




	   // Used to read info that is common  for all runs in system.dat. Can be overwritten later in the individual run definitions

	   commonData * data=new commonData();
	   data->setCritical(true);
	   data->readFile("system.dat");
	   int firstScenarie=0;
	   int num=0;
data->setCritical(false);
	   data->FindItem("JumpStart",&firstScenarie);

	   data->FindItem("JumpStop",&num);
	   
	   bool remove=false;
	   data->FindItem("removeOldData",&remove);

	  	for(index=firstScenarie;index<=num;index++)
	   {



	            data->setCritical(true);
	            data->FindSection("run",index);
	            data->FindItem("StartYear",&StartYear);

	            data->FindItem("StopYear",&StopYear);

	            data->FindItem("InputDirectory",&InputDirectory1 );

	            data->FindItem("OutputDirectory",&OutputDirectory1 );
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
	            globalSettings=new settings();
	            globalSettings->setInputDir(InputDirectory1);
	            globalSettings->setOutputDir(OutputDirectory1);
	            globalSettings->run=index;

	            if(remove==true)
	            	   {
	            		   fileAccess remove;
	            		   string path=globalSettings->getOutputDirectory();
	            		   remove.deleteEveryFileInFolder(path);
	            	   }


	            theControlParameters = new controlParameters;

	            data->FindItem("ClimateFileName",&ClimateFileName );
	            data->setCritical(false);
	            if(!data->FindItem("ClimateDirectory",&ClimateDirectory ))
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
	            if(!data->FindItem("ClimateFileType",&ClimateFileType ))
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



				data->FindItem("EnvironmentalIndicator",&EnvironmentalIndicatorDate);

				data->FindItem("EpotCalculationMethod",&EpotCalculationMethod);

				data->FindItem("EconomicIndicatorDate",&EconomicIndicatorDate);

				data->FindItem("VolatModel",&VolatModel);


	            theControlParameters->SetvolatModel(VolatModel);

	            if(data->FindItem("SimpleTemperatureModel",&SimpleTemperatureModel)==true)
	            {

	            	theControlParameters->SetSimpleTemperatureModel(SimpleTemperatureModel);
	            }
	            if(data->FindItem("AirToSoilTemperature",&AirToSoilTemperature)==true)
	            {

	            	theControlParameters->SetAirToSoilTemperature(AirToSoilTemperature);
	            }
	            if(data->FindItem("uniSizeFields",&uniSizeFields)==true)
	            {

	            	theControlParameters->SetFieldsUniSize(uniSizeFields);
	            }
	            if(data->FindItem("fieldsFlexSize",&fieldsFlexSize)==true)
	            {

	            	theControlParameters->SetFieldsFlexSize(fieldsFlexSize);
	            }

	            if(data->FindItem("SingleField",&singleField)==true)
	            {
	               theControlParameters->SetSingleField(singleField);
	               globalSettings->DetailsData.PlantDetails=true;
	               globalSettings->DetailsData.SoilDetails=true;
	            }
	            if(data->FindItem("fertiliserUnit",&fertiliserUnit)==true)
	            {

					theControlParameters->SetFertiliserUnit(fertiliserUnit);
	            }
	            if(data->FindItem("MeasuredSoilTemperature",&MeasuredSoilTemperature)==true)
	           	{

	            	theControlParameters->SetMeasuredSoilTemperature(MeasuredSoilTemperature);

	           	}


	            if(data->FindItem("WriteSoilTemperature",&WriteSoilTemperature)==true)
	            {

						theControlParameters->SetWriteSoilTemperature(WriteSoilTemperature);
	           	}
	            if(data->FindItem("ForcedOperations",&ForcedOperations)==true)
	            {

						theControlParameters->SetForcedOperations(ForcedOperations);
	            }


	            data->FindItem("ForcedOperations",&ForcedHarvest);
	            theControlParameters->SetForcedHarvest(ForcedHarvest);
	            double HarvestWaste=0.05;

	            data->FindItem("HarvestWaste",&HarvestWaste);
	            if(data->FindItem("HarvestWaste",&HarvestWaste)==true)
	            {
	             theControlParameters->SetHarvestWaste(HarvestWaste); // 0.05 - 0.10 may be a sensible range
	            }


	            double StrawLeft= 0.45;

	            data->FindItem("StrawLeft",&StrawLeft);

	            theControlParameters->SetStrawLeft(StrawLeft);
	            if (data->FindItem("ForcedOperations",&ForcedOperations)==true)
	            	theControlParameters->SetUseReductionParameters(UseReductionParameters);
	            if (data->FindItem("ForcedOperations",&ForcedOperations)==true)
	            	theControlParameters->SetGrassWeed(GrassWeed);
	            if (data->FindItem("ForcedOperations",&ForcedOperations)==true)
	             	theControlParameters->SetOrganicFarm(Organic);

	               theTime.SetTime(day,month,StartYear);


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


	               theProducts                  = new products;
	               theOutput                    = new IndicatorOutput;
	               theMessage                   = new message;


	               theTechnics                  = new technics("technics",0); // BMP
	               theClimate                   = new climate(ClimateFileType,EpotCalculationMethod); // BMP
	               theBuildings        	        = new buildings;
	               theManure_manager            = new manureManager();
	               eventControl * aEventControl = new eventControl;

	               theMessage->InitMessage(FN1,FN2,FN3);
	               globalSettings->readParameter();
	               cout<<"Starting Simulation" <<endl;

	               aEventControl->Simulation(index, stopTime,(char*)InputDirectory1.c_str(),(char*)OutputDirectory1.c_str(),(char*)ClimateDirectory.c_str(),
	                           (char*)ClimateFileName.c_str(), (char*) EconomicIndicatorDate.c_str(), (char*) EnvironmentalIndicatorDate.c_str());

	               Display();
	               
	               delete theProducts; //if gcc tries to free this part of memory it failes. It looks like it have already been free but this is the only part where it is?????

	               delete theOutput;
	               if(index==num)
	            	   delete data;
	               delete theTechnics;
	               delete theClimate;
	               delete theManure_manager;
	               delete aEventControl;
	               delete theControlParameters;
				   delete globalSettings;
	               delete theMessage;

	         }


	   return 0;
}
double diffclock(clock_t clock1,clock_t clock2)
{
	double diffticks=clock1-clock2;
	double diffms=(diffticks*1000)/CLOCKS_PER_SEC;
	return diffms;
}





int main(int argc,char *argv[])  //NJH, 28.2.07
{

	
	int times=1;
	fileAccess hd;
	string path=hd.getCurrentPath();
	clock_t begin=clock();
	for(int i=0;i<times;i++)
	{
		hd.changeDir(path);
		modified();
	}
	clock_t end=clock();
	cout << "Time elapsed: " << double(diffclock(end,begin))/times << " ms"<< endl;
	return true;
}
#endif


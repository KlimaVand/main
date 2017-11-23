/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include <bstime.h>
#include <products.h>
#include <message.h>
#include <legislat.h>
#include <thebuild.h>
#include <climclas.h>
#include <event.h>
#include <timeuse.h>
#include <output.h>
#include <contrparm.h>
#include <technics.h>
#include <output.h>
#include <manure_manager.h>
#include <wchar.h>
#include <direct.h>

#ifndef __ANSICPP__
   #include <except.h>
#endif

bsTime theTime;
products * theProducts;
message * theMessage;
timeUse* timeConsumption;
IndicatorOutput * theOutput;
OutputControl* theOutputControl;
legislate * theLegislation;
technics * theTechnics;
buildings * theBuildings;
climate * theClimate;
controlParameters * theControlParameters;
Manure_manager_class *  theManure_manager;

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
int main(int argc,char *argv[])  //NJH, 28.2.07
{
   base aBase;
   bsTime stopTime;

   typedef char string100[100];
   string100
      InputDirectory,
      OutputDirectory,
      ClimateDirectory,
      ClimateFileName,
//      ScenarioFileName,
      baseOutputDir;
   string100 EconomicIndicatorDate="1/1";
   string100 EnvironmentalIndicatorDate="1/1";
   string100 WaterFlowMethod="";

   int StartYear=0;
   int StopYear=0;
   int day=1;
   int month=8;
   int UseGams=0;
   int NumberOfFixedYears = 1;
   int GenerateFixedPlans = 0;
   int NumberOfScenarios = 0;
   int ClimateFileType = -1;
   int EpotCalculationMethod = 0;
   int JumpStart = 0;
   int JumpStop = 0;
   int VolatModel=0;
   int singleField=-1;
   int fertiliserUnit=0;
 //  bool SellPlantProducts=false;
   bool
      UseReductionParameters, 		 // Default determined by theControlParameters
      MeasuredSoilTemperature,       // Default determined by theControlParameters
      WriteSoilTemperature,
      SimpleTemperatureModel,        // Default determined by theControlParameters
      // WriteDetails,                  // Default determined by theControlParameters
      GrassWeed,                     // Default determined by theControlParameters
      Organic, 					    	 // Default determined by theControlParameters
      ForcedOperations,              // Default determined by theControlParameters
      ForcedHarvest,
      fieldsFlexSize,                // When true, fields are allowed to shrimp or expand
      uniSizeFields;                 // When true, all fields are 1 ha big

   int index;
	theOutputControl = new OutputControl;
   cout << "Farm ASSEsment Tool v2.2" << endl << endl;

   char buffer[MAX_TXT];
   _getcwd(buffer, MAX_TXT);
   // NJH 28.02.07
 /*  if (argc==2)       //needed if using Excel spreadsheet interface
   {
   	if (!chdir(argv[1])==0)
   		theMessage->FatalError("Directory not found! Program terminating ");
   }
   else   */
	   _chdir("c:\\fasset_v2\\");
   _getcwd(buffer, MAX_TXT);
   cout << buffer << endl;

   aBase.SetCritical();
   aBase.OpenInputFile("system.dat");
   aBase.UnsetCritical();
   // Used to read info that is common  for all runs in system.dat. Can be overwritten later in the individual run definitions
   int offset=0;
   aBase.GetParameter("OperationOffset",&offset);
   aBase.SetCritical();
   int first,num;
  	aBase.GetSectionNumbers("run",&first,&num);
   aBase.UnsetCritical();
	if (aBase.GetParameter("JumpStart",&JumpStart))
   {
	   aBase.GetParameter("JumpStop",&JumpStop);
      if ((JumpStart>=0)&& (JumpStart<=num))
          first = JumpStart;
      else
      {
         cout << "JumpStart less than zero or greater than number of possible runs" << endl;
         cout << "Program will terminate after keypress" << endl;
   #ifdef __ANSICPP__
         char dum;
         cin >> dum;
   #else
         getch();
   #endif
         exit(99);
      }
   }

  	for(index=first;index<num;index++)
   {
	   bool SkipRun = false;
   	if(aBase.FindSection("run",index))
      {
         aBase.UnsetCritical();
         aBase.GetParameter("SkipRun",&SkipRun);
         if (!SkipRun)
         {
            theControlParameters = new controlParameters;
            theControlParameters->SetoperationOffset(offset);
            if (StartYear==0)
            {
               aBase.SetCritical();
               aBase.GetParameter("StartYear",&StartYear);
               aBase.UnsetCritical();
            }
            if (StopYear==0)
            {
               aBase.SetCritical();
               aBase.GetParameter("StopYear",&StopYear);
               aBase.UnsetCritical();
            }
            aBase.SetCritical();
            aBase.GetParameter("InputDirectory",InputDirectory);
            aBase.GetParameter("OutputDirectory",OutputDirectory);
/*#ifdef __ANSICPP__
            aBase.GetParameter("ClimateDirectory",ClimateDirectory);
            aBase.GetParameter("ClimateFileName",ClimateFileName);
#else*/
            aBase.UnsetCritical();
            ClimateDirectory[0]='\0';
            aBase.GetParameter("ClimateDirectory",ClimateDirectory);
				if (strlen(ClimateDirectory)==0)                         //old format input file - climate directory    NJH Nov 2008
            {
	            aBase.SetCritical();
	            aBase.GetParameter("ClimateFileName",ClimateFileName);
               string instring = ClimateFileName;
               int loc = instring.rfind("\\",instring.length());          //separate directory and file names
               if (loc>0)
               {
                  string direct = instring.substr(0,loc);
                  strcpy(ClimateDirectory,direct.c_str());
                  string climFileName = instring.substr(loc+1);
                  strcpy(ClimateFileName,climFileName.c_str());
	            }
            }
            else
					aBase.GetParameter("ClimateFileName",ClimateFileName);
//#endif
            aBase.UnsetCritical();
            aBase.GetParameter("EnvironmentalIndicator",EnvironmentalIndicatorDate);
            aBase.GetParameter("EconomicIndicator",EconomicIndicatorDate); // !!!
            if (ClimateFileType<0)
               if (!aBase.GetParameter("ClimateFileType",&ClimateFileType))
                  ClimateFileType = 0;
            aBase.UnsetCritical();
            if (aBase.GetParameter("EpotCalculationMethod",&EpotCalculationMethod))
               if (ClimateFileType==0 || ClimateFileType==3)
                  theMessage->FatalError("EpotCalculationMethod is set although evapotranspiration is in the selected climate file type");
            if (ClimateFileType<0 || ClimateFileType>6)
               theMessage->FatalError("Unknown climate file type");
            aBase.GetParameter("EconomicIndicator",EconomicIndicatorDate);
            if (!aBase.GetParameter("UseLPModel",&UseGams))
               UseGams = 0;
            if (!aBase.GetParameter("GenerateFixedPlans",&GenerateFixedPlans))
               GenerateFixedPlans = 0;
            if (!aBase.GetParameter("NumberOfFixedYears",&NumberOfFixedYears))
               NumberOfFixedYears = 1;
            if (!aBase.GetParameter("NumberOfScenarios",&NumberOfScenarios))
               NumberOfScenarios = 0;
            // "WaterFlowMethod" currently not used - only one method available
            if (!aBase.GetParameter("VolatModel",&VolatModel)) //added by NJH Sept 2006
               VolatModel = 0; // OBS - emmision factors !!!
            theControlParameters->SetvolatModel(VolatModel);
            if (aBase.GetParameter("WaterFlowMethod",WaterFlowMethod))
            {
               int wfm=-1;
               if(strcmp("DARCY",WaterFlowMethod)==0)
                  wfm=0;
               if(strcmp("TIPPING",WaterFlowMethod)==0)
                  wfm=1;
               if(strcmp("RICHARDS",WaterFlowMethod)==0)
                  wfm=2;
               if(strcmp("COMBINED",WaterFlowMethod)==0)
                  wfm=3;
               if (wfm<0)
               {
                  cout << endl << "Illegal string '" << WaterFlowMethod << "' after 'WaterFlowMethod' in 'system.dat'" << endl;
                  cout << "Program will terminate after keypress" << endl;
#ifdef __ANSICPP__
      char dum;
      cin >> dum;
#else
      getch();
#endif
                  exit(99);
               }
               theControlParameters->SetWaterFlowMethod(wfm);
            }
            if (aBase.GetParameter("SimpleTemperatureModel",&SimpleTemperatureModel))
               theControlParameters->SetSimpleTemperatureModel(SimpleTemperatureModel);
            uniSizeFields=false;
            if(aBase.GetParameter("AllFieldsOneSize",&uniSizeFields))
               theControlParameters->SetFieldsUniSize(uniSizeFields);
            fieldsFlexSize=false;
            if(aBase.GetParameter("FieldsFlexSize",&fieldsFlexSize)) // Allow fields to expand or shrimp
               theControlParameters->SetFieldsFlexSize(fieldsFlexSize);
            if (uniSizeFields && fieldsFlexSize)
               theMessage->FatalError("main - uniSizeFields and fieldsFlexSize can not both be set to true at the same time");
            if(aBase.GetParameter("SingleField",&singleField))
            {
               theControlParameters->SetSingleField(singleField);
               theOutputControl->SetPlantDetails(true);
               theOutputControl->SetSoilDetails(true);
            }
            if(aBase.GetParameter("FertiliserUnit",&fertiliserUnit))
               theControlParameters->SetFertiliserUnit(fertiliserUnit);
            if (aBase.GetParameter("MeasuredSoilTemperature",&MeasuredSoilTemperature))
               theControlParameters->SetMeasuredSoilTemperature(MeasuredSoilTemperature);
            if (aBase.GetParameter("WriteSoilTemperature",&WriteSoilTemperature))             // Write soil temperature to file for later use in the above procedure (for speed reasons)
               theControlParameters->SetWriteSoilTemperature(WriteSoilTemperature);
            ForcedOperations=false;
            if (aBase.GetParameter("ForcedOperations",&ForcedOperations))
               theControlParameters->SetForcedOperations(ForcedOperations);
            ForcedHarvest = ForcedOperations;
            aBase.GetParameter("ForcedHarvest",&ForcedHarvest);
            theControlParameters->SetForcedHarvest(ForcedHarvest);
            double hw=0.05;
            if (aBase.GetParameter("HarvestWaste",&hw))
               theControlParameters->SetHarvestWaste(hw); // 0.05 - 0.10 may be a sensible range
            if (hw>1.0 || hw<0.0)
               theMessage->FatalError("main - illegal value for harvest waste");
            double sl=0.45;
            aBase.GetParameter("StrawLeft",&sl);
            theControlParameters->SetStrawLeft(sl);
            if (aBase.GetParameter("UseReductionParameters",&UseReductionParameters))
               theControlParameters->SetUseReductionParameters(UseReductionParameters);
            if (aBase.GetParameter("GrassWeed",&GrassWeed))
               theControlParameters->SetGrassWeed(GrassWeed);
            if (aBase.GetParameter("OrganicFarm",&Organic))
               theControlParameters->SetOrganicFarm(Organic);
            aBase.SetCritical();
            strcpy(baseOutputDir,OutputDirectory);

            for (int scenario=0; scenario<=NumberOfScenarios; scenario++)
            {
               int OffSet [30] = {0,5,10,15,20,25,30,23,24,26,28};
               for (int i=11; i<30;i++)
                  OffSet[i] = i;
               theTime.SetTime(day,month,StartYear+OffSet[scenario]);
               cout << "Simulation starts: " << theTime << endl;

               stopTime.SetTime(day,month,StopYear+OffSet[scenario]);       			// JB added this
               cout << "Simulation stops:  " << stopTime << endl;

               if (NumberOfScenarios>0)
               {
                  char scenarioNumber [40];
                  itoa(scenario,scenarioNumber,10);
                  strcpy(OutputDirectory,baseOutputDir);
                  strcat(OutputDirectory,scenarioNumber);
                  strcat(OutputDirectory,"\\");
                  UseGams = 0;
                  GenerateFixedPlans = 0;
               }
               string100 FN1,FN2, FN3, FN4, FN5;
//               chdir(OutputDirectory);
               strcpy(FN1,OutputDirectory);
               strcat(FN1,"warnings.txt");
               strcpy(FN2,OutputDirectory);
               strcat(FN2,"logfile.txt");
               strcpy(FN3,OutputDirectory);
               strcat(FN3,"debug.txt");      //debug file name
               strcpy(FN4,OutputDirectory);
               strcat(FN4,"fieldfile1.txt");
               strcpy(FN5,OutputDirectory);
               strcat(FN5,"fieldfile2.txt");
               theOutputControl->ScenarieLog(InputDirectory);
               theOutputControl->ScenarieLog(OutputDirectory);
               theOutputControl->SetoutputDirectory(OutputDirectory);
               theProducts                  = new products;
               theOutput                    = new IndicatorOutput;
               theMessage                   = new message;
               timeConsumption              = new timeUse;
               theLegislation               = new legislate;
               theTechnics                  = new technics("technics",0); // BMP
               theClimate                   = new climate(ClimateFileType,EpotCalculationMethod); // BMP
               theBuildings        	        = new buildings;
               theManure_manager            = new Manure_manager_class();
               eventControl * aEventControl = new eventControl;
               theMessage->InitMessage(FN1,FN2,FN3,FN4,FN5);

               aEventControl->Simulation(index, stopTime,UseGams,NumberOfFixedYears,GenerateFixedPlans,(char*)&InputDirectory,(char*)&OutputDirectory,(char*)&ClimateDirectory,
                                         (char*)&ClimateFileName, (char*) &EconomicIndicatorDate, (char*) &EnvironmentalIndicatorDate);

               Display();
               delete theProducts;
               delete theOutput;
               delete timeConsumption;
               delete theMessage;
               delete theLegislation;
               delete theTechnics;
               delete theClimate;
               delete theBuildings;
               delete theManure_manager;
               delete aEventControl;
            }
            delete theControlParameters;
         }
      }
      else
         theMessage->FatalError("Error in numbering of sections '[run()]'");
   }
   delete theOutputControl;
   aBase.CloseInputFile();
   return 0;
}


/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "..\Nitroscape\MMSClass.h"


#ifndef __BCplusplus__
 using namespace std;
#endif

typedef double double4[4];
typedef double4 * ptrdouble4;
typedef char string100[100];
bsTime theTime;
products * theProducts;
message * theMessage;
systemData *thesystemData;
environData *theenvironData;
farmData *thefarmData;

IndicatorOutput * theOutput;
outputData* theoutputData;
legislate * theLegislation;
technics * theTechnics;
buildings * theBuildings;
climate * theClimate;
timeUse * timeConsumption;
controlParameters * theControlParameters;
manureManager *  theManure_manager;
eventControl *aEventControl;
bsTime stopTime;
linkList<patchInterfaceClass> * thePatches;
int simYear;

MMSClass * theMMS;

extern "C" __declspec(dllexport)
int GetNumOfPatches()
{return theMMS->GetNumOfPatches();}

extern "C" __declspec(dllexport)
void SetEcoStatus(int patchNo, double ReadyForHarvestOrLater,
      double IrrigationDemand, float prod[4][2])

{
	linkList <harvInfoClass> *theHarvestProducts = new linkList <harvInfoClass>;
   for (int i=0; i<2; i++)
   {
   	harvInfoClass *aharvInfoClass = new harvInfoClass();
      aharvInfoClass->SetproductCode(int (prod[0][i]));
      aharvInfoClass->SetamountDM(prod[1][i]);
      aharvInfoClass->SetamountN(prod[1][i]*prod[2][i]/100.0);
      aharvInfoClass->SetamountC(prod[1][i]*prod[3][i]/100.0);
      theHarvestProducts->InsertLast(aharvInfoClass);
   }
	theMMS->SetEcoStatus(patchNo, ReadyForHarvestOrLater, IrrigationDemand, theHarvestProducts);
}

extern "C" __declspec(dllexport)
void GetMMSTillage(int patchNo, double *theType, double *theDepth, double *theFractionIncorporated)
{
	theMMS->GetMMSTillage(patchNo, theType, theDepth, theFractionIncorporated);
}

extern "C" __declspec(dllexport)
void GetMMSAddOrganic(int patchNo, double *theType, double *theamount, double *theDepth, double *theTotalN, double *theNH4N,
		double *theNO3N, double *thepercentDM, double *theC)
{
	theMMS->GetMMSAddOrganic(patchNo, theType, theamount, theDepth, theTotalN, theNH4N, theNO3N, thepercentDM, theC);
}

extern "C" __declspec(dllexport)
void GetMMSDoFertilisation(int patchNo, double *theNH4N, double *theNO3N)
{
	theMMS->GetMMSDoFertilisation(patchNo, theNH4N, theNO3N);
}

extern "C" __declspec(dllexport)
void GetMMSDoSowing(int patchNo, double *theSeedCode, double *theSeedDM, double *theSeedN)
{
	theMMS->GetMMSDoSowing(patchNo, theSeedCode, theSeedDM, theSeedN);
}

extern "C" __declspec(dllexport)
void GetMMSDoHarvesting(int patchNo, double *theharvestCode, double *thepropGrainHarvested, double *thepropStrawHarvested)
{
	theMMS->GetMMSDoHarvesting(patchNo, theharvestCode, thepropGrainHarvested, thepropStrawHarvested);
}


extern "C" __declspec(dllexport)
void initialise_MMS(char * inputDirectory, char * climateDirectory, char * climateFileName, char * outputDirectory)
{

      systemData::get_instance();
      environData::get_instance();
      outputData::get_instance();
      string outputDirectory(outputDir);
      systemData::get_instance()->runData[0].setOutputDirectory(outputDirectory);
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



	theMMS = new MMSClass();
	theMMS->initialise_MMS(inputDirectory, climateDirectory, climateFileName, outputDirectory);
}


extern "C" __declspec(dllexport)
double GetMMSPatchAmmonia(int patchNo)
{
	return theMMS->GetMMSPatchAmmonia(patchNo);
}

extern "C" __declspec(dllexport)
double GetMMSHouseAmmonia(int houseNo)
{
	return theMMS->GetMMSHouseAmmonia(houseNo);
}

extern "C" __declspec(dllexport)
double GetMMSStoreAmmonia(int storeNo)
{
	return theMMS->GetMMSStoreAmmonia(storeNo);
}


extern "C" __declspec(dllexport)
double GetMMSAllSpreadAmmonia()
{
	return theMMS->GetMMSSpreadAmmonia();
}

extern "C" __declspec(dllexport)
double GetMMSAllHouseAmmonia()
{
	return theMMS->GetMMSHouseAmmonia();
}

extern "C" __declspec(dllexport)
double GetMMSAllStoreAmmonia()
{
	return theMMS->GetMMSStoreAmmonia();
}



extern "C" __declspec(dllexport)
   int GetNumberOfHouses()
{
	return theMMS->GetNumberOfHouses();
}

extern "C" __declspec(dllexport)
   int GetNumberOfStore()
{
	return theMMS->GetNumberOfStore();
}

extern "C" __declspec(dllexport)
void DoMMSDaily()
{
	theMMS->zeroFarmOutputVariables();
	theMMS->DoMMSDaily();
}

extern "C" __declspec(dllexport)
void Destroy_MMS()
{
	theMMS->Destroy_MMS();
}

int modified()
{

	   //base aBase;
	   bsTime stopTime;

	   std::string
	      InputDirectory,
	      OutputDirectory,
	      ClimateDirectory,
	      ClimateFileName,
	//      ScenarioFileName,
	      baseOutputDir;
	   std::string EconomicIndicatorDate="1/1";
	   std::string EnvironmentalIndicatorDate="1/1";
	   std::string WaterFlowMethod="";

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
	   cout << "Farm ASSEsment Tool v2.2" << endl << endl;




	   // Used to read info that is common  for all runs in system.dat. Can be overwritten later in the individual run definitions
	   int offset=systemData::get_instance()->getOperationOffset();
	   int first=systemData::get_instance()->getJumpStart();
	   int num=systemData::get_instance()->getJumpStop();

	  	for(index=first;index<=num;index++)
	   {
	  		systemData::get_instance()->setCurrentRun(index);
            environData::get_instance()->reload();
            farmData::get_instance()->reload();
            outputData::get_instance()->reload();
		   bool SkipRun = systemData::get_instance()->runData[index].getSkipRun();
	         if (!SkipRun)
	         {
	        	 if(systemData::get_instance()->getremoveOldData()==true)
	        	 {
	        		 fileAccess remove;
	        		 string path=systemData::get_instance()->runData[index].getOutputDirectory();
	        		 remove.deleteEveryFileInFolder(path);
	        	 }
	            theControlParameters = new controlParameters;
	            theControlParameters->SetoperationOffset(offset);
	            StartYear=systemData::get_instance()->runData[index].getStartYear();
	            StopYear=systemData::get_instance()->runData[index].getStopYear();
	            InputDirectory=systemData::get_instance()->runData[index].getInputDirectory();
	            OutputDirectory=systemData::get_instance()->runData[index].getOutputDirectory();
	            ClimateDirectory=systemData::get_instance()->runData[index].getClimateDirectory();
				EnvironmentalIndicatorDate=systemData::get_instance()->runData[index].getEnvironmentalIndicator();// aBase.GetParameter("EnvironmentalIndicator",EnvironmentalIndicatorDate);

				ClimateFileType =systemData::get_instance()->runData[index].getClimateFileType();

				EpotCalculationMethod=systemData::get_instance()->runData[index].getEpotCalculationMethod();

	            EconomicIndicatorDate=systemData::get_instance()->runData[index].getEconomicIndicatorDate();
	            UseGams=systemData::get_instance()->runData[index].getUseLPModel();
	            GenerateFixedPlans=systemData::get_instance()->runData[index].getGenerateFixedPlans();
	            NumberOfFixedYears=systemData::get_instance()->runData[index].getNumberOfFixedYears();
	            NumberOfScenarios=systemData::get_instance()->runData[index].getNumberOfScenarios();
	            VolatModel=systemData::get_instance()->runData[index].getVolatModel();
               ClimateFileName= systemData::get_instance()->runData[index].getClimateFileName();
	            theControlParameters->SetvolatModel(VolatModel);
	            int wfm=systemData::get_instance()->runData[index].getWaterFlowMethodInt();
	            theControlParameters->SetWaterFlowMethod(wfm);
	            if(systemData::get_instance()->runData[index].getSimpleTemperatureModelFile()==true)
	            {
	            	SimpleTemperatureModel=systemData::get_instance()->runData[index].getSimpleTemperatureModel();
	            	theControlParameters->SetSimpleTemperatureModel(SimpleTemperatureModel);
	            }
	            if(systemData::get_instance()->runData[index].getAirToSoilTemperatureFile()==true)
	            {
	            	AirToSoilTemperature=systemData::get_instance()->runData[index].getAirToSoilTemperature();
	            	theControlParameters->SetAirToSoilTemperature(AirToSoilTemperature);
	            }
	            if(systemData::get_instance()->runData[index].getuniSizeFieldsFile()==true)
	            {
	            	uniSizeFields=systemData::get_instance()->runData[index].getuniSizeFields();
	            	theControlParameters->SetFieldsUniSize(uniSizeFields);
	            }
	            if(systemData::get_instance()->runData[index].getuniSizeFieldsFile()==true)
	            {
	            	fieldsFlexSize=systemData::get_instance()->runData[index].getFieldsFlexSize();
	            	theControlParameters->SetFieldsFlexSize(fieldsFlexSize);
	            }
	            singleField=systemData::get_instance()->runData[index].getsingleField();
	            if(systemData::get_instance()->runData[index].getsingleFieldFile()==true)
	            {
	               theControlParameters->SetSingleField(singleField);
	               outputData::get_instance()->DetailsData.setPlantDetails(true);
	               outputData::get_instance()->DetailsData.setSoilDetails(true);
	            }
	            if(systemData::get_instance()->runData[index].getfertiliserUnitFile()==true)
	            {
	            	fertiliserUnit=systemData::get_instance()->runData[index].getfertiliserUnit();
					theControlParameters->SetFertiliserUnit(fertiliserUnit);
	            }
	            if(systemData::get_instance()->runData[index].getMeasuredSoilTemperatureFile()==true)
	           	{
	            	MeasuredSoilTemperature=systemData::get_instance()->runData[index].getMeasuredSoilTemperature();
	            	theControlParameters->SetMeasuredSoilTemperature(MeasuredSoilTemperature);

	           	}


	            if(systemData::get_instance()->runData[index].getMeasuredSoilTemperatureFile()==true)
	            {
						WriteSoilTemperature=systemData::get_instance()->runData[index].getWriteSoilTemperature();
						theControlParameters->SetWriteSoilTemperature(WriteSoilTemperature);
	           	}
	            if (systemData::get_instance()->runData[index].getForcedOperationsFile()==true)
	            {
	            	ForcedOperations=systemData::get_instance()->runData[index].getForcedOperations();
						theControlParameters->SetForcedOperations(ForcedOperations);
	            }

	            ForcedHarvest =systemData::get_instance()->runData[index].getForcedHarvest();
	            theControlParameters->SetForcedHarvest(ForcedHarvest);
	            double HarvestWaste=0.05;
	            HarvestWaste=systemData::get_instance()->runData[index].getHarvestWaste();
	            if(systemData::get_instance()->runData[index].getHarvestWasteFile()==true)
	            {
	             theControlParameters->SetHarvestWaste(HarvestWaste); // 0.05 - 0.10 may be a sensible range
	            }


	            double StrawLeft= systemData::get_instance()->runData[index].getStrawLeft();
	            theControlParameters->SetStrawLeft(StrawLeft);
	            if (systemData::get_instance()->runData[index].getForcedOperationsFile()==true)
	            	theControlParameters->SetUseReductionParameters(UseReductionParameters);
	            if (systemData::get_instance()->runData[index].getForcedOperationsFile()==true)
	            	theControlParameters->SetGrassWeed(GrassWeed);
	            if (systemData::get_instance()->runData[index].getForcedOperationsFile()==true)
	             	theControlParameters->SetOrganicFarm(Organic);
	            baseOutputDir=baseOutputDir+OutputDirectory;

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
	                  OutputDirectory=OutputDirectory+baseOutputDir+scenarioNumber+"\\";
	                  UseGams = 0;
	                  GenerateFixedPlans = 0;
	               }

	               char FN1[100],FN2[100], FN3[100], FN4[100], FN5[100];
	//               chdir(OutputDirectory);
	               strcpy(FN1,OutputDirectory.c_str());
	               strcat(FN1,"warnings.txt");
	               strcpy(FN2,OutputDirectory.c_str());
	               strcat(FN2,"logfile.txt");
	               strcpy(FN3,OutputDirectory.c_str());
	               strcat(FN3,"debug.txt");      //debug file name
	               strcpy(FN4,OutputDirectory.c_str());
	               strcat(FN4,"fieldfile1.txt");
	               strcpy(FN5,OutputDirectory.c_str());
	               strcat(FN5,"fieldfile2.txt");

                  char string80[80];
                   strcpy(string80, OutputDirectory.c_str());


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
	               cout<<"staring Simulation" <<endl;

	               aEventControl->Simulation(index, stopTime,UseGams,NumberOfFixedYears,GenerateFixedPlans,(char*)InputDirectory.c_str(),(char*)OutputDirectory.c_str(),(char*)ClimateDirectory.c_str(),
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

	            }
	            delete theControlParameters;
	         }
	   }

	   return 0;
}


/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../Nitroscape/MMSClass.h"
#include "../Nitroscape/harvInfoClass.h"
#include "../base/timeuse.h"
#include "../data/systemData.h"

#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>




void MMSClass::InitialiseInterface(eventControl *aEventControl)
{

   int numFields = aEventControl->GettheCropRotation()->GetFields()->NumOfNodes();
   for (int j=0; j<numFields;j++)
   {
      field *aField = aEventControl->GettheCropRotation()->GetFields()->ElementAtNumber(j);
      aField->UpdatePatch(farmNo);

   }
   CloseInputFile();
   for (int i=0; i<myBuildings->GettheStables()->NumOfNodes(); i++)
   {
   	houseEmission[i].location=myBuildings->GetStable(i)->Getlocation();
   	houseEmission[i].name=myBuildings->GetStable(i)->GetAlias();
   }
   for (int i=0; i<myBuildings->GettheManurestores()->NumOfNodes(); i++)
   {
   //I have a feeling that the following cast is a bit naughty
   	storeEmission[i].location=((building *) myBuildings->GetManurestore(i))->Getlocation();
   	storeEmission[i].name=myBuildings->GetManurestore(i)->GetAlias();
   }
}

void MMSClass::zeroPatchOutputVariables()
{

	//for(int i=0;i<fieldList.size();i++)
	{
	//	fieldList.at(i)->zeroOutputVariables();
	}
}

void MMSClass::zeroFarmOutputVariables()
{
   for (int i=0; i<theBuildings->GettheStables()->NumOfNodes(); i++)
   {
   	houseEmission[i].NH3Emission=0.0;
   	houseEmission[i].N2OEmission=0.0;
   }
}

double MMSClass::GetMMSHouseAmmonia(int houseNo)
{
	double retVal=0.0;
   retVal=houseEmission[houseNo].NH3Emission;
	return retVal;
}

double MMSClass::GetMMSHouseAmmonia()
{
	double retVal=0.0;
   for (int i=0; i<myBuildings->GettheStables()->NumOfNodes();i++)
   {
	   retVal+=houseEmission[i].NH3Emission;
	}
	return retVal*1000.0;
}

double MMSClass::GetMMSHouseN2O(int houseNo)
{
	double retVal=0.0;
   retVal=houseEmission[houseNo].N2OEmission;
	return retVal;
}

double MMSClass::GetMMSHouseN2O()
{
	double retVal=0.0;
   for (int i=0; i<myBuildings->GettheStables()->NumOfNodes();i++)
   {
	   retVal+=houseEmission[i].N2OEmission;
	}
	return retVal*1000.0;
}

double MMSClass::GetMMSHouseNO(int houseNo)
{
	double retVal=0.0;
   retVal=houseEmission[houseNo].NOEmission;
	return retVal;
}

double MMSClass::GetMMSHouseNO()
{
	double retVal=0.0;
   for (int i=0; i<myBuildings->GettheStables()->NumOfNodes();i++)
   {
	   retVal+=houseEmission[i].NOEmission;
	}
	return retVal*1000.0;
}

double MMSClass::GetMMSStoreAmmonia(int storeNo)
{
	double retVal=0.0;
   retVal=storeEmission[storeNo].NH3Emission;
	return retVal*1000.0;
}

double MMSClass::GetMMSStoreAmmonia()
{
	double ret_val=0.0;
   for (int i=0; i<myBuildings->GettheManurestores()->NumOfNodes();i++)
      ret_val +=storeEmission[i].NH3Emission;
	return ret_val*1000.0;
}

double MMSClass::GetMMSStoreN2O(int storeNo)
{
	double retVal=0.0;
   retVal=storeEmission[storeNo].N2OEmission;
	return retVal*1000.0;
}

double MMSClass::GetMMSStoreN2O()
{
	double ret_val=0.0;
   for (int i=0; i<myBuildings->GettheManurestores()->NumOfNodes();i++)
      ret_val +=storeEmission[i].N2OEmission;
	return ret_val*1000.0;
}

double MMSClass::GetMMSStoreNO(int storeNo)
{
	double retVal=0.0;
   retVal=storeEmission[storeNo].NOEmission;
	return retVal*1000.0;
}

double MMSClass::GetMMSStoreNO()
{
	double ret_val=0.0;
   for (int i=0; i<myBuildings->GettheManurestores()->NumOfNodes();i++)
      ret_val +=storeEmission[i].NOEmission;
	return ret_val*1000.0;
}

/****************************************************************************\
\****************************************************************************/
//void MMSClass::initialise_MMS(string inputDirectory, string climateDirectory, string climateFileName, int climateFileType, string outputDirectory)
void MMSClass::initialise_MMS(systemRunData aRunData)
{


   mySystemData = new systemData(false);

   mySystemData->SetSystemRunData(aRunData);
   mySystemData->setCurrentRun(0);
	strcpy(myInputDirectory,mySystemData->getInputDirectory().c_str());
	strcpy(myOutputDirectory,mySystemData->getOutputDirectory().c_str());
   strcpy(myClimateDirectory,mySystemData->getClimateDirectory().c_str());
   strcpy(myClimateFileName,mySystemData->getClimateFileName().c_str());
   //set global to local, otherwise outputData, farmData and environData will get data from the wrong place
   theSystemData = mySystemData;
   myOutputData = new outputData();
   myFarmData = new farmData();
   myEnvironData = new environData();
   theClimate=NULL;

   for (int i=0; i<20; i++)
   {
		houseEmission[i].NH3Emission=0.0;
		storeEmission[i].NH3Emission=0.0;
		houseEmission[i].N2OEmission=0.0;
		storeEmission[i].N2OEmission=0.0;
		houseEmission[i].NOEmission=0.0;
		storeEmission[i].NOEmission=0.0;
   }

   std::string
   InputDirectory,
   OutputDirectory,
   ClimateDirectory,
   ClimateFileName,
   baseOutputDir;
   std::string EconomicIndicatorDate="1/1";
   std::string EnvironmentalIndicatorDate="1/1";
   std::string WaterFlowMethod="";

   int ClimateFileType = mySystemData->getClimateFileType();
   int EpotCalculationMethod = 0;

   int VolatModel=0;
   int singleField=-1;
   int fertiliserUnit=0;
	myTime = new bsTime(1, 8, mySystemData->getStartYear());
	stopTime = new bsTime(1,8,mySystemData->getStopYear());
   simYear =0;
   //  bool SellPlantProducts=false;
   bool
   UseReductionParameters=false, 		 // Default determined by theControlParameters
   MeasuredSoilTemperature=false,       // Default determined by theControlParameters
   WriteSoilTemperature=false,
   SimpleTemperatureModel=false,        // Default determined by theControlParameters
   // WriteDetails,                  // Default determined by theControlParameters
   GrassWeed=false,                     // Default determined by theControlParameters
   Organic=false, 					    	 // Default determined by theControlParameters
   ForcedOperations=false,              // Default determined by theControlParameters
   ForcedHarvest=false,
   fieldsFlexSize=false,                // When true, fields are allowed to shrimp or expand
   uniSizeFields=false;                 // When true, all fields are 1 ha big


   myControlParameters = new controlParameters;
   EnvironmentalIndicatorDate=mySystemData->getEnvironmentalIndicator();// aBase.GetParameter("EnvironmentalIndicator",EnvironmentalIndicatorDate);

   EpotCalculationMethod=mySystemData->getEpotCalculationMethod();

   myControlParameters->SetvolatModel(VolatModel);
   int wfm=mySystemData->getWaterFlowMethodInt();
   myControlParameters->SetWaterFlowMethod(wfm);
   if(mySystemData->getSimpleTemperatureModelFile()==true)
   {
      SimpleTemperatureModel=mySystemData->getSimpleTemperatureModel();
      myControlParameters->SetSimpleTemperatureModel(SimpleTemperatureModel);
   }
   if(mySystemData->getuniSizeFieldsFile()==true)
   {
      uniSizeFields=mySystemData->getuniSizeFields();
      myControlParameters->SetFieldsUniSize(uniSizeFields);
   }
   if(mySystemData->getuniSizeFieldsFile()==true)
   {
      fieldsFlexSize=mySystemData->getFieldsFlexSize();
      myControlParameters->SetFieldsFlexSize(fieldsFlexSize);
   }
   singleField=mySystemData->getsingleField();
   if(mySystemData->getsingleFieldFile()==true)
   {
      myControlParameters->SetSingleField(singleField);
      myOutputData->DetailsData.setPlantDetails(true);
      myOutputData->DetailsData.setSoilDetails(true);
   }
   if(mySystemData->getfertiliserUnitFile()==true)
   {
      fertiliserUnit=mySystemData->getfertiliserUnit();
   myControlParameters->SetFertiliserUnit(fertiliserUnit);
   }
   if(mySystemData->getMeasuredSoilTemperatureFile()==true)
   {
      MeasuredSoilTemperature=mySystemData->getMeasuredSoilTemperature();
      myControlParameters->SetMeasuredSoilTemperature(MeasuredSoilTemperature);

   }


   if(mySystemData->getMeasuredSoilTemperatureFile()==true)
   {
   WriteSoilTemperature=mySystemData->getWriteSoilTemperature();
   myControlParameters->SetWriteSoilTemperature(WriteSoilTemperature);
   }
   if (mySystemData->getForcedOperationsFile()==true)
   {
      ForcedOperations=mySystemData->getForcedOperations();
   myControlParameters->SetForcedOperations(ForcedOperations);
   }

   ForcedHarvest =mySystemData->getForcedHarvest();
   myControlParameters->SetForcedHarvest(ForcedHarvest);
   double HarvestWaste=0.05;
   HarvestWaste=mySystemData->getHarvestWaste();
   if(mySystemData->getHarvestWasteFile()==true)
   {
    myControlParameters->SetHarvestWaste(HarvestWaste); // 0.05 - 0.10 may be a sensible range
   }


   double StrawLeft= mySystemData->getStrawLeft();
   myControlParameters->SetStrawLeft(StrawLeft);
   if (mySystemData->getForcedOperationsFile()==true)
      myControlParameters->SetUseReductionParameters(UseReductionParameters);
   if (mySystemData->getForcedOperationsFile()==true)
      myControlParameters->SetGrassWeed(GrassWeed);
   if (mySystemData->getForcedOperationsFile()==true)
      myControlParameters->SetOrganicFarm(Organic);
   OutputDirectory=mySystemData->getOutputDirectory();
   baseOutputDir=baseOutputDir+OutputDirectory;



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


   myProducts                  = new products;
   myOutput                    = new IndicatorOutput;
   myMessage                   = new message;
   timeConsumption              = new timeUse;
   myLegislation               = new legislate;
   myTechnics                  = new technics("technics",0); // BMP
   myClimate                   = new climate(ClimateFileType,EpotCalculationMethod);
   myBuildings        	        = new buildings;
   myManure_manager            = new manureManager();
   myEventControl = new eventControl();
   SetMMSGlobals();

   myMessage->InitMessage(FN1,FN2,FN3,FN4,FN5);
/*   cout << "Input directory    " << inputDirectory << endl;
   cout << "Output directory   " << outputDirectory << endl;
   cout << "Climate directory  " << climateDirectory << endl;
   cout << "Climate file       " << climateFileName << endl;*/

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
//	  mkdir(myOutputDirectory,'-p');
/*	#else
	  cout << "Error - directory not found " << endl;
	  cout << errno << endl;
	  cout << "Press any key to exit " << endl;
	  char dum;
	  cin >> dum;
	  exit(99);
	#endif*/
#endif
   if (chdir(myInputDirectory)!=0)
		myMessage->FatalError("Event:: input directory ",myInputDirectory," not found!");
	myEventControl->Initialize(stopTime->GetYear(),(char*)&myInputDirectory,(char*)&myClimateDirectory,(char*)&myClimateFileName,(char*)&myOutputDirectory);
	InitialiseInterface(myEventControl);
}

void MMSClass::DoMMSDaily()
{
	SetMMSGlobals();
   chdir(myInputDirectory);
	zeroPatchOutputVariables();
   if ((theTime.GetDay()==30)&&(theTime.GetMonth()==4))
   	cout << "got to 1.5" << endl;
   if ((theTime.GetDay()==1)&&(theTime.GetMonth()==8))
   {
      char fileExtension[4];
      fileExtension[0] = '\0';
      if (simYear<100)
         sprintf(fileExtension,"f%02d",simYear);
      else
         sprintf(fileExtension,"f%03d",simYear);
      myEventControl->ReceivePlan(fileExtension,myInputDirectory);
      simYear++;
   }
   myEventControl->DailyUpdate();
   for (int i=0; i<myBuildings->GettheStables()->NumOfNodes(); i++)
   {
   	houseEmission[i].NH3Emission=myBuildings->GetStable(i)->GetNH3Emission();
   	houseEmission[i].N2OEmission=myBuildings->GetStable(i)->GetN2OEmission();
   	houseEmission[i].NOEmission=myBuildings->GetStable(i)->GetNOEmission();
   }
   for (int i=0; i<myBuildings->GettheManurestores()->NumOfNodes(); i++)
   {
   	storeEmission[i].NH3Emission=myBuildings->GetManurestore(i)->GetNH3Emission();
   	storeEmission[i].N2OEmission=myBuildings->GetManurestore(i)->GetN2OEmission();
   	storeEmission[i].NOEmission=myBuildings->GetManurestore(i)->GetNOEmission();
   }
   if ((myTime->GetDay()==31) && (myTime->GetMonth()==7))
      myEventControl->GiveIndicator(environmentalIndicator,simYear);

   myTime->AddOneDay();
}

void MMSClass::Destroy_MMS()
{
   delete myProducts;
   delete myOutput;
   delete myMessage;
   delete myLegislation;
   delete myTechnics;
   delete myClimate;
   delete myBuildings;
   delete myManure_manager;
   delete myEventControl;
   delete myOutputData;
   delete myTime;
   delete stopTime;
}

void MMSClass::SetMMSGlobals()
{
   theProducts = myProducts;
	theMessage	= myMessage;
//	thetimeConsumption* mytimeConsumption;
	theOutput	= myOutput;
	theLegislation = myLegislation;
	theTechnics	= myTechnics;
	theBuildings	= myBuildings;
	theClimate = myClimate;
	theControlParameters = myControlParameters;
   theOutput = myOutput;
	theManure_manager	= myManure_manager;
   theOutputData = myOutputData;
   theEnvironData = myEnvironData;
   theFarmData = myFarmData;
   theTime = *myTime;
}

bool MMSClass::GetfinishedRun()
{
   if (*myTime == *stopTime)
      return true;
   else
      return false;
};


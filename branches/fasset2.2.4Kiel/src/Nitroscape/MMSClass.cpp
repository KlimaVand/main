/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../Nitroscape/MMSClass.h"
#include "../Nitroscape/harvInfoClass.h"
#include "../base/timeUse.h"
#include "../data/systemData.h"

#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>


int MMSClass::GetNumOfPatches()
{
 int aVal=theInterfacePatches->NumOfNodes();
 return aVal;
}

void MMSClass::InitialiseInterface(eventControl *aEventControl)
{
	theInterfacePatches = new linkList<patchInterfaceClass>;
   int numFields = aEventControl->GettheCropRotation()->GetFields()->NumOfNodes();
   for (int j=0; j<numFields;j++)
   {
      field *aField = aEventControl->GettheCropRotation()->GetFields()->ElementAtNumber(j);
      int numPatches = aField->GetNumOfPatches();

      for (int i=0;i<numPatches;i++)
         theInterfacePatches->InsertLast(aField->GetPatch(i)->GetthePatchInterface());

   }
}

void MMSClass::zeroPatchOutputVariables()
{
   for (int i=0; i<theInterfacePatches->NumOfNodes(); i++)
   {
		patchInterfaceClass * apatchInterfaceClass  = theInterfacePatches->ElementAtNumber(i);
      apatchInterfaceClass->zeroOutputVariables();
   }
}

void MMSClass::zeroFarmOutputVariables()
{
   for (int i=0; i<theBuildings->GettheStables()->NumOfNodes(); i++)
   	houseNH3Emission[i]=0.0;
}


void MMSClass::SetEcoStatus(int patchNo, double ReadyForHarvestOrLater,
      double IrrigationDemand, linkList <harvInfoClass> *theHarvestProducts)

{
	patchInterfaceClass * apatchInterfaceClass  = theInterfacePatches->ElementAtNumber(patchNo);
   apatchInterfaceClass->SetEcoStatus(ReadyForHarvestOrLater,IrrigationDemand,
	   theHarvestProducts);
}

bool MMSClass::GetMMSTillage(int patchNo, double *theType, double *theDepth, double *theFractionIncorporated)
{
	patchInterfaceClass * apatchInterfaceClass  = theInterfacePatches->ElementAtNumber(patchNo);
	*theType=apatchInterfaceClass->GettheType()+1.0;
   *theDepth=apatchInterfaceClass->GettheDepth();
   *theFractionIncorporated = apatchInterfaceClass->GettheFractionIncorporated();
   apatchInterfaceClass->SettheType(-1);
   if (*theType>0.0)
	   return true;
   else
	   return false;
}

bool MMSClass::GetMMSAddOrganic(int patchNo, double *theType, double *theamount, double *theDepth, double *theTotalN, double *theNH4N,
		double *theNO3N, double *thepercentDM, double *theC)
{
	*theType=0.0;  *theamount=0.0; *theDepth=0.0; *theTotalN=0.0; *theNH4N=0.0; *theNO3N=0.0;*theC=0.0;
	patchInterfaceClass * apatchInterfaceClass  = theInterfacePatches->ElementAtNumber(patchNo);
   if (apatchInterfaceClass->GetdecompAmount()>0.0)
   {
		*theamount = apatchInterfaceClass->GetdecompAmount();
	   *theDepth=0.0; //apatchInterfaceClass->GettheDepth();
		*theNH4N = apatchInterfaceClass->GetdecompNH4_N();
		*theNO3N = apatchInterfaceClass->GetdecompNO3_N();
		*theTotalN = *theNH4N + *theNO3N + apatchInterfaceClass->GetdecompOrg_N();
		*theC = apatchInterfaceClass->GetdecompC();
		*thepercentDM = apatchInterfaceClass->GetdecompPercentDM();
      *theType = apatchInterfaceClass->GetdecompType();
      apatchInterfaceClass->SetdecompAmount(0.0);
   }
   if (*theamount>0.0)
	   return true;
   else
	   return false;
}

bool MMSClass::GetMMSDoFertilisation(int patchNo, double *theNH4N, double *theNO3N)
{
	patchInterfaceClass * apatchInterfaceClass  = theInterfacePatches->ElementAtNumber(patchNo);
	*theNO3N = apatchInterfaceClass->GettheNitrate();
   *theNH4N = apatchInterfaceClass->GettheAmmonium();
   if ((*theNO3N>0.0)||(*theNH4N>0.0))
	   return true;
   else
	   return false;
}

bool MMSClass::GetMMSDoSowing(int patchNo, double *theSeedCode, double *theSeedDM, double *theSeedN)
{
	patchInterfaceClass * apatchInterfaceClass  = theInterfacePatches->ElementAtNumber(patchNo);
	*theSeedCode = (double) apatchInterfaceClass->GetthecropID();
   *theSeedDM=apatchInterfaceClass->GetamountSeedDM();
   *theSeedN = apatchInterfaceClass->GetseedN();
   apatchInterfaceClass->SetamountSeedDM(0.0);
   if (*theSeedDM>0.0)
	   return true;
   else
	   return false;
}

bool MMSClass::GetMMSDoHarvesting(int patchNo, double *theharvestCode, double *thepropGrainHarvested, double *thepropStrawHarvested)
{
	patchInterfaceClass * apatchInterfaceClass  = theInterfacePatches->ElementAtNumber(patchNo);
	*theharvestCode = (double) apatchInterfaceClass->GetharvestCode();
   *thepropGrainHarvested=apatchInterfaceClass->GetpropGrainHarvested();
   *thepropStrawHarvested = apatchInterfaceClass->GetpropStrawHarvested();
   apatchInterfaceClass->SetharvestCode(0);
   if (*theharvestCode>0.0)
	   return true;
   else
	   return false;
}

double MMSClass::GetMMSPatchAmmonia(int patchNo)
{
	double ret_val=0.0;
	patchInterfaceClass * apatchInterfaceClass  = theInterfacePatches->ElementAtNumber(patchNo);
   ret_val = apatchInterfaceClass->GetNH3Emission();
	return ret_val;
}

double MMSClass::GetMMSSpreadAmmonia()
{
	double ret_val=0.0;
   for (int i=0; i<theInterfacePatches->NumOfNodes();i++)
   {
      patchInterfaceClass * apatchInterfaceClass  = theInterfacePatches->ElementAtNumber(i);
      ret_val += apatchInterfaceClass->GetNH3Emission();
	}
	return ret_val;
}


double MMSClass::GetMMSHouseAmmonia(int houseNo)
{
	SetMMSGlobals();
	double retVal=0.0;
   retVal=theBuildings->GettheStables()->ElementAtNumber(houseNo)->GetvolatAmmonia()*1000.0;
	return retVal;
}

double MMSClass::GetMMSHouseAmmonia()
{
	double ret_val=0.0;
   for (int i=0; i<myBuildings->GettheStables()->NumOfNodes();i++)
   {
      stable* astore= theBuildings->GettheStables()->ElementAtNumber(i);
      ret_val += astore->GetvolatAmmonia();
	}
	return ret_val*1000.0;
}

double MMSClass::GetMMSStoreAmmonia(int storeNo)
{
	SetMMSGlobals();
	double retVal=0.0;
   retVal=myBuildings->GettheManurestores()->ElementAtNumber(storeNo)->GetvolatAmmonia();
	return retVal*1000.0;
}

double MMSClass::GetMMSStoreAmmonia()
{
	double ret_val=0.0;
   for (int i=0; i<myBuildings->GettheManurestores()->NumOfNodes();i++)
   {
      manurestore* astore= theBuildings->GettheManurestores()->ElementAtNumber(i);
      ret_val += astore->GetvolatAmmonia();
	}
	return ret_val*1000.0;
}

/****************************************************************************\
\****************************************************************************/
//void MMSClass::initialise_MMS(string inputDirectory, string climateDirectory, string climateFileName, int climateFileType, string outputDirectory)
void MMSClass::initialise_MMS(systemRunData aRunData)
{

   mySystemData = new systemData(false);
   mySystemData->SetSystemRunData(aRunData);
  // cout<<aRunData.getAirToSoilTemperature();
/*   mySystemData->setInputDirectory(inputDirectory);
   mySystemData->setClimateDirectory(climateDirectory);
   mySystemData->setClimateFileName(climateFileName);
   mySystemData->setOutputDirectory(outputDirectory);
*/
	strcpy(myInputDirectory,mySystemData->getInputDirectory().c_str());
	strcpy(myOutputDirectory,mySystemData->getOutputDirectory().c_str());
   strcpy(myClimateDirectory,mySystemData->getClimateDirectory().c_str());
   strcpy(myClimateFileName,mySystemData->getClimateFileName().c_str());
   myOutputData = new outputData();
   myFarmData = new farmData();
   myEnvironData = new environData();;

   for (int i=0; i<20; i++)
   {
		houseNH3Emission[i]=0.0;
		storeNH3Emission[i]=0.0;
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
   stopTime.SetTime(31,7,mySystemData->getStartYear());
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
#ifndef __BCplusplus__
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
	myEventControl->Initialize(&stopTime,(char*)&myInputDirectory,(char*)&myClimateDirectory,(char*)&myClimateFileName,(char*)&myOutputDirectory);
	InitialiseInterface(myEventControl);
}

void MMSClass::DoMMSDaily()
{
	SetMMSGlobals();
   chdir(myInputDirectory);
	zeroPatchOutputVariables();
   if ((theTime.GetDay()==1)&&(theTime.GetMonth()==8))
   {
      char fileExtension[5]; //CK
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
   	houseNH3Emission[i]=myBuildings->GetStable(i)->GetvolatAmmonia();
   for (int i=0; i<myBuildings->GettheManurestores()->NumOfNodes(); i++)
   	storeNH3Emission[i]=myBuildings->GetManurestore(i)->GetvolatAmmonia();
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
 //  theSystemData = mySystemData;
   theEnvironData = myEnvironData;
   theFarmData = myFarmData;
   theTime = *myTime;
}


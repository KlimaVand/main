#include "..\base\bstime.h"
#include "..\base\message.h"
#include "..\base\linklist.h"

#include <iostream>
#include "..\Nitroscape\harvInfoClass.h"
message * theMessage;
bsTime theTime;   //needed for theMessage

typedef double double4[4];
typedef double * ptrdouble;

extern "C"
{
// status variables from ecosystem to farm
		int numOfPatches;
   	double TotalLAI;
      double ReadyForHarvestOrLater;
      double IrrigationDemand;
      double IrrigationWater;
      double IrrigationN;
      double AbovegroundCropN;
      double MaxCropHeight;
      double StandingDMdensity;
//      double dryMatterGrazed;
//      double grazedHt;

//Get amount of DM available that is above a certain height (in cm)
	// status variables from ecosystem to farm
		double AvailableStandingDM;

	// from farm to ecosystem
		double AvailableStandingDMHeight;

//tell ecosystem to kill crops
	// from farm to ecosystem
      int KillAllCrops;  //needs to be reset daily

//! from N fertiliser addition
	// from farm to ecosystem
      double theFertNitrate;
      double theFertAmmonium;

// sowing
		double thecropID;
      double amountSeedDM;
      double seedN;

//! tillage
	// from farm to ecosystem
      double theType;                                          // Tillage Type
      double theDepth;                                         // Tillage Depth
      double theFractionIncorporated;                          // Fraction of surface organic matter incorporated


//! harvesting
	// from farm to ecosystem
      double harvestCode;
      double propGrainHarvested;
      double propStrawHarvested;

	// status variables from ecosystem to farm
		double ProductName;
		linkList <harvInfoClass> *HarvestProducts;

//! manuring
	// from farm to ecosystem
   double theManureType;
   double theManureamount;
   double theManureDepth;
   double theManureTotalN;
   double theManureNH4N;
	double theManureNO3N;
   double theManurepercentDM;
   double theManureC;


int testInt;

__declspec(dllimport)
void DoDaily_ECO();
__declspec(dllimport)
void Destroy_ECO();
__declspec(dllimport) void initialise_ECO(int numEcosysts, char * direct, char *climateDir, char * climateFileName, char * outputDir, int day, int month, int year);
__declspec(dllimport) int GetNumOfPatches();
__declspec(dllimport) void initialise_MMS(int farmNo);
__declspec(dllimport) void DoMMSDaily();
__declspec(dllimport) void Destroy_MMS();
__declspec(dllimport) void GetEcoStatus(int ecoNum, double *totalLAI, double *ReadyForHarvestOrLater,
      double *IrrigationDemand, linkList <harvInfoClass> *theHarvestProducts);
__declspec(dllimport) void SetEcoStatus(int patchNo, double ReadyForHarvestOrLater,
      double IrrigationDemand, float prod[4][2]);
__declspec(dllimport) bool GetMMSTillage(int patchNo, double *theType, double *theDepth, double *theFractionIncorporated);
__declspec(dllimport) void SetECOTillage(int ecoNum, double theType, double theDepth, double theFractionIncorporated);
__declspec(dllimport) bool GetMMSDoSowing(int patchNo, double *theSeedCode, double *theSeedDM, double *theSeedN);
__declspec(dllimport) void SetECODoSowing(int ecoNum, double theSeedCode, double theSeedDM, double theSeedN);
__declspec(dllimport) bool GetMMSDoHarvesting(int patchNo, double *theharvestCode, double *thepropGrainHarvested, double *thepropStrawHarvested);
__declspec(dllimport) void SetECODoHarvesting(int ecoNum, double theharvestCode, double thepropGrainHarvested, double thepropStrawHarvested);
__declspec(dllimport) bool GetMMSAddOrganic(int patchNo, double *theType, double *theamount, double *theDepth, double *theTotalN, double *theNH4N,
		double *theNO3N, double *thepercentDM, double *theC);
__declspec(dllimport) void SetECOAddOrganic(int ecoNum, double theType, double theamount, double theDepth, double theTotalN, double theNH4N,
		double theNO3N, double thepercentDM, double theC);
__declspec(dllimport) bool GetMMSDoFertilisation(int patchNo, double *theNH4N, double *theNO3N);
__declspec(dllimport) void SetECOFertilisation(int ecoNum, double theNH4N, double theNO3N);
__declspec(dllimport) double GetMMSAllHouseAmmonia();
__declspec(dllimport) double GetMMSAllStoreAmmonia();
__declspec(dllimport) double GetMMSAllSpreadAmmonia();
__declspec(dllimport) char *GetMyInputDirectory();
__declspec(dllimport) char *GetMyOutputDirectory();
__declspec(dllimport) char *GetMyClimateDirectory();
__declspec(dllimport) char *GetMyClimateFileName();
}


void Initialise()
{
	HarvestProducts = new linkList <harvInfoClass>;
   theType = 0.0;
}

void main(int argc, char* argv[])
{
	char InputDirectory[100];
   char OutputDirectory[100];
   char ClimateDirectory[100];
   char ClimateFileName[100];

   fstream *afile = new fstream("testDLL.txt", fstream::in | fstream::out);
	Initialise();
   initialise_MMS(0);
   int numPatches = GetNumOfPatches();
//   initialise_ECO(numPatches, "c:\\Home\\FASSET_scenarios\\Cropfarm\\R1\\","c:\\Home\\FASSET_scenarios\\Climate\\","foulum.clm",
  // 					"c:\\Home\\FASSET_scenarios\\Cropfarm\\R1\\output", 1,8,1987);
   strcpy(InputDirectory,GetMyInputDirectory());
   strcpy(OutputDirectory,GetMyOutputDirectory());
   strcpy(ClimateDirectory,GetMyClimateDirectory());
   strcpy(ClimateFileName,GetMyClimateFileName());
   initialise_ECO(numPatches, InputDirectory,ClimateDirectory,ClimateFileName,OutputDirectory, 1,8,1987);
   for (int i=0;i<numPatches;i++)
   {
      GetEcoStatus(i, &TotalLAI, &ReadyForHarvestOrLater, &IrrigationDemand, HarvestProducts);
      float prod[4][2];
      for (int i=0; i<2; i++)
      {
         harvInfoClass *aharvInfoClass = HarvestProducts->ElementAtNumber(i);
         prod[0][i]= double (aharvInfoClass->GetproductCode());
         prod[1][i]= aharvInfoClass->GetamountDM();
         if (aharvInfoClass->GetamountDM()>0.0)
         {
	         prod[2][i]= aharvInfoClass->GetamountN() * 100.0/aharvInfoClass->GetamountDM();
	         prod[3][i]= aharvInfoClass->GetamountC() * 100.0/aharvInfoClass->GetamountDM();
			}
         else
         {
         	prod[2][i]= 0.0;
         	prod[3][i]= 0.0;
         }
      }
      SetEcoStatus(i, ReadyForHarvestOrLater, IrrigationDemand,prod);
   }
   bsTime currTime;
   currTime.SetTime(1,8,1987);
   theTime.SetTime(1,8,1987);
   bsTime stopTime;
   stopTime.SetTime(1,8,1989);

   while(currTime < stopTime)
      {
      	if ((theTime.GetDay()==14)&&(theTime.GetMonth()==4)&&(theTime.GetYear()==1988))
         {
          testInt =theTime.GetDay();
         }
      	DoMMSDaily();
         *afile << GetMMSAllHouseAmmonia() << "\t" << GetMMSAllStoreAmmonia() << "\t" << GetMMSAllSpreadAmmonia() << endl;
         for (int i=0;i<numPatches;i++)
         {
				GetMMSTillage(i, &theType, &theDepth, &theFractionIncorporated);
            if (theType!=0)
					SetECOTillage(i, (theType-1.0), theDepth, theFractionIncorporated);
				GetMMSDoSowing(i, &thecropID, &amountSeedDM, &seedN);
            if (amountSeedDM>0.0)
					SetECODoSowing(i, thecropID, amountSeedDM, seedN);
				GetMMSAddOrganic(i, &theManureType, &theManureamount,&theManureDepth,&theManureTotalN,&theManureNH4N, &theManureNO3N,
						&theManurepercentDM, &theManureC);
            if (theManureamount>0.0)
					SetECOAddOrganic(i, theManureType, theManureamount,theManureDepth,theManureTotalN,theManureNH4N, theManureNO3N,
						theManurepercentDM, theManureC);
				GetMMSDoFertilisation(i,  &theFertAmmonium, &theFertNitrate);
            if ((theFertAmmonium>0.0)||(theFertNitrate>0.0))
					SetECOFertilisation(i, theFertAmmonium,theFertNitrate);

				GetMMSDoHarvesting(i, &harvestCode, &propGrainHarvested, &propStrawHarvested);
            if (harvestCode>0)
	            SetECODoHarvesting(i, harvestCode, propGrainHarvested, propStrawHarvested);
         }
         DoDaily_ECO();
         for (int i=0;i<numPatches;i++)
         {
            GetEcoStatus(i, &TotalLAI, &ReadyForHarvestOrLater, &IrrigationDemand, HarvestProducts);
				float prod[4][2];
            for (int i=0; i<2; i++)
            {
               harvInfoClass *aharvInfoClass = HarvestProducts->ElementAtNumber(i);
               prod[0][i]= double (aharvInfoClass->GetproductCode());
               prod[1][i]= aharvInfoClass->GetamountDM();
            if (aharvInfoClass->GetamountDM()>0.0)
            {
               prod[2][i]= aharvInfoClass->GetamountN() * 100.0/aharvInfoClass->GetamountDM();
               prod[3][i]= aharvInfoClass->GetamountC() * 100.0/aharvInfoClass->GetamountDM();
            }
            else
            {
               prod[2][i]= 0.0;
               prod[3][i]= 0.0;
            }
            }
            SetEcoStatus(i, ReadyForHarvestOrLater, IrrigationDemand, prod);
         }
         theTime.AddOneDay();
         currTime.AddOneDay();
      }
	Destroy_MMS();
   Destroy_ECO();
   delete HarvestProducts;
   delete afile;
}

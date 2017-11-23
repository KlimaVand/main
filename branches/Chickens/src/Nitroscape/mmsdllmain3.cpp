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
#include "../base/climate.h"
#include "../event/eventControl.h"
#include "../base/timeuse.h"
#include "../base/controlParameters.h"
#include "../technics/technics.h"
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
#include "base/climate.h"
#include "event/eventControl.h"
#include "base/timeUse.h"
#include "base/controlParameters.h"
#include "technics/technics.h"
#include "tools/fileAccess.h"
#include "data/farmData.h"
#include "data/environData.h"
#include "data/outputData.h"
#include "data/commonData.h"
#include "data/systemData.h"
#endif

#include "../Nitroscape/MMSClass.h"

using namespace std;

//typedef char string100[100];

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

linkList<patchInterfaceClass> * thePatches;

MMSClass * theMMS;

extern "C"
{
    __declspec(dllexport) int GetNumOfPatches()
   {return theMMS->GetNumOfPatches();}

    __declspec(dllexport) void SetEcoStatus(int patchNo, double ReadyForHarvestOrLater,
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

   __declspec(dllexport) void GetMMSTillage(int patchNo, double *theType, double *theDepth, double *theFractionIncorporated)
   {
      theMMS->GetMMSTillage(patchNo, theType, theDepth, theFractionIncorporated);
   }

   __declspec(dllexport) void GetMMSAddOrganic(int patchNo, double *theType, double *theamount, double *theDepth, double *theTotalN, double *theNH4N,
         double *theNO3N, double *thepercentDM, double *theC)
   {
      theMMS->GetMMSAddOrganic(patchNo, theType, theamount, theDepth, theTotalN, theNH4N, theNO3N, thepercentDM, theC);
   }

   __declspec(dllexport) void GetMMSDoFertilisation(int patchNo, double *theNH4N, double *theNO3N)
   {
      theMMS->GetMMSDoFertilisation(patchNo, theNH4N, theNO3N);
   }

   __declspec(dllexport) void GetMMSDoSowing(int patchNo, double *theSeedCode, double *theSeedDM, double *theSeedN)
   {
      theMMS->GetMMSDoSowing(patchNo, theSeedCode, theSeedDM, theSeedN);
   }

   __declspec(dllexport) void GetMMSDoHarvesting(int patchNo, double *theharvestCode, double *thepropGrainHarvested, double *thepropStrawHarvested)
   {
      theMMS->GetMMSDoHarvesting(patchNo, theharvestCode, thepropGrainHarvested, thepropStrawHarvested);
   }


   __declspec(dllexport) void initialise_MMS(int farmNo)
   {
//NJH call to this class constructor leads to input data being read from system.dat (must be in current working directory
		theSystemData =new systemData(true);
//NJH read number of farms - could be replaced by Palm variable
   	int numOfFarms=theSystemData->getRunLevel()+1;
      if (farmNo>numOfFarms)
      	exit(0);
      theMMS = new MMSClass();
	   theMMS->initialise_MMS(theSystemData->getSystemRunData(farmNo));
   }


   __declspec(dllexport) double GetMMSPatchAmmonia(int patchNo)
   {
      return theMMS->GetMMSPatchAmmonia(patchNo);
   }

   __declspec(dllexport) double GetMMSHouseAmmonia(int houseNo)
   {
      return theMMS->GetMMSHouseAmmonia(houseNo);
   }

   __declspec(dllexport) double GetMMSStoreAmmonia(int storeNo)
   {
      return theMMS->GetMMSStoreAmmonia(storeNo);
   }


   __declspec(dllexport) double GetMMSAllSpreadAmmonia()
   {
      return theMMS->GetMMSSpreadAmmonia();
   }

   __declspec(dllexport) double GetMMSAllHouseAmmonia()
   {
      return theMMS->GetMMSHouseAmmonia();
   }

   __declspec(dllexport) double GetMMSAllStoreAmmonia()
   {
      return theMMS->GetMMSStoreAmmonia();
   }


   __declspec(dllexport) int GetNumberOfHouses()
   {
      return theMMS->GetNumberOfHouses();
   }

   __declspec(dllexport)   int GetNumberOfStore()
   {
      return theMMS->GetNumberOfStore();
   }

   __declspec(dllexport) void DoMMSDaily()
   {
      theMMS->zeroFarmOutputVariables();
      theMMS->DoMMSDaily();
   }

   __declspec(dllexport) void Destroy_MMS()
   {
      theMMS->Destroy_MMS();
   }

   __declspec(dllexport) char *GetMyInputDirectory()
   {
      return theMMS->GetMyInputDirectory();
   }
   __declspec(dllexport) char *GetMyOutputDirectory()
   {
      return theMMS->GetMyOutputDirectory();
   }
   __declspec(dllexport) char *GetMyClimateDirectory()
   {
      return theMMS->GetMyClimateDirectory();
   }
   __declspec(dllexport) char *GetMyClimateFileName()
   {
      return theMMS->GetMyClimateFileName();
   }
}

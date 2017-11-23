/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../MMSClass.h"


using namespace std;

typedef double double4[4];
typedef double4 * ptrdouble4;
typedef char string100[100];
bsTime theTime;
products * theProducts;
message * theMessage;
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
/*string100
      InputDirectory,
      OutputDirectory,
      ClimateDirectory,
      ClimateFileName,
      baseOutputDir;
  */

MMSClass * theMMSClass;
extern "C" __declspec(dllexport)
int GetNumOfPatches(){return theMMSClass->GetNumOfPatches();}


void InitialiseInterface(eventControl *aEventControl)
{
	theMMSClass->InitialiseInterface(aEventControl);
}

/*void zeroPatchOutputVariables()
{
	theMMSClass->zeroPatchOutputVariables();
}
  */
void zeroFarmOutputVariables()
{
	theMMSClass->zeroFarmOutputVariables();
}


extern "C" __declspec(dllexport)
void SetEcoStatus(int patchNo, double ReadyForHarvestOrLater,
      double IrrigationDemand, linkList <harvInfoClass> *theHarvestProducts)

{
	theMMSClass->SetEcoStatus(patchNo, ReadyForHarvestOrLater, IrrigationDemand, theHarvestProducts);
}

extern "C" __declspec(dllexport)
void GetMMSTillage(int patchNo, double *theType, double *theDepth, double *theFractionIncorporated)
{
	theMMSClass->GetMMSTillage(patchNo, theType, theDepth, theFractionIncorporated);
}

extern "C" __declspec(dllexport)
void GetMMSAddOrganic(int patchNo, double *theType, double *theamount, double *theDepth, double *theTotalN, double *theNH4N,
		double *theNO3N, double *thepercentDM, double *theC)
{
	theMMSClass->GetMMSAddOrganic(patchNo, theType, theamount, theDepth, theTotalN, theNH4N, theNO3N, thepercentDM, theC);
}

extern "C" __declspec(dllexport)
void GetMMSDoFertilisation(int patchNo, double *theNH4N, double *theNO3N)
{
	theMMSClass->GetMMSDoFertilisation(patchNo, theNH4N, theNO3N);
}

extern "C" __declspec(dllexport)
void GetMMSDoSowing(int patchNo, double *theSeedCode, double *theSeedDM, double *theSeedN)
{
	theMMSClass->GetMMSDoSowing(patchNo, theSeedCode, theSeedDM, theSeedN);
}

extern "C" __declspec(dllexport)
void GetMMSDoHarvesting(int patchNo, double *theharvestCode, double *thepropGrainHarvested, double *thepropStrawHarvested)
{
	theMMSClass->GetMMSDoHarvesting(patchNo, theharvestCode, thepropGrainHarvested, thepropStrawHarvested);
}


/****************************************************************************\
\****************************************************************************/
extern "C" __declspec(dllexport)
void initialise_MMS(std::string inputDirectory, std::string climateDirectory, std::string climateFileName, std::string outputDirectory)
{
	theMMSClass->initialise_MMS(inputDirectory, climateDirectory, climateFileName, outputDirectory);
}

extern "C" __declspec(dllexport)
void DoMMSDaily()
{
	theMMSClass->DoMMSDaily();
}

extern "C" __declspec(dllexport)
void Destroy_MMS()
{
	theMMSClass->Destroy_MMS();
}



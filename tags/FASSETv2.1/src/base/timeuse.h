/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 Class: timeUse
(c) J�rgen Berntsen, Danmarks JordbrugsForskning
E-mail: Jorgen.Berntsen@agrsci.dk
class used for profiling the time consumption of different functions
\****************************************************************************/

#ifndef __timeUse_H
   #define __timeUse_H
#ifdef __ANSICPP__
	#include <ctime>
#else
	#include <time.h>
#endif
enum {
   LP1Simulation,
   LP3Simulation,
   CroprotSimulation,
   UpdateFieldSimulation,
   UpdateFieldIndicators,
   PatchOutput,
   PatchIndicators,
   HandleOperationsSimulation,
   PatchDynamicsSimulation,
   DailyGrazerPatch,
   DailyGrazerPatch1,
   PatchDaily,
   DailyCropRot,
   updateSoil,
   waterSimulation,
   temperatureSimulation,
   OrganicMatterSimulation,
   PlantTranspSimulation,
   N_uptakeSimulation,
   AddIndicatorToOutput
};

class timeUse
{
   private:
   	enum {MaxNumberOftimeUse=50};

      fstream f;
      int calls[MaxNumberOftimeUse];
      clock_t startTime[MaxNumberOftimeUse];
      clock_t usedTime[MaxNumberOftimeUse];

   public:
      timeUse();
      ~timeUse();
      void startProfiling(int timeUseNumber);
      void endProfiling(int timeUseNumber);
      void outputtimeUse();
};

extern timeUse * timeConsumption;

#endif



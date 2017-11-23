/*
 * timeUsed.h
 *
 *  Created on: 27/01/2010
 *      Author: beer
 */

#ifndef TIMEUSED_H_
#define TIMEUSED_H_

#ifndef __BCplusplus__
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


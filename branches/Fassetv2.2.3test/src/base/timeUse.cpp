/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "timeUse.h"
#include "bstime.h"
#include "message.h"

/****************************************************************************\
  Constructor
\****************************************************************************/
timeUse::timeUse()
{
   for (int i=0; i<MaxNumberOftimeUse; i++)
   {
   	startTime[i] = clock_t(0);
    usedTime[i]  = clock_t(0);
      calls[i] = 0;
   }
#ifdef __BCplusplus__
   f.open("\\fasset\\timeUse.txt",ios::out);
#else
   f.open("timeUse.txt",ios::out);
#endif
   f.precision(4);
}


/****************************************************************************\
  Destructor
\****************************************************************************/
timeUse::~timeUse()
{
	f.close();
}

void timeUse::startProfiling(int profileNumber)
{
	startTime[profileNumber] = clock();
   calls[profileNumber]++;
}

void timeUse::endProfiling(int profileNumber)
{
	usedTime[profileNumber] += (clock()-startTime[profileNumber]);
}

void timeUse::outputtimeUse()
{
   f << "------------------------------------------------------------------" << endl;
   f << "                                " << theTime.GetYear() << endl;
   f << "------------------------------------------------------------------" << endl;

	for (int i=0; i<MaxNumberOftimeUse; i++)
   {
   	if (usedTime[i]>0.0)
      {
      	string outputString;
         switch (i)
         {
				case CroprotSimulation: outputString          = "Croprot total    "; break;
	         case LP1Simulation: outputString              = "LP period 1 "; break;
	         case LP3Simulation: outputString              = "LP period 3 "; break;
            case UpdateFieldSimulation: outputString      = " Update field:    "; break;
            case UpdateFieldIndicators: outputString      = "  Field indicators: "; break;
   			case PatchOutput: outputString                = "  Patch output:   "; break;
   			case PatchIndicators: outputString            = "   Patch Indicators:   "; break;
            case HandleOperationsSimulation: outputString = "  HandleOperations:   "; break;
            case PatchDynamicsSimulation: outputString    = "  PatchDynamicsSimulation: "; break;
          	case PatchDaily: outputString                 = "   PatchDaily:  "; break;
				case updateSoil: outputString                 = "    UpdateSoil  "; break;
				case waterSimulation: outputString            = "     Water  "; break;
				case temperatureSimulation: outputString      = "     Temperature "; break;
	         case OrganicMatterSimulation: outputString    = "     Organic matter "; break;
	         case PlantTranspSimulation: outputString      = "     Plant transpiration: "; break;
	         case N_uptakeSimulation: outputString         = "     N uptake: "; break;
            case DailyGrazerPatch: outputString           = "  DailyGrazerPatch:  "; break;
            case DailyGrazerPatch1: outputString          = "   DailyGrazerPatch1: "; break;
				case AddIndicatorToOutput: outputString       = "Indicator output: "; break;
            default: outputString                         = "Unknown profile: "; break;
         }
         char out[80];
#ifndef __BCplusplus__
         float timeUse = usedTime[i]/double(CLOCKS_PER_SEC);
#else
         float timeUse = usedTime[i]/CLK_TCK;
#endif
         if (calls[i]==0)
            timeUse=0.0;
         sprintf(out, "%-30s  calls: %7i  time: %7.1f ", (char*) outputString.c_str(),calls[i],timeUse );

         f << out << endl;
		}
      startTime[i] = clock_t(0);
      usedTime[i]  = clock_t(0);
      calls[i] = 0;
   }
}

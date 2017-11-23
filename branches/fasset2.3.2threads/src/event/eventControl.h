/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __EVENT_H
   #define __EVENT_H

#include "../base/bstime.h"
#include "../economy/economics.h"
#include "../technics/technics.h"

#include "../manager/operationsManager.h"
#include "../manager/manureManager.h"
#include "../fields/cropRotation.h"
#include "../livestock/livestock.h"
#include "../products/products.h"
#include "../base/settings.h"
#include "../building/thebuild.h"
#include "../base/budget.h"
#include "../base/id.h"
#include <thread>
class eventControl: public base
{
   /* Attributes */
   private:
	commonData systemData;
	int run;
	bool removeOldData;
	string InputDirectory1;
	string OutputDirectory1;
	string ClimateDirectory;
	string ClimateFileName;
   	livestock * theLivestock;
   	string EconomicIndicatorDate;
   	string EnvironmentalIndicatorDate;
      cropRotation * theCropRotation;
//      manager * theManager;
      operationsManager *theOperational_manager;
	   int ScaleBuildings;
	   bsTime stopTime;
		budget Nbudget;
		bool cleanUpCalled;

      /* Actions */
      void InitVariables();



   public:
      void operator()();

      // Default Constructor
      void print();
      eventControl();
      eventControl(const eventControl& aEventControl);
     // Destructor
      ~eventControl();
      // Other functions
      void Initialize(bsTime * stopTime, char* inputDir, char * climateDir, char * climateFilename, char * outputPath);
      void ReceivePlan(char * fileExtension, char *inputDir);

         void DailyUpdate();
         void Start(void);
         void Settup(commonData data, int index, bool remove);
         void CleanUp();

      void Simulation(int runNumber, bsTime stopTime,
                      char * inputDir, char * outputDir, char * climateDir,
                      char * climateFilename, string economicIndicatorDate, string environmentalIndicatorDate);

      void GiveIndicator(indicatorTypes indicatorType, int yearNumber);
};

#endif



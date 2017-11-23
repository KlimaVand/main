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
#include "../data/systemData.h"
#include "../building/thebuild.h"
#include "../base/budget.h"


class eventControl: public base
{
   /* Attributes */
   private:
   	livestock * theLivestock;

      cropRotation * theCropRotation;
//      manager * theManager;
      operationsManager *theOperational_manager;
	   int ScaleBuildings;

		budget Nbudget;


      /* Actions */
      void InitVariables();



   public:
      // Default Constructor
      eventControl();
     // Destructor
      ~eventControl();
      // Other functions
      void Initialize(bsTime * stopTime, char* inputDir, char * climateDir, char * climateFilename, char * outputPath);
      void ReceivePlan(char * fileExtension, char *inputDir);

         void DailyUpdate();


      void Simulation(int runNumber, bsTime stopTime,
                      char * inputDir, char * outputDir, char * climateDir,
                      char * climateFilename, string economicIndicatorDate, string environmentalIndicatorDate);

      void GiveIndicator(indicatorTypes indicatorType, int yearNumber);
};

#endif



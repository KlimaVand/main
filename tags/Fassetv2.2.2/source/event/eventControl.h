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
#include "../legislation/legislation.h"
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
      double CapitalInterest;
      double MinFieldArea;
	   int ScaleBuildings;
      int AnimalModel;
		budget Nbudget;
		bool abort;

      /* Actions */
      void InitVariables();

      void GiveIndicator(indicatorTypes indicatorType, int yearNumber);
      void RenameFile(string oldFileName,string newFileName, string extension, char * outputDir);
      void CopyFile(const char* inputFileName, const char* outputFileName,const char* inputDir,const char* outputDir);
      void RenameFiles(char * outputDir, char* inputDir, int GenerateFixedPlans, int yearNumber);
      void BasicLPParameters();
      void CalcLP(int periods);

   public:
      // Default Constructor
      eventControl();
      // Constructor with arguments
      eventControl(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Destructor
      ~eventControl();
      // Other functions
      void Initialize(bsTime * stopTime, char* inputDir, char * climateDir, char * climateFilename, char * outputPath);
      void ReceivePlan(char * fileExtension, char *inputDir);
         void DailyUpdate();
      cropRotation * GettheCropRotation(){return theCropRotation;};
      void TestGams(bsTime stopTime, char * inputDir, char * outputDir, char * climateFilename);
      void Simulation(int runNumber, bsTime stopTime, int useGams, int NumberOfFixedYears, int GenerateFixedPlans,
                      char * inputDir, char * outputDir, char * climateDir,
                      char * climateFilename, string economicIndicatorDate, string environmentalIndicatorDate);
      void EndSimulation(bool show = false);
};

#endif



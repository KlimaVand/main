/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
   Author    J�rgen Berntsen
     Date    13/5-97
 Revision    20/1-97 DailyUpdate added
 Swinestk is necessary because the input to the model is number of pigs produced per
 year and this must be adjusted to a daily number, based on the length of the production
 cycle.
\****************************************************************************/

#ifndef SWINESTK_H
  #define SWINESTK_H

#include "animalStock.h"
#include "swine.h"
#include "slaughterSwine.h"
#include "../building/PigHousing.h"
#include "../products/animalProduct.h"
#include "../building/thebuild.h"

/****************************************************************************\
 Class: swinestock
 A swine stock generally has an instance of a swine and a slaughterSwine, representing
 all swines and baconers in the swine stock. Most functions are called
 from an instance of livestock.
\****************************************************************************/

class swineStock  : public animalStock
{
   /* Attributes */
   protected:
		swine * pregnantSwines;
   	swine *lactatingSwines;
   	swine* matingSwines;
      slaughterSwine* piglets;
      slaughterSwine* baconers;
      slaughterSwine* newSows;
      PigHousing* aStable;
      animalProduct* weanersInOutPrDay;
      animalProduct* pigletsInOutPrDay;
      animalProduct* pigletsPrDay;
      animalProduct* baconersInOutPrDay;
      animalProduct* baconersPrDay;
      animalProduct* newsowsInOutPrDay;
      animalProduct* newsowsPrDay;  //holds details of the daily amounts to be exported
      animalProduct* castsowsPrDay;
		linkList <pig> * pointyPigs;  //holds pointers to the pigs - used when operations done on all pigs
		double SowProductionCycle;    //days between successive births
   /* Actions */
   private:
		swineStock& operator=(const swineStock& a); // Dissable the compilers generation of default assignment operator.
		swineStock(const swineStock& a);            // Dissable the compilers generation of default copy constructor.
      // Constructor
      swineStock(){};

      // write function for Gams
      void writeYearPlaces(char* filename);
      void writeFeedComponents(char* filename, int produced);
      void writeFeedMin(char* filename);
   	void writeFeedMax(char* filename);
		void writeSwineYield(char* filename);
		void writeManureProduction(char* filename);
      void writeManureNPK(char* filename);
      void writeSwineFeed(char* filename);
      void writeNotProducablePrice(char* filename);
      void writeFeedPercentages(char* filename, int produced);
      void writePigLab(char* filename);

   public:
      // Constructor with arguments
      swineStock(const char *aName,const int aIndex,const base * aOwner);
      // Destructor
      ~swineStock();

      // Set functions
      void SetStable(PigHousing* aaStable) {aStable = aaStable;};

      // Get functions
      PigHousing* GetStable() {return aStable;};
      double GetNumberOfpregnantSwines() {return pregnantSwines->GetNumberPrYear();};
      double GetNumberOflactatingSwines() {return lactatingSwines->GetNumberPrYear();};
      double GetNumberOfmatingSwines() {return matingSwines->GetNumberPrYear();};
      double GetNumberOfSwines() {return GetNumberOfpregnantSwines() +
                           GetNumberOflactatingSwines() + GetNumberOfmatingSwines();};
		double GetNumberOfbaconers() {return baconers->GetNumberPrYear();};
		double GetNumberOfPiglets() {return piglets->GetNumberPrYear();};

      double GetNumber();
      double GetDE() {return pregnantSwines->GetDE()+lactatingSwines->GetDE()+matingSwines->GetDE()
                         +piglets->GetDE() + baconers->GetDE();};

      void DailyUpdate();
      void ReceivePlan(char* fileName);
      void ReceiveFeed(char* fileName);

		void InitSwines(fstream * file, buildings* theBuildings);
		void CheckNbalance();
      void ExportStock();
		virtual void EndBudget(bool show = false);
      double GetAnimalN() {return 0.0;}
      animalProduct * GetStockBought();
      animalProduct * GetStockSold();
};

#endif



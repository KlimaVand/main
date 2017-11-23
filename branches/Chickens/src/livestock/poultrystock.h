/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

#ifndef SWINESTK_H
  #define SWINESTK_H

#include "animalStock.h"
#include "poultry.h"
#include "../building/poultryhouse.h"
#include "../products/animalProduct.h"
#include "../building/thebuild.h"

/****************************************************************************\
\****************************************************************************/

class poultryStock  : public animalStock
{
   /* Attributes */
   protected:
      poultryhouse* aHouse;
      poultry * theHens;
      poultry * theBroilers;
      poultry * thePullets;
      animalProduct* pulletsPrDay;
      animalProduct* pulletsInOutPrDay;
      animalProduct* broilersInOutPrDay;
      animalProduct* broilersPrDay;
      animalProduct* hensInOutPrDay;
      animalProduct* hensPrDay;  //holds details of the daily amounts to be exported
      linkList <poultry> * pointyPoultry;  //holds pointers to the pigs - used when operations done on all pigs
      double PoultryProductionCycle;    //days between successive births
		double henProductionCycle;
		double pulletProductionCycle;
		double broilersProductionCycle;
   /* Actions */
   private:
		poultryStock& operator=(const poultryStock& a); // Dissable the compilers generation of default assignment operator.
		poultryStock(const poultryStock& a);            // Dissable the compilers generation of default copy constructor.
      // Constructor
      poultryStock(){};

   public:
      // Constructor with arguments
      poultryStock(const char *aName,const int aIndex,const base * aOwner);
      // Destructor
      ~poultryStock();

      // Set functions
      //void SetStable(pigstable* aaStable) {aStable = aaStable;};

      // Get functions
      poultryhouse* GetStable() {return aHouse;};
      double GetNumberOfHens() {return theHens->GetNumberPrYear();};
      double GetNumberOfPullets() {return thePullets->GetNumberPrYear();};
      double GetNumberOfbroilers() {return theBroilers->GetNumberPrYear();};
      double GetNumber();
//      double GetDE() {return pregnantpoultrys->GetDE()+lactatingpoultrys->GetDE()+matingpoultrys->GetDE()
  //                       +piglets->GetDE() + broilers->GetDE();};

      void DailyUpdate();
      void ReceivePlan(char* fileName);
      void ReceiveFeed(char* fileName);

      void Initpoultry(fstream * file, buildings* theBuildings);
      void CheckNbalance();
      void ExportStock();
      virtual void EndBudget(bool show = false);
      double GetAnimalN() {return 0.0;}
      animalProduct * GetStockBought();
      animalProduct * GetStockSold();
		void InitPoultry(fstream * file, buildings* theBuildings);
};

#endif



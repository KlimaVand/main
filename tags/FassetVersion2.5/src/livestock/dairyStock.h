#ifndef DAIRYSTK_H
  #define DAIRYSTK_H

#include "animalStock.h"
#include "cattle.h"
#include "../products/animalProduct.h"
#include "cattleGroup.h"
#include "../building/buildings.h"

/****************************************************************************\
\****************************************************************************/

class dairyStock  : public animalStock
{
   /* Attributes */
   protected:
      linkList <cattleGroup> * grouplist;
		linkList <cattle> * pointyCalves;  //holds pointers to the calves produced today
      linkList<cattle> * oldies;				//holds pointers to the animals that have out-aged their age group

		int maxNumPerCohort;   //

public:
      int IDnumber;     // to identify individuals

   /* Actions */
   private:
		dairyStock& operator=(const dairyStock& a); // Dissable the compilers generation of default assignment operator.

      // Constructor
      dairyStock(){
    	  grouplist = new linkList<cattleGroup>;
    	  pointyCalves = new linkList<cattle>;
    	  oldies = new linkList<cattle>;
      };

   public:
      dairyStock(const dairyStock& a);
      // Constructor with arguments
      dairyStock(const char *aName,const int aIndex,const base * aOwner);
      linkList<cattleGroup> *GetGrouplistClone();
      linkList<animal> *GetTheStockClone();
      linkList<cattle> *GetPointyCalvesClone();
      linkList<cattle> *GetOldiesClone();
      // Destructor
     virtual  ~dairyStock();

      // Get functions

      double GetDE();
		void ReceivePlan(char* fileExtension);
      void DailyUpdate();
		void InitCattle(commonData * &file, int anAnimalModel);

      linkList <cattleGroup> * GetCattleGroup() const {return grouplist;};
      void CheckBirth();
		void CheckAge();
		void SeedGroups();
      void Cull(linkList<cattle> * victims);
		double GetTotalAnimals();
		void AutoRecruit();
		void GiveIndicator(int indicatorType);
		void CheckWean(bool stopLactation);
      double GetHerdN();
      double GetHerdC();
		void theSorting(bool canCull);
		virtual  void dairyStockcheckBalances(bool show);
	  //! send animal info on manure and heat production
	  void InformBuildings();
};
#endif



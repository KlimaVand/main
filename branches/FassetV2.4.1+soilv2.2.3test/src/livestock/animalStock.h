/****************************************************************************\
  Archive
   Author    Jørgen Berntsen
     Date    13/5-97
 Revision    20/1-97 DailyUpdate added
 ----------------------------------------------------------------------------
      Log
\****************************************************************************/

#ifndef ANIMSTK_H
  #define ANIMSTK_H

#include "../building/stable.h"
#include "../base/budget.h"
#include "../livestock/animal.h"
#include "../base/cloneList.h"

/****************************************************************************\
 Class: animalStock
 SuperClass of all animals stocks. This class should not be instanciated.
 Each animal stock has a name and a stable.
\****************************************************************************/

class animalStock : public base
{
   /* Attributes */
   public:

   linkList <animal> * theStock;

	budget Nbudget;            // budgets
   public:

   /* Actions */
   private:
		animalStock& operator=(const animalStock& a); // Dissable the compilers generation of default assignment operator.
   public:
		animalStock(const animalStock& a);            // Dissable the compilers generation of default copy constructor.
   public:
      // Default Constructor
      animalStock();
      animalStock(const char *aName,const int aIndex,const base * aOwner);
      // Destructor
      virtual ~animalStock(); 		// a virtual destructor so that a pointer to a derived class calls the correct destructor

      virtual void DailyUpdate(){};
      virtual void Initialize();
      virtual void ReceivePlan(char* fileExtension){};




      // Get functions

      double GetNumber() {return 0.0;};
      virtual double GetDE() {return 0.0;};
      virtual void checkBalances(bool show = false);
		virtual void GiveIndicator(int indicatorType) {};
      virtual void FlushIndicator(int indicatorType){};
	  virtual void InformBuildings(){};
	  virtual  void dairyStockcheckBalances(bool show){theMessage->FatalError("animalStock::dairyStockcheckBalances- should only be call from dairyStock");};
};

#endif



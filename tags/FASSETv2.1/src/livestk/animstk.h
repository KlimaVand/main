/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
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

#include <stable.h>
#include "budget.h"
#include <animal.h>
#include <clonlist.h>

/****************************************************************************\
 Class: animalStock
 SuperClass of all animals stocks. This class should not be instanciated.
 Each animal stock has a name and a stable.
\****************************************************************************/

class animalStock : public base
{
   /* Attributes */
   protected:

   linkList <animal> * theStock;
   stable* aStable;
	budget Nbudget;            // budgets
   public:

   /* Actions */
   private:
		animalStock& operator=(const animalStock& a); // Dissable the compilers generation of default assignment operator.
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
      virtual void CalcLP1(){};
      virtual void CalcLP3(){};

      // Set functions
      void SetStable(stable* aaStable) {aStable = aaStable;};

      // Get functions
      stable* GetStable() {return aStable;};
      double GetNumber() {return 0.0;};
      virtual double GetDE() {return 0.0;};
      virtual void checkBalances(bool show = false);
		virtual void GiveIndicator(int indicatorType) {};
      virtual void FlushIndicator(int indicatorType){};
};

#endif



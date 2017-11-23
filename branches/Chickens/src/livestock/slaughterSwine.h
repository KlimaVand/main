/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Class "slaughterSwine"
Class for the slaughterSwine object. There is only one object for all slaughterSwines.
(c) Jørgen Berntsen, Danmarks JordbrugsForskning.
First version: 13/5-97
Revised: 16/5
NJH - Jan 2000 revised to cover piglets, finishers and replacement sows. Separate instance for each. Also reads data from file
(livestock.dat) rather than via hardcoding.
\****************************************************************************/

#ifndef slaughterSwine_H
  #define slaughterSwine_H

class slaughterSwine;

#include "pig.h"
#include "../products/animalProduct.h"

/****************************************************************************\
 Class: slaughterSwine
\****************************************************************************/

class slaughterSwine : public pig
{
   /* Attributes */
   private:

    animalProduct* slaughterSwinePrDay;
   /* Actions */
   private:


   public:
      // Default Constructor
      slaughterSwine();
      slaughterSwine(const char *aName,const int aIndex,const base * aOwner);
      // Destructor
      ~slaughterSwine();

      // Other functions

      void DailyUpdate();
      void ReceivePlan(char * FileName);
//      double GetDE() {return NumberPrYear/30.0;};
      void SetNumberPrYear (double aNumberPrYear){ pig::SetNumberPrYear(aNumberPrYear);}
		virtual double GetSensibleHeatProduction(double weight, double n);
};



#endif



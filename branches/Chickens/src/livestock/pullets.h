/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
\****************************************************************************/

#ifndef PULLETS_H
  #define PULLETS_H

#include "poultry.h"
#include "../products/animalProduct.h"

/****************************************************************************\
 Class: pullets
\****************************************************************************/

class pullets : public poultry
{
   /* Attributes */
   private:

    animalProduct* pulletsPrDay;
   /* Actions */
   private:


   public:
      // Default Constructor
      pullets();
      pullets(const char *aName,const int aIndex,const base * aOwner);
      // Destructor
      ~pullets();

      // Other functions
      double feedingDays();

      void Manure(manure* fluidManure, manure* solidManure);
      void DailyUpdate();
      void ReceivePlan(char * FileName);
      void SetNumberPrYear (double aNumberPrYear){ poultry::SetNumberPrYear(aNumberPrYear);}
		virtual double GetSensibleHeatProduction(double weight, double n);
};



#endif



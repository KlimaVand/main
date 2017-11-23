/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
\****************************************************************************/

#ifndef BROILERS_H
  #define BROILERS_H

#include "poultry.h"
#include "../products/animalProduct.h"

/****************************************************************************\
 Class: broilers
\****************************************************************************/

class broilers : public poultry
{
   /* Attributes */
   private:

    animalProduct* broilersPrDay;
   /* Actions */
   private:


   public:
      // Default Constructor
      broilers();
      broilers(const char *aName,const int aIndex,const base * aOwner);
      // Destructor
      ~broilers();

      // Other functions
      double feedingDays();

      void Manure(manure* fluidManure, manure* solidManure);
      void DailyUpdate();
      void ReceivePlan(char * FileName);
      void SetNumberPrYear (double aNumberPrYear){ poultry::SetNumberPrYear(aNumberPrYear);}
		virtual double GetSensibleHeatProduction(double weight, double n);
};



#endif



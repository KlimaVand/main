/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
\****************************************************************************/

#ifndef HENS_H
  #define HENS_H

#include "poultry.h"

/****************************************************************************\
 Class: hens
\****************************************************************************/

class hens : public poultry
{
   /* Attributes */
   private:
    double eggsPerBird;
    double eggN;
    double eggC;
   private:

   public:
      // Default Constructor
      hens();
      hens(const char *aName,const int aIndex,const base * aOwner);
      // Destructor
      ~hens();

		// Other functions
      void Manure(manure* fluidManure, manure* solidManure);
      void DailyUpdate();
      void ReceivePlan(char * FileName);
      virtual double GetEggsPerBird() {return eggsPerBird;};
      void  SetEggsPerBird(double eggsPerYear) {eggsPerBird = eggsPerYear;};
		virtual void ReadParameters(fstream * file);
		double GetSensibleHeatProduction(double weight, double n);
	   virtual void SeteggN(double aVal);
   	virtual void SeteggC(double aVal);
};



#endif



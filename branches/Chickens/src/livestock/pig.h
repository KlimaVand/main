/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

// Class: pig                  //Borland C++ 5.02 - FASSET style
#define NT     //stops FASSET files from redefining Boolean

#ifndef __pig_H  //Required for current class
  #define __pig_H

#include "../base/linklist.h"
#include "../base/common.h"
#include "animal.h" //Required for base class
#include "../building/stable.h" //Required for include files
 #include "../products/feedItem.h" //Required for include files
 #include "../products/manure.h" //Required for include files
#include "../building/pigstysection.h"

class pig: public animal
{
 protected:
 double NumberPrDay;
 double NumberPrYear;

 double aveWeight;
 linkList <product> *feedItemList;

  pig (){};// :animal();
public:
			//Constructor setting attributes to default values
  pig (const char *aName,const int aIndex,const base * aOwner);
  virtual ~ pig ( ); //Destructor

// Get and set functions for attributes
   double GetNumberPrDay() const {return NumberPrDay;}
   double GetNumberPrYear() const {return NumberPrYear;}

   void SetNumberPrDay (double aNumberPrDay){ NumberPrDay = aNumberPrDay;}
   void SetNumberPrYear (double aNumberPrYear){ NumberPrYear = aNumberPrYear;}
   void SetaveWeight(double aVal) {aveWeight=aVal;};
   double GetaveWeight() {return aveWeight;};
   virtual double GetDE() {return NumberPrYear * LivestockUnits;};
   //overload animal class functions
   //virtual void SetStableSection (pigstysection* aStableSection){ StableSection = aStableSection;}
   virtual pigstysection* GetStableSection () { return (pigstysection*) StableSection;}

   //void SetdailyWeightGain(double adailyWeightGain) {dailyWeightGain = adailyWeightGain;}
   //user created functions
   virtual void ReadParameters(fstream * file);
   virtual void DailyUpdate ()  ;

   void ReceivePlan (char* fileName)  ;

   double ProduceManure( manure* fluidManure,  manure* solidManure);
     //GAMs functions

   virtual void EndBudget(bool show = false);


	virtual double GetSensibleHeatProduction(double weight, double n){return 0.0;};
   double CalcMaintenanceEnergy(double weight);
};
#endif

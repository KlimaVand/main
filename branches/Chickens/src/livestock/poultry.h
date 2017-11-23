/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

// Class: poultry                  //Borland C++ 5.02 - FASSET style
#define NT     //stops FASSET files from redefining Boolean

#ifndef POULTRY_H  //Required for current class
  #define POULTRY_H

#include "../base/linklist.h"
#include "../base/common.h"
#include "animal.h" //Required for base class
#include "../building/stable.h" //Required for include files
 #include "../products/feedItem.h" //Required for include files
 #include "../products/manure.h" //Required for include files
//#include "../building/poultrystysection.h"

class poultry: public animal
{
 protected:
 double NumberPrDay;
 double NumberPrYear;
  double aveWeight;
 linkList <product> *feedItemList;

  poultry (){};// :animal();
public:
			//Constructor setting attributes to default values
  poultry (const char *aName,const int aIndex,const base * aOwner);
  virtual ~ poultry ( ); //Destructor

// Get and set functions for attributes
   double GetNumberPrDay() const {return NumberPrDay;}
   double GetNumberPrYear() const {return NumberPrYear;}

   void SetNumberPrDay (double aNumberPrDay){ NumberPrDay = aNumberPrDay;}
   void SetNumberPrYear (double aNumberPrYear){ NumberPrYear = aNumberPrYear;}
   void SetaveWeight(double aVal) {aveWeight=aVal;};
   double GetaveWeight() {return aveWeight;};
   virtual double GetDE() {return NumberPrYear * LivestockUnits;};
   //overload animal class functions
   //virtual void SetStableSection (poultrystysection* aStableSection){ StableSection = aStableSection;}
   //virtual poultrystysection* GetStableSection () { return (poultrystysection*) StableSection;}

   //void SetdailyWeightGain(double adailyWeightGain) {dailyWeightGain = adailyWeightGain;}
   //user created functions
   virtual void ReadParameters(fstream * file);
   virtual void DailyUpdate ()  ;
//   void GetExtraFeed(double NRequired);
   void ReceivePlan (char* fileName)  ;
   virtual double feedingDays () {return theTime.daysInYear(1);} ;
   double ProduceManure( manure* fluidManure,  manure* solidManure);
     //GAMs functions
//   void ManureProduction ( fstream* fileStream)  ;
//   void FeedMax ( fstream* fileStream)  ;
//   void FeedMin ( fstream* fileStream)  ;
   virtual void EndBudget(bool show = false);
   bool TruncateName(string * ret_string);
   feedItem * GetFeedItem(product * menuPtr);

	virtual double GetSensibleHeatProduction(double weight, double n){return 0.0;};
   double CalcMaintenanceEnergy(double weight);
   virtual double GetEggsPerBird(){return 0.0;}
   virtual void SeteggN(double aVal);
   virtual void SeteggC(double aVal);
};
#endif

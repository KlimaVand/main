/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
  Archive
   Author Lars Kroll lokal 1875
     Date 21/5 97
 Revision
 ----------------------------------------------------------------------------
      Log
\****************************************************************************/

#ifndef PIGSTYSECTION_H
	#define PIGSTYSECTION_H

#include "genAsset.h"
//#include "PigStyTypes.h"
#include "animSec.h"

class pigstysection: public animalSection
{
	double Capacity;
   double VariableFeeding;
   double VariableCleaning;
   double ConstantFeeding;
   double ConstantCleaning;
   double variablePeriodical;
   double constantPeriodical;
   double numberOfAnimalsPrBatch;
   double batchInterval;
   int DaysInSection;
   double numberPigsPerSty;

	public:
      pigstysection(char* aName,const int aIndex,const base* aOwner);
	   virtual ~pigstysection();

	public:

   //set functions
	void setCapacity(double aCap) {Capacity=aCap;};
   void setBatchInteval(double aInterval) {batchInterval = aInterval;};
   void setnumberOfAnimalsPrBatch(double aNumber) {numberOfAnimalsPrBatch = aNumber;};
	//get functions
   double GetLabour();
   double GetDaysInSection() {return DaysInSection;};
	double GetCapacity(double ProductionCyclus);

	void ReadParameters(fstream* file);

};

#endif

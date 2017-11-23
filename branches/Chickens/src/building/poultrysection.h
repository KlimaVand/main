/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

#ifndef POULTRYSECTION_H
	#define POULTRYSECTION_H

#include "../economy/genAsset.h"
//#include "poultryTypes.h"
#include "animalSection.h"

class poultrysection: public animalSection
{
	double Capacity;
   double numberOfAnimalsPrBatch;
   double batchInterval;
   int DaysInSection;

	public:
      poultrysection(char* aName,const int aIndex,const base* aOwner);
	   virtual ~poultrysection();

	public:

   //set functions
	void setCapacity(double aCap) {Capacity=aCap;};
   void setBatchInteval(double aInterval) {batchInterval = aInterval;};
   void setnumberOfAnimalsPrBatch(double aNumber) {numberOfAnimalsPrBatch = aNumber;};
	//get functions
   double GetLabour(){return 0.0;};
   double GetDaysInSection() {return DaysInSection;};
	double GetCapacity(double ProductionCyclus);

	void ReadParameters(fstream* file);

};

#endif

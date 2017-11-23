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

#ifndef PIGSTABLE_H
	#define PIGSTABLE_H

#include "../base/common.h"
#include "stable.h"
#include "pigstysection.h"
class swineStock;

class pigstable :public stable
{
	swineStock* Livestock;
   double batchInterval;

   public:
   pigstable(char* aname,const int aIndex,const base* aOwner,
          	 animalStock* alivestock=NULL);
   virtual ~pigstable();

//   virtual double GetAreaUsed();                     //overrides stable::GetAreaUsed()
	virtual string GetAnimalType();
   double GetLabour();
   double GetSowsProductionCyclus();
	double GetBaconersProductionCyclus();
   double GetSowsCapacity();
   double GetBaconersCapacity();
   void SetLivestock(swineStock* aLivestock);

   virtual void DailyUpdate();
   virtual void ReadParameters(fstream* file);
	virtual void CalcLP();
	virtual void GiveIndicator(int indicatorType);
   virtual bool CapacityOverflow();
   void PutStockInSection();
   virtual double CalcMinimumVentilationRate();
   virtual double CalcMaximumVentilationRate();
};

#endif

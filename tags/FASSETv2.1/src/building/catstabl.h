/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
\****************************************************************************/

#ifndef CATSTABLE_H
	#define CATSTABLE_H

#include <common.h>
#include "stable.h"
class dairyStock;

class catstable :public stable
{
	dairyStock* Livestock;

   public:
   catstable(char* aname,const int aIndex,const base* aOwner,
          	 animalStock* alivestock=NULL);
   ~catstable();

	string GetAnimalType();
   void SetLivestock(dairyStock* aLivestock);

   virtual void DailyUpdate();
   virtual void ReadParameters(fstream* file);
	virtual void CalcLP();
	virtual void GiveIndicator(int indicatorType);
   virtual bool CapacityOverflow();
   void PutStockInSection();
};

#endif

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

#include "../base/common.h"
#include "stable.h"
class dairyStock;

class cattlehouse :public stable
{
	dairyStock* Livestock;

   public:
   cattlehouse(char* aname,const int aIndex,const base* aOwner,
          	 animalStock* alivestock=NULL);
   ~cattlehouse();

	string GetAnimalType();
   void SetLivestock(dairyStock* aLivestock);

   virtual void DailyUpdate();
   virtual void ReadParameters(fstream* file);
	virtual void CalcLP();
	virtual void GiveIndicator(int indicatorType);
   void PutStockInSection();
};

#endif

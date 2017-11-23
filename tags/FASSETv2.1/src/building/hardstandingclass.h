/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
\****************************************************************************/

#ifndef HARDSTAND_H
	#define HARDSTAND_H

#include <common.h>
#include "stable.h"
class dairyStock;

class hardStandingClass :public stable
{
	dairyStock* Livestock;

   public:
   hardStandingClass(char* aname,const int aIndex,const base* aOwner,
          	 animalStock* alivestock=NULL);
   ~hardStandingClass();

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

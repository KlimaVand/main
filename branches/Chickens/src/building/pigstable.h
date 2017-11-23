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



   public:
   pigstable(char* aname,const int aIndex,const base* aOwner,
          	 animalStock* alivestock=NULL);
   pigstable(){};
   virtual ~pigstable();

   virtual void DailyUpdate();
   virtual void ReadParameters(fstream* file);

   virtual void ReadDefaultParameters();

   virtual double CalcMinimumVentilationRate();
   virtual double CalcMaximumVentilationRate();
};

#endif

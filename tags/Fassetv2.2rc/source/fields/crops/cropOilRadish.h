/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef cropOilRadishish_h
  #define cropOilRadishish_h

#include "../../base/cloneList.h"
#include "cropBeet.h"

class cropOilRadish: public cropBeet
{
protected:


public:
   cropOilRadish(const char * aName, const int aIndex, const base * aOwner);
   virtual double Update(double ActivePar);
   virtual void Cut(plantItem * cutPlantMaterial, double CutHeight);
};

#endif


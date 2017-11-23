#ifndef cropDyersWoad_h
  #define cropDyersWoad_h

#include "../../base/cloneList.h"
#include "cropSbarley.h"
class cropDyersWoad: public cropSbarley
{
protected:


public:
   cropDyersWoad(const char * aName, const int aIndex, const base * aOwner, string cropName);
   virtual double Update(double ActivePar);
   virtual void Cut(plantItem * cutPlantMaterial, double CutHeight) ;
}
;

#endif


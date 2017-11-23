#ifndef cropHairyVetch_h
  #define cropHairyVetch_h

#include "../../base/cloneList.h"
#include "cropPea.h"

class cropHairyVetch: public cropPea
{
protected:


public:
   cropHairyVetch(const char * aName, const int aIndex, const base * aOwner, string cropName);
   virtual double Update(double ActivePar);
//   virtual void Cut(plantItem * cutPlantMaterial, double CutHeight) ;
};

#endif


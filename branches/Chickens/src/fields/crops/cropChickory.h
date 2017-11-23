#ifndef cropChickory_h
  #define cropChickory_h

#include "../../base/cloneList.h"
#include "cropSbarley.h"

class cropChickory: public cropSbarley
{
protected:


public:
   cropChickory(const char * aName, const int aIndex, const base * aOwner);
   virtual double Update(double ActivePar);
   virtual void Cut(plantItem * cutPlantMaterial, double CutHeight) ;
};

#endif


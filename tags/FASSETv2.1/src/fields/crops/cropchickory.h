#ifndef cropChickory_h
  #define cropChickory_h

#include <clonlist.h>
#include <cropgrss.h>

class cropChickory: public cropRyegrass
{
protected:


public:
   cropChickory(const char * aName, const int aIndex, const base * aOwner);
   virtual double Update(double ActivePar);
//	void Cut(plantItem * cutPlantMaterial, double CutHeight);
};

#endif


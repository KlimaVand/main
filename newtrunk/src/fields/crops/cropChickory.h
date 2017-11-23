
#ifndef cropChickory_h
#define cropChickory_h

#include "../../base/cloneList.h"
#include "cropSbarley.h"

class cropChickory:
    public cropSbarley
{
    protected:

    public:
        cropChickory(string       aName,
                     const int    aIndex,
                     const base * aOwner,
                     string       cropName);

        virtual double Update(double ActivePar);

        virtual void Cut(plantItem * cutPlantMaterial,
                         double      CutHeight);
};
#endif


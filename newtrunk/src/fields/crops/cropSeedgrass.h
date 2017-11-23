
#ifndef cropSeedGrass_h
#define cropSeedGrass_h

#include "../../base/cloneList.h"
#include "cropRyegrass.h"

class cropSeedGrass:
    public cropRyegrass
{
    protected:

    public:
        cropSeedGrass(string       aName,
                      const int    aIndex,
                      const base * aOwner,
                      string       cropName);

        virtual void Harvest(decomposable * Storage,
                             decomposable * Straw);
};
#endif


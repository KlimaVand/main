
#include "crop.h"

#ifndef CROPSBRL
#define CROPSBRL
class cropSbarley:
    public crop
{
    public:
        cropSbarley(string       aName,
                    const int    aIndex,
                    const base * aOwner,
                    string       cropName);
};
#endif


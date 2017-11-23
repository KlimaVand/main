
#ifndef cropCrimsonClove_H
#define cropCrimsonClove_H

#include "cropClover.h"

class cropCrimsonClover:
    public cropClover
{
    public:
        cropCrimsonClover(string       aName,
                          const int    aIndex,
                          const base * aOwner,
                          string       cropName);
};
#endif


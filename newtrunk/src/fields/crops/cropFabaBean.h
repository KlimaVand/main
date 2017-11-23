
#ifndef cropFabaBean_H
#define cropFabaBean_H

#include "cropPea.h"
#include "phenology.h"

class cropFabaBean:
    public cropPea
{
    public:
        cropFabaBean(string       aName,
                     const int    aIndex,
                     const base * aOwner,
                     string       cropName);
};
#endif


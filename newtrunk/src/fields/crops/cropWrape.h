
#include "crop.h"

class cropWrape:
    public crop
{
    public:
        cropWrape(string       aName,
                  const int    aIndex,
                  const base * aOwner,
                  string       cropName);
};

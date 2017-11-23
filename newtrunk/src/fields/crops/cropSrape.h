
#include "crop.h"

class cropSrape:
    public crop
{
    public:
        cropSrape(string       aName,
                  const int    aIndex,
                  const base * aOwner,
                  string       cropName);
};

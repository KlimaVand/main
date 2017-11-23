
#include "cropMaize.h"

class cropSunflower:
    public cropMaize
{
    public:
        cropSunflower(string       aName,
                      const int    aIndex,
                      const base * aOwner,
                      string       cropName);
};

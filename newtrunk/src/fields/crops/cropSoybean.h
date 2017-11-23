
#include "cropPea.h"

class cropSoybean:
    public cropPea
{
    public:
        cropSoybean(string       aName,
                    const int    aIndex,
                    const base * aOwner,
                    string       cropName);
};

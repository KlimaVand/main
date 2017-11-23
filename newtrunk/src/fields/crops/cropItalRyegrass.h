
#include "cropRyegrass.h"

class cropItalRyegrass:
	public cropRyegrass
{
    public:
        cropItalRyegrass(string       aName,
                         const int    aIndex,
                         const base * aOwner,
                         string       cropName);

        void Cut(plantItem * cutPlantMaterial,
                 double      cut_height);
};


#include "crop.h"

#ifndef CROPMAIZE
#define CROPMAIZE
class cropMaize:
    public crop
{
private:
	double SenescenceHeatThres;
	double SenescenceHeatResp;
    public:
        cropMaize(string       aName,
                  const int    aIndex,
                  const base * aOwner,
                  string       cropName);

        // double fTW();
        virtual void Cut(plantItem * cutPlantMaterial,
                         double      cut_height);
		virtual void CalcLeafAreaIndices();
        virtual bool ReadyForHarvest() {
            return Phenology -> DS > 1.0;
        }

        // BMP modified 01.03.2007
};
#endif


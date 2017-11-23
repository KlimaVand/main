
#ifndef CRROPSWHTH
#define CRROPSWHTH

#include "cropSbarley.h"

class cropSpringWheat:
    public cropSbarley
{
	private:
	double SenescenceHeatThres;
	double SenescenceHeatResp;
    public:
        cropSpringWheat(string       aName,
                        const int    aIndex,
                        const base * aOwner,
                        string       cropName);
		virtual void CalcLeafAreaIndices();
		
};
#endif



#ifndef CROPRYE
#define CROPRYE

#include "crop.h"
#include "phenology.h"

class cropRye:
    public crop
{
    protected:

        // State variables
        double HeightA,    // Height constants see Olesen, 2002 for description
               HeightB;

    public:
        cropRye(string       aName,
                const int    aIndex,
                const base * aOwner,
                string       cropName);

        cropRye(const cropRye & acrop);

        virtual void Sow(double SeedDensDryMatt,
                         double RelativeDensity,
                         double NitrogenInSeed);

        virtual double fNitrogen();

        virtual void UpdateHeight();
};
#endif


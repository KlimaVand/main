
#ifndef PIGSTYSECTION_H
#define PIGSTYSECTION_H

// #include "PigStyTypes.h"

#include "animalSection.h"

class pigstysection:
    public animalSection
{
    int DaysInSection;

    public:
        pigstysection(const pigstysection & s);

        pigstysection(const animalSection & s);

        pigstysection(string        aName,
                      const int     aIndex,
                      const base *  aOwner,
                      commonData *& runData,
                      commonData *& common,
                      bool          acontrolledVent);

        virtual ~pigstysection();

    public:

        // get functions
        double GetDaysInSection() {
            return DaysInSection;
        }

        double GetCapacity(double ProductionCyclus);
};
#endif


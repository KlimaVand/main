/*
 *
 * Created by Nick Hutchings, Sept 2006
 *
 */

#ifndef __TETEMAN_H
#define __TETEMAN_H

#include "techEquip.h"

/*
 *
 * Class: manureApplic
 *
 */
class manureApplic:
    public techEquip
{

    /* Attributes */
    protected:
        double propBuried;
        double propAreaWetted;
        double volume;
        bool   cropContamination;
        double NH3emissionFactor;

        /* Actions */

    private:
        manureApplic();    // Dissable use of default constructor

    public:
        manureApplic(string       aName,
                     const int    aIndex = -1,
                     const base * aOwner = nullptr);

        manureApplic(const manureApplic & amanureApplic);

        virtual ~manureApplic();

        virtual void ReadParameters(commonData *& file);

        double GetpropBuried() {
            return propBuried;
        }

        double GetpropAreaWetted() {
            return propAreaWetted;
        }

        bool GetcropContamination() {
            return cropContamination;
        }

        double GetNH3emissionFactor() {
            return NH3emissionFactor;
        }

        void SetpropBuried(double aProp) {
            propBuried = aProp;
        }
};
#endif


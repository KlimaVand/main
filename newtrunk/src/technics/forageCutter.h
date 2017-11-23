/*
 *
 * Created by Nick Hutchings, Sept 2006
 *
 */

#ifndef __TETECUT_H
#define __TETECUT_H

#include "techEquip.h"

/*
 *
 * Class: forageCutter
 *
 */
class forageCutter:
    public techEquip
{

    /* Attributes */
    protected:
        double residualHeight;

        /* Actions */

    private:
        forageCutter();    // Dissable use of default constructor

    public:
        forageCutter(string       aName,
                     const int    aIndex = -1,
                     const base * aOwner = nullptr);

        forageCutter(const forageCutter & aforageCutter);

        virtual ~forageCutter();

        virtual void ReadParameters(commonData *& file);

        double GetresidualHeight() {
            return residualHeight;
        }
};
#endif


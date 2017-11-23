/*
 *  Author   Lars Noboe Andersen & Poul Lassen
 * Revision   Bjørn Molt Petersen, May-June 1997
 */

#ifndef __TEOPFI_H
#define __TEOPFI_H

#include "operation.h"
#include "../base/commonData.h"

/*
 *
 * Class: fieldOperationTech
 *
 */
class fieldOperationTech:
    public operation
{

    /* Attributes */
    protected:
        double area;
        double distance;
        double oldArea;
        double oldDistance;
        string cropName;
        double workingSpeed;
        double turningTime;
        double turningHeadland;
        double disturbanceAllowance;
        double relaxationAllowance;
        double machinePoolPrice;

        /* Actions */

    private:
        fieldOperationTech();                     // Dissable use of default constructor

        fieldOperationTech(
                const fieldOperationTech & f);    // Dissable the compilers generation of default copy constructor.

        fieldOperationTech & operator = (
                const fieldOperationTech & f);    // Dissable the compilers generation of default assignment operator.

    protected:
        virtual void CalcGangFuel();

    public:
        fieldOperationTech(string       aName,
                           const int    aIndex,
                           const base * aOwner = nullptr);

        virtual ~fieldOperationTech();

        void Setarea(double aarea) {
            area = aarea;
        }

        void SetcropName(const string & acropName) {
            cropName = acropName;
        }

        void Setdistance(double adistance) {
            distance = adistance;
        }

        virtual void ReadParameters(commonData *& file);

        virtual void CalcCost(calcLPType mode) {}

        virtual void UpdateProducts(bool contractor = false);

        double GetMachinePoolPrice() {
            return machinePoolPrice;
        }
};
#endif


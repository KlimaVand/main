
#ifndef __FIOPAN_H
#define __FIOPAN_H

#include "fieldOperationFields.h"
#include "../../products/manure.h"
#include "../../technics/animalFertTech.h"

/*
 * Describe what happens when one use animal Manure on a field
 */
class animalFertFields:
    public fieldOperationFields
{
    private:
        manure * aManure;
        double   exposeTime;

        animalFertFields & operator = (
                const animalFertFields & f);    // Dissable the compilers generation of default assignment operator.

    public:
        animalFertFields();

        animalFertFields(const animalFertFields & a);

        virtual ~animalFertFields();

        virtual animalFertFields * clone() const;

        // ! return true
        virtual bool IsFertilizingOp() {
            return true;
        }

        virtual void DefineAnimalFertFieldsOp(operationNames op,
                string                                       crop,
                string                                       cropNm,
                int                                          yr,
                int                                          mon,
                int                                          d,
                double                                       ar,
                int                                          fieldn,
                double                                       dist,
                manure *                                     theManure,
                double                                       anExposeTime = 24.0);    // , string applicMethod);

        virtual manure * GetManure() {
            return aManure;
        }

        virtual double GetExposeTime() {
            return exposeTime;
        }

        virtual bool CalcCost(linkList<operation> * aOperationList,
                              linkList<techEquip> * aTechFarmList,
                              calcLPType            mode,
                              double *              cost,
                              bool                  update);

        virtual fieldOperationTech * GetfieldOperationTech(linkList<operation> * aOperationList);

        // virtual void AdjustManure(double adjustmentFactor);
        void Output(ofstream * fs);
};
#endif


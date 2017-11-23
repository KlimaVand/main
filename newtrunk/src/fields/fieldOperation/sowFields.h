
#ifndef __FIOPSO_H
#define __FIOPSO_H

#include "fieldOperationFields.h"
#include "../../products/seed.h"
#include "../../technics/sowTech.h"

class sowFields:
    public fieldOperationFields
{
    protected:
        struct sowStruct {
            string crop_id;
            seed * seeds;
        };


        // !Optional reduction in radiation use efficiency
        double    reduction;    //
        sowStruct plants[MaxPlants];

        // ! Plant Protection strategy
        int PVStrategy;

        // ! number of plants in that area
        int numOfPlants;

        sowFields & operator = (
                const sowFields & f);    // Dissable the compilers generation of default assignment operator.

    public:
        sowFields();

        sowFields(const sowFields & h);

        virtual ~sowFields();

        virtual sowFields * clone() const;

        virtual void DefineSowFieldsOp(operationNames op,
                                       string         crop,
                                       string         cropNm,
                                       int            yr,
                                       int            mon,
                                       int            d,
                                       double         ar,
                                       int            fieldn,
                                       double         dist,
                                       int            PVStrategy,
                                       double         reduc,
                                       seed *         theSeed);

        virtual seed * GetSeed(int i) {
            return plants[i].seeds;
        }

        virtual string GetSpecificCropId(int i) {
            return plants[i].crop_id;
        }

        virtual int GetPVStrat() {
            return PVStrategy;
        }

        virtual double GetReduction() {
            return reduction;
        }

        virtual void AddSeed(seed * s,
                             string crop);

        virtual bool IsSowingOp() {
            return true;
        }

        virtual int GetSeedNum() {
            return numOfPlants;
        }

        virtual bool CalcCost(linkList<operation> * aOperationList,
                              linkList<techEquip> * aTechFarmList,
                              calcLPType            mode,
                              double *              cost,
                              bool                  update);

        virtual void Output(ofstream * fs);
};
#endif


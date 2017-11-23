// Soil added organic matter aggregation class
// (c) Bj�rn Molt Petersen, Danmarks JordbrugsForskning 1998



#ifndef ADDMATT_H
#define ADDMATT_H

#include "matter.h"
#include "addedPool.h"

class addedMatter:
    public matter
{
    protected:
        enum { maxSubPools = 2000 };

        int         numOfSubPools;
        addedPool * addPoolList[maxSubPools];

    public:
        addedMatter(string       Pname,
                    const int    Index,
                    const base * owner);

        addedMatter(const addedMatter & source);

        virtual ~addedMatter();

        virtual addedMatter * clone() const;

        virtual void Initialize(double clayEff1,
                                double clayEff2);

        virtual void Update(double environmentEffect);

        virtual void CatchInfluxAOM(cnMatter & content);

        virtual void CatchInflux(cnMatter & content) {
            CatchInfluxAOM(content);
        }

        virtual void TestAddProduct(organicProduct * prod,
                                    double *         check,
                                    double *         CN,
                                    int *            num,
                                    bool             forAOM);

        // Should never be called for this instance
        virtual double PotentialNitrogenMineralisation();

        virtual double ResidualCarbon();

        virtual void FinalExport(double ratio);

        virtual cnMatter GetCarbon();

        virtual double GetCarbonC();

        virtual double GetNitrogenN();

        virtual double GetNitrogenN15();

        virtual nitrogen GetNitrogen();

        virtual bool IsAOM() const {
            return true;
        }

        virtual double PotentialCarbonTurnover();
};
#endif


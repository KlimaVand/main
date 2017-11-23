
#include "../base/common.h"
#include "addedMatter.h"
#include "../base/settings.h"

/*
 * Constructor with arguments
 * Pname - Name of instance
 * Index - Instance number
 * owner - Pointer to owner object
 */
addedMatter::addedMatter(string       Pname,
                         const int    Index,
                         const base * owner):
    matter(Pname,
           Index,
           owner)
{
    numOfSubPools = 0;
}

addedMatter::addedMatter(const addedMatter & source):
    matter(source)
{
    if (!source.IsAOM() && cnRatio <= 0) {
            theMessage -> FatalError("addedMatter::addedMatter - illegal state of cnRatio variable");
    }

    numOfSubPools = source.numOfSubPools;

    for (int i = 0; i < numOfSubPools; i++) {
        addPoolList[i] = new addedPool(*source.addPoolList[i]);
    }
}

addedMatter * addedMatter::clone() const {
    addedMatter * a = new addedMatter(*this);

    return a;
}

addedMatter::~addedMatter() {
    for (int i = 0; i < numOfSubPools; i++) {
        delete addPoolList[i];
    }
}

/*
 * Initializes the organic pool object
 * Must be called AFTER all parameters are read by all organic matter objects
 */
void addedMatter::Initialize(double clayEff1,
                             double clayEff2) {
    if (cnRatio > 0) {
        theMessage -> FatalError("The added matter pool '", poolNameChar, "' can not have a fixed C/N ratio");
    }

    matter::Initialize(clayEff1, clayEff2);

    if ((carbon.c > 0) && (0 == numOfSubPools)) {
        addedPool * aP = new addedPool(poolNameChar, numOfSubPools, Owner);

        aP -> SetDecompositionRate(decompositionRate);
        aP -> SetCarbon(carbon);
        aP -> SetClayEffect1(clayEff1);
        aP -> SetClayEffect2(clayEff2);
        aP -> SetUseClayEffect(useClayEffect);
        aP -> SetExponentialDecay(expDecay);

        addPoolList[0] = aP;
        numOfSubPools  = 1;
    }
}

/*
 * Calculates the decomposition of the organic matter in the pool
 * Does ONLY transfer the carbon to the receiver pools, if the subpool contributes
 * to a net mineralisation, NOT if it contributes to a net immobilisation.
 * environmentEffect - covers combined effects of water content and temperature.
 */
void addedMatter::Update(double environmentEffect) {
    cnMatter outflux;
    cnMatter out;

    if (numOfSubPools > 0) {
        for (int i = 0; i < numOfSubPools; i++) {
            out.SetAll(0.0, 0.0, 0.0, 0.0);
            addPoolList[i] -> Decay(environmentEffect, out, CNcrit());

            outflux = outflux + out;
        }

        if (outflux.c > 0.0) {
            Export(outflux);
        }

        carbon = carbon - outflux;    // In order to do the double book-keeping
    }
}

double addedMatter::PotentialCarbonTurnover() {
    double retVal = 0.0;

    if (numOfSubPools > 0) {
        for (int i = 0; i < numOfSubPools; i++) {
            retVal += addPoolList[i] -> PotentialCarbonTurnover();
        }
    }

    return retVal;
}

double addedMatter::PotentialNitrogenMineralisation() {
    double retVal = 0.0;

    if (numOfSubPools > 0) {
        for (int i = 0; i < numOfSubPools; i++) {
            retVal += addPoolList[i] -> PotentialNitrogenMineralisation();
        }
    }

    return retVal;
}

double addedMatter::ResidualCarbon() {
    double retVal = 0.0;

    if (numOfSubPools > 0) {
        for (int i = 0; i < numOfSubPools; i++) {
            retVal += addPoolList[i] -> ResidualCarbon();
        }
    }

    return retVal;
}

void addedMatter::FinalExport(double ratio) {
    cnMatter outflux;

    if (numOfSubPools > 0) {
        for (int i = 0; i < numOfSubPools; i++) {
            addPoolList[i] -> FinalExportAOM(ratio, outflux);
        }

        Export(outflux);
    }
}

/*
 * Adds all import to the pool, and returns the current carbon content
 * content - C content in pool
 */
void addedMatter::CatchInfluxAOM(cnMatter & content) {
    cnMatter c;

    if (numOfSubPools > 0) {
        for (int i = 0; i < numOfSubPools; i++) {
            addPoolList[i] -> CatchInflux(c);

            carbon = carbon + c;
        }

        content = carbon;
    }
}

/*
 * Investigates whether one of the fractions of the added matter fits with the
 * current pool. If this is the case, the matter is added to a subpool.
 * prod - pointer to organic product
 */
void addedMatter::TestAddProduct(organicProduct * prod,
                                 double *         check,
                                 double *         CN,
                                 int *            num,
                                 bool             forAOM) {
    *CN  = -1.0;
    *num = 0;

    if (forAOM) {
        for (int k = 0; k < prod -> GetNumOfFractions(); k++) {
            if (poolName == prod -> GetDestPool(k)) {
                *num = k;

                double decompr;

                decompr = prod -> GetDecompositionRate(k);

                if (decompr < 0) {                  // Decompositionrate not defined in product
                    decompr = decompositionRate;    // Set to pre-defined rate
                }

                bool found = false;

                for (int i = 0; i < numOfSubPools; i++) {
                    double addDecomp = addPoolList[i] -> GetDecompositionRate();

                    if (((prod -> GetReduceTurnover() == 0) && (addDecomp == decompr))
                            || ((prod -> GetReduceTurnover() > 0)
                                && (prod -> GetReduceTurnover() == addPoolList[i] -> GetReduceTurnover())
                                && (fabs(addDecomp - decompr * prod -> GetReduceTurnover()) < 1e-15))) {
                        found = true;

                        addPoolList[i] -> AddOrganicProduct(prod, k, check);

                        break;
                    }
                }

                if (!found) {
                    addedPool * aP = new addedPool(poolNameChar, numOfSubPools, Owner);

                    aP -> SetDecompositionRate(decompr);
                    aP -> AddOrganicProduct(prod, k, check);
                    aP -> SetClayEffect1(clayEffect1);
                    aP -> SetClayEffect2(clayEffect2);
                    aP -> SetUseClayEffect(useClayEffect);
                    aP -> SetExponentialDecay(expDecay);

                    addPoolList[numOfSubPools] = aP;

                    aP -> SetReduceTurnover(prod -> GetReduceTurnover());
                    numOfSubPools++;

                    if (numOfSubPools > maxSubPools) {
                        theMessage -> FatalError("addedMatter::TestAddProduct - to many pools");
                    }
                }
            }
        }
    }
}

cnMatter addedMatter::GetCarbon() {
    cnMatter retVal;

    if (numOfSubPools > 0) {
        for (int i = 0; i < numOfSubPools; i++) {
            retVal = retVal + addPoolList[i] -> GetCarbon();
        }
    }

    return retVal;
}

double addedMatter::GetCarbonC() {
    double retVal = 0;

    if (numOfSubPools > 0) {
        for (int i = 0; i < numOfSubPools; i++) {
            retVal += addPoolList[i] -> GetCarbonC();
        }
    }

    return retVal;
}

nitrogen addedMatter::GetNitrogen() {
    nitrogen retVal;

    if (numOfSubPools > 0) {
        for (int i = 0; i < numOfSubPools; i++) {
            retVal = retVal + addPoolList[i] -> GetNitrogen();
        }
    }

    return retVal;
}

double addedMatter::GetNitrogenN() {
    double retVal = 0;

    if (numOfSubPools > 0) {
        for (int i = 0; i < numOfSubPools; i++) {
            retVal += addPoolList[i] -> GetNitrogenN();
        }
    }

    return retVal;
}

double addedMatter::GetNitrogenN15() {
    double retVal = 0;

    if (numOfSubPools > 0) {
        for (int i = 0; i < numOfSubPools; i++) {
            retVal += addPoolList[i] -> GetNitrogenN15();
        }
    }

    return retVal;
}

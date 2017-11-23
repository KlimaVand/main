
#include "matter.h"
#include <string.h>

/*
 *
 * @param Pname - Name of instance
 * @param Index - Instance number
 * @param owner - Pointer to owner object
 */
matter::matter(string       Pname,
               const int    Index,
               const base * owner):
    base(Pname,
         Index,
         owner)
{
    carbon.Clear();
    influx.Clear();
    intermed.Clear();
    added.Clear();

    cnRatio           = -1.0;
    poolName          = -1;
    poolNameChar      = "";
    decompositionRate = 0.0;
    clayEffect1       = 1.0;
    clayEffect2       = 1.0;
    useClayEffect     = false;
    expDecay          = false;
    connections       = 0;
    CNC               = -1.0;
}

matter * matter::clone() const {
    matter * m = new matter(*this);

    return m;
}

/*
 * Reads parameters and initial state from file
 */
void matter::ReadParameters(commonData *& file) {
    file -> setCritical(true);

    if (file -> FindSection(Name, Index)) {    // Top of inheritance hierarcy
        file -> FindItem("Name", &poolNameChar);

        if (poolNameChar == "AOM1") {
            poolName = 0;
        } else if (poolNameChar == "AOM2") {
            poolName = 1;
        } else if (poolNameChar == "SMB1") {
            poolName = 2;
        } else if (poolNameChar == "SMB2") {
            poolName = 3;
        } else if (poolNameChar == "SMR") {
            poolName = 4;
        } else if (poolNameChar == "NOM") {
            poolName = 5;
        } else if (poolNameChar == "IOM") {
            poolName = 6;
        }

        file -> setCritical(false);
        file -> FindItem("CNratio", &cnRatio);
        file -> FindItem("UseClayEffect", &useClayEffect);
        file -> FindItem("DecompositionRate", &decompositionRate);
        file -> FindItem("ExponentialDecay", &expDecay);
        file -> setCritical(true);

        // Read connections
        double checkN = 0.0;
        double checkC = 0.0;
        int    small, large;

        file -> FindVectorSize("Direction", &small, &large);

        for (int i = small; i <= large; i++) {
            string tmp;

            file -> FindItem("Direction", &tmp, i);

            connectionList[i].connectnameChar = tmp;

            if (connectionList[i].connectnameChar == "AOM1") {
                connectionList[i].connectname = 0;
            } else if (connectionList[i].connectnameChar == "AOM2") {
                connectionList[i].connectname = 1;
            } else if (connectionList[i].connectnameChar == "SMB1") {
                connectionList[i].connectname = 2;
            } else if (connectionList[i].connectnameChar == "SMB2") {
                connectionList[i].connectname = 3;
            } else if (connectionList[i].connectnameChar == "SMR") {
                connectionList[i].connectname = 4;
            } else if (connectionList[i].connectnameChar == "NOM") {
                connectionList[i].connectname = 5;
            } else if (connectionList[i].connectnameChar == "IOM") {
                connectionList[i].connectname = 6;
            }

            if (file -> FindItem("Fraction", &connectionList[i].Cfraction, i)) {
                connectionList[i].Nfraction = connectionList[i].Cfraction;
            }

            file -> setCritical(false);
            file -> FindItem("FractionN", &connectionList[i].Nfraction, i);

            checkC += connectionList[i].Cfraction;
            checkN += connectionList[i].Nfraction;

            connections++;
        }

        if ((fabs(checkC - 1.0) > 1E-6) || (fabs(checkN - 1.0) > 1E-6)) {

            string  name(Name);
            string  output = "Incorrect sum of fractions in " + name + "." + conv.IntToString(Index);

            theMessage -> FatalError(output);
        }
    }
}

cnMatter matter::GetCarbon() {
    cnMatter tmp;

    tmp.c     = GetCarbonC();
    tmp.c_iso = carbon.c_iso + intermed.c_iso + influx.c_iso + added.c_iso;
    tmp.n     = GetNitrogenN();
    tmp.n15   = GetNitrogenN15();

    return tmp;
}

double matter::GetCarbonC() {
    return carbon.c + intermed.c + influx.c + added.c;
}

double matter::GetNitrogenN() {
    return carbon.n + intermed.n + influx.n + added.n;
}

double matter::GetNitrogenN15() {
    return carbon.n15 + intermed.n15 + influx.n15 + added.n15;
}

nitrogen matter::GetNitrogen() {
    nitrogen tmp;

    tmp.SetBoth(GetNitrogenN(), GetNitrogenN15());

    return tmp;
}

double matter::ResidualCarbon() {
    return intermed.c;
}

double matter::PotentialNitrogenMineralisation() {
    double retVal = intermed.n - intermed.c / CNcrit();

    if (cnRatio > 0) {
        retVal += influx.n - influx.c / cnRatio;
    }

    if ((retVal > 1E-10) && (intermed.c > 1E-10)) {
        cout << endl << "Warning: matter::PotentialNitrogenMineralisation - possible programming inconsistency" << endl;
        cout << "Press any key to continue" << endl;

#ifndef __BCplusplus__
        char dummy;

        cin >> dummy;
#else
        getch();
#endif

    }

    return retVal;
}

/*
 * Initializes the organic pool object
 * Must be called AFTER all parameters are read by all organic matter objects
 */
void matter::Initialize(double clayEff1,
                        double clayEff2) {
    if (useClayEffect) {
        clayEffect1 = clayEff1;
        clayEffect2 = clayEff2;
    } else { // CK: to make it clearer, even if it is the default
    	clayEffect1 = 1;
    	clayEffect2 = 1;
    }

    for (int i = 0; i < connections; i++) {
        connectionList[i].connectPt = ((organicMatter *) Owner)  -> GetPoolPointer(connectionList[i].connectname);
    }
}

/*
 * Adds carbon to the current carbon content in the pool, via a "transit deposit"
 * c - Carbon to add to the current content
 */
void matter::Import(cnMatter c) {
    if (c.c < 0) {
        theMessage -> FatalError("C import to pool negative");
    }

    if (c.n < 0) {
        theMessage -> FatalError("N import to pool negative");
    }

    influx = influx + c;
}

/*
 * Investigates whether one of the fractions of the added matter fits with the
 * current pool. If this is the case, the matter is added
 * prod - pointer to organic product
 */
void matter::TestAddProduct(organicProduct * prod,
                            double *         check,
                            double *         CN,
                            int *            num,
                            bool             forAOM) {
    *num = 0;

    if (!forAOM) {
        for (int i = 0; i < prod -> GetNumOfFractions(); i++) {
            if (poolName >= 7) {
                theMessage -> FatalError("matter::TestAddProduct");
            }

            if (poolName == prod -> GetDestPool(i)) {
                if (true /* !!!!! */) {    // A crude version of indirect MIT !!!
                    cnMatter am = prod -> GetAmount();

                    ((organicMatter *) Owner) -> EquilibrateN15(am);
                    prod -> SetAmount(am);
                }

                AddOrganicProduct(prod, i, check);

                *num = i;
                *CN  = cnRatio;
            }
        }
    }
}

/*
 * Adds a fraction of an organic product to the present pool
 * prod           - pointer to organic product
 * fractionNumber - number of fraction to be added
 */
void matter::AddOrganicProduct(organicProduct * prod,
                               int              fractionNumber,
                               double *         check) {
    cnMatter add;
    cnMatter prd = prod -> GetAmount();

    add.c     = prd.c * prod -> GetCFraction(fractionNumber) * prod -> GetCarbonFraction();
    add.c_iso = prd.c_iso * prod -> GetCFraction(fractionNumber) * prod -> GetCarbonFraction();

    // double nf=prod->GetNFraction(fractionNumber);
    add.n   = prd.n * prod -> GetNFraction(fractionNumber);
    add.n15 = prd.n15 * prod -> GetNFraction(fractionNumber);

    if (!IsAOM()) {    // Adjust to match C/N ratio
        if (cnRatio <= 0.0) {
            theMessage -> FatalError("matter::AddOrganicProduct - C/N ratio should not be zero or negative here");
        }

        double addN   = add.c / cnRatio;
        double factor = 0.0;

        if (add.n > 0) {
            factor = addN / add.n;
        }

        add.n15 = factor * add.n15;
        add.n   = addN;
    }

    *check += add.c;
    added  = added + add;

    if (add.n < -0.000001) {
        cout << add.n << endl;

        theMessage -> FatalError("matter::AddOrganicProduct - negative amount of N added");
    }

    if (add.c < 0.0) {
        theMessage -> FatalError("matter::AddOrganicProduct - negative amount of C added");
    }
}

/*
 * For the moment this function simply returns the highest C/N ratio of all
 * receiver pools.
 */
double matter::CNcrit() {
    if (CNC <= 0) {
        for (int i = 0; i < connections; i++) {
            if (connectionList[i].connectPt) {
                double cn = connectionList[i].connectPt -> GetCNcrit();

                if (cn <= 0) {
                    theMessage -> FatalError("addedMatter::CalcCNcrit - a pool has undefined CNcrit");
                }

                if ((cn < CNC) || (CNC < 0.0)) {
                    CNC = cn;
                }
            } else {
                double n = connectionList[i].Nfraction;    // Any N to CO2 is mineralised, but should generally not occur

                if (n > 0.0) {
                    cout << "Warning - no N should flow to directly to CO2" << endl;
                }
            }
        }
    }

    return CNC;
}

void matter::TestState() {
    if (carbon.c_iso < 0.0) {
        carbon.c_iso = 0.0;
    }

    if (carbon.n15 < 0.0) {
        carbon.n15 = 0.0;
    }

    if (carbon.c < -1E-10) {
        theMessage -> FatalError("matter::TestState - negative amount of C");
    }

    if (carbon.n < -1E-10) {
        theMessage -> FatalError("matter::TestState - negative amount of N");
    }

    if (!IsAOM() && cnRatio <= 0) {
            theMessage -> FatalError("matter::TestState - illegal state of cnRatio variable");
    }
}

/*
 * Calculates the decomposition of the organic matter in the pool and transfers
 * the carbon to the receiver pools.
 * environmentEffect - covers combined effects of water content and temperature.
 */
void matter::Update(double environmentEffect) {
    TestState();

    if (intermed.c > 0.0) {
        theMessage -> FatalError("matter::Update - programming inconsistency");
    }

    intermed.SetAll(0.0, 0.0, 0.0, 0.0);

    if (carbon.c > 1E-45) {
       // Like DAISY, the only one that affects turnover in this code

        if (expDecay) {         // Exponential decay
            intermed = carbon * (1 - exp(-decompositionRate * environmentEffect * clayEffect2));
        } else {                // Euler integration
            intermed = carbon * decompositionRate * environmentEffect * clayEffect2;
        }

        if (intermed.c > carbon.c) {
            intermed = carbon;
        }

        carbon = carbon - intermed;
    }

    if (intermed.n > 0.0 && (intermed.c / intermed.n) <= CNcrit()) {
            Export(intermed);
            intermed.SetAll(0.0, 0.0, 0.0, 0.0);
     }


    TestState();
}

/*
 * Adds all import to the pool, and returns the current carbon content
 * content - C content in pool
 */
void matter::CatchInflux(cnMatter & content) {
    if (influx.c < 0.0) {
        theMessage -> FatalError("Influx to pool negative");
    }

    cnMatter in = influx + added;

    if ((in.c) > 0.0) {
        if (in.n > 0) {
            if (cnRatio <= 0.0) {
                theMessage -> FatalError("matter::CatchInflux - 'cnRatio' has illegal value for this instance");
            }

            nitrogen nit;

            nit.n   = in.n - in.c / cnRatio;
            nit.n15 = in.n15 / in.n * nit.n;
            in.n    -= nit.n;

            ((organicMatter *) Owner) -> ExchangeNitrogen(&nit);

            in.n15 -= nit.n15;
        } else {
            theMessage -> FatalError("matter::CatchInflux - C with no associated N");
        }
    }

    carbon = carbon + in;

    influx.SetAll(0.0, 0.0, 0.0, 0.0);
    added.SetAll(0.0, 0.0, 0.0, 0.0);

    content = carbon;
}

void matter::FinalExport(double ratio) {
    if ((ratio > 1.0) || (ratio < 0.0)) {
        theMessage -> FatalError("matter::FinalExport - 'ratio' has illegal value");
    }

    if (intermed.c > 0.0) {
        carbon = carbon + intermed * (1.0 - ratio);

        Export(intermed * ratio);
        intermed.SetAll(0.0, 0.0, 0.0, 0.0);
    }
}

/*
 * Exports carbon to other pools
 * c - Carbon to export
 */
void matter::Export(cnMatter c) {
    if (c.c > 0.0) {

        for (int i = 0; i < connections; i++) {
            if ((connectionList[i].Cfraction < 0.0) || (connectionList[i].Cfraction > 1.0000001)) {
                theMessage -> FatalError("matter::Export - C fraction is illegal for ", poolNameChar);
            }

            if ((connectionList[i].Nfraction < 0.0) || (connectionList[i].Nfraction > 1.0000001)) {
                theMessage -> FatalError("matter::Export - N fraction is illegal for ", poolNameChar);
            }

            cnMatter exportMatter;

            exportMatter.SetAll(c.c * connectionList[i].Cfraction, c.c_iso * connectionList[i].Cfraction,
                                c.n * connectionList[i].Nfraction, c.n15 * connectionList[i].Nfraction);

            if (connectionList[i].connectPt) {
                connectionList[i].connectPt -> Import(exportMatter);
            } else {
                ((organicMatter *) Owner) -> CO2emmision(exportMatter);    // Pointer nullptr, so flux directly to CO2
            }
        }
    }
}

double matter::GetConnectionFraction(int poolName) {
    double retval = 0.0;

    for (int i = 0; i < connections; i++) {
        if (connectionList[i].connectname == poolName) {
            retval += connectionList[i].Cfraction;
        }
    }

    return retval;
}

matter::matter(const matter & source):
    base(source)
{

    cnRatio     = source.cnRatio;

    if (!source.IsAOM() &&  cnRatio <= 0) {
            cout << "CNratio is " << cnRatio << endl;

            theMessage -> FatalError("matter::matter - illegal state of cnRatio variable");

    }

    poolName          = source.poolName;
    poolNameChar      = source.poolNameChar;
    decompositionRate = source.decompositionRate;
    clayEffect1       = source.clayEffect1;
    clayEffect2       = source.clayEffect2;
    useClayEffect     = source.useClayEffect;
    expDecay          = source.expDecay;
    connections       = source.connections;
    CNC               = source.CNC;
    carbon            = source.carbon;
    influx            = source.influx;
    intermed          = source.intermed;
    added             = source.added;

    for (int i = 0; i < source.connections; i++) {
        connectionList[i].connectPt       = nullptr;
        connectionList[i].Cfraction       = source.connectionList[i].Cfraction;
        connectionList[i].Nfraction       = source.connectionList[i].Nfraction;
        connectionList[i].connectname     = source.connectionList[i].connectname;
        connectionList[i].connectnameChar = source.connectionList[i].connectnameChar;
    }
}

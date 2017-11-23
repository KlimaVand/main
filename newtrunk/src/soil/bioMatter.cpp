
#include "../base/common.h"
#include "bioMatter.h"
#include <string.h>

/*
 *
 * @param Pname - Name of instance
 * @param Index - Instance number
 * @param owner - Pointer to owner object
 */
bioMatter::bioMatter(string       Pname,
                     const int    Index,
                     const base * owner):
    matter(Pname,
           Index,
           owner)
{
    lackOfN               = 0.0;
    maintenance           = 0.0;
    utilizationEfficiency = 0.0;
    deathRate             = 0.0;
    minCNratio            = -1.0;
    cnRatioDefault        = -1.0;
}

bioMatter::bioMatter(const bioMatter & source):
    matter(source)
{
    lackOfN = 0.0;

    if (!source.IsBiomass()) {
        theMessage -> FatalError("bioMatter::bioMatter - source is not biomatter");
    }

    maintenance           = source.maintenance;
    utilizationEfficiency = source.utilizationEfficiency;
    deathRate             = source.deathRate;
    minCNratio            = source.minCNratio;
    cnRatioDefault        = source.cnRatioDefault;
    intermedCO2           = source.intermedCO2;
}

bioMatter * bioMatter::clone() const {
    bioMatter * b = new bioMatter(*this);

    return b;
}

void bioMatter::SetCarbon(cnMatter c) {
    if (c.n > 0.0) {
        if ((c.c / c.n > (cnRatio + 1E-6)) || (c.c / c.n < (minCNratio - 1E-6))) {
            theMessage -> FatalError("bioMatter::SetCarbon - C/N ratio beyond possible borders");
        }
    } else if (c.c > 0.0) {
        theMessage -> FatalError("bioMatter::SetCarbon - C present in pool but no N");
    }

    carbon = c;
}

/*
 * Reads parameters and initial state from file
 */
void bioMatter::ReadParameters(commonData *& file) {
    matter::ReadParameters(file);
    file -> setCritical(true);
    file -> FindItem("UtilizationEfficiency", &utilizationEfficiency);
    file -> FindItem("CNratioMin", &minCNratio);
    file -> FindItem("CNratioMax", &cnRatio);
    file -> setCritical(false);
    file -> FindItem("CNratioDefault", &cnRatioDefault);

    if (cnRatioDefault < 0.0) {
        cnRatioDefault = minCNratio;
    }

    if (minCNratio > cnRatio) {
        theMessage -> FatalError("bioMatter::ReadParameters - max cn ratio must be higher than min cn ratio");
    }

    file -> FindItem("DeathRate", &deathRate);
    file -> FindItem("Maintenance", &maintenance);

    double tt = 0.0;
    double tr = 0.0;

    file -> FindItem("TotalTurnover", &tt);

    if (file -> FindItem("TurnoverRatio", &tr)) {
        if ((tt <= 0.0) && (tr <= 0.0)) {
            theMessage -> FatalError(
                "bioMatter::ReadParameters - both 'TurnoverRatio' and 'TotalTurnover' must be specified and above zero");
        }

        if ((maintenance > 0.0) || (deathRate > 0.0)) {
            theMessage -> FatalError("bioMatter::ReadParameters - inconsistent reading of parameters");
        }

        deathRate   = tt * tr;
        maintenance = tt - deathRate;
    }

    if (deathRate <= 0.0) {
        theMessage -> FatalError("bioMatter::ReadParameters - 'deathRate' must be above zero");
    }

    decompositionRate = deathRate + maintenance;
}

double bioMatter::GetCNcrit() {
    double retVal = 1E6;

    if (utilizationEfficiency > 0.0) {
        retVal = cnRatio / utilizationEfficiency;
    }

    return retVal;
}

double bioMatter::ResidualCarbon() {
    return intermed.c + intermedCO2.c;
}

double bioMatter::PotentialNitrogenMineralisation() {
    double retVal = 0.0;

    if (cnRatio > 0) {
        retVal = influx.n - (influx.c * utilizationEfficiency) / cnRatio;

        if (intermed.c > 0.0) {
            retVal += intermed.n - intermed.c / CNcrit();
        }
    } else {
        theMessage -> FatalError("C/N ratio not set for the '", poolNameChar, "' pool");
    }

    return retVal;
}

void bioMatter::FinalExport(double ratio) {
    if ((ratio > 1.0) || (ratio < 0.0)) {
        theMessage -> FatalError("matter::FinalExport - 'ratio' has illegal value");
    }

    if ((intermed.c > 0.0) && (intermed.c / intermed.n) <= CNcrit()) {
        carbon = carbon + (intermed + intermedCO2) * (1.0 - ratio);

        Export(intermed * ratio);
        ((organicMatter *) Owner) -> CO2emmision(intermedCO2 * ratio);
        intermed.SetAll(0.0, 0.0, 0.0, 0.0);
        intermedCO2.SetAll(0.0, 0.0, 0.0, 0.0);
    }
}

/*
 * Calculates the death of the biomatter in the pool and transfers the carbon
 * to the receiver pools.
 * Calculates respiration and transfers the carbon to the CO2 book-keeping
 * function in the owner class.
 * environmentEffect - covers combined effects of water content and temperature.
 * // OBS maintanancerespiration not accounted correctly when using Runge-Kutta
 */
void bioMatter::Update(double environmentEffect) {

    // if ((carbon.c > 0) && (carbon.n <= 0)) {
    // cout << "Name path: " << GetLongName() << endl;
    // cout << theTime << endl;
    //
    // theMessage -> FatalError("bioMatter::Update - C present but no N (1)");
    // } else {
    // double cnrat = carbon.c / carbon.n;
    //
    // if (fabs(cnrat - cnRatio) > 0.01) {
    // theMessage -> FatalError("bioMatter::Update - illegal C/N ratio");
    // }
    // }
    //
    // if (maintenance <= 0.0) {
    // theMessage -> FatalError("bioMatter::Update - 'maintenance' must be above zero");
    // }
    //
    // TestState();
    //
    // if (carbon.n < -1E-10) {
    // theMessage -> FatalError("bioMatter::Update - negative N content");
    // }
    // if ((intermed.c > 0.0) || (intermedCO2.c > 0.0)) {
    // theMessage -> FatalError("bioMatter::Update - programming inconsistency");
    // }
    if (carbon.c > 0.0) {

        // CK: clayEffect is set to 1 if it is not used
        // Both DAISY and "homemade", otherwise "cl" is one
        double   dr = deathRate * environmentEffect * clayEffect1;
        double   mr = maintenance * environmentEffect * clayEffect1;
        double   tr = dr + mr;
        cnMatter totalLoss;

        if (expDecay) {
            totalLoss = carbon * (1 - exp(-tr));
        } else {
            totalLoss = carbon * tr;
        }

        if (totalLoss.c > carbon.c) {    // CK: shouldn't be tr < 1 ???
            totalLoss = carbon;
        }

        intermed    = totalLoss * dr / tr;
        intermedCO2 = totalLoss * mr / tr;

        if (intermed.c > carbon.c) {     // CK: strange...
            intermed = carbon;
        }

        // TestState(); is not only a test, can change values...
        carbon = carbon - intermedCO2 - intermed;

        // TestState(); is not only a test, can change values...
        intermed.n      += intermedCO2.n;
        intermedCO2.n   = 0.0;
        intermed.n15    += intermedCO2.n15;
        intermedCO2.n15 = 0.0;
    }

    FinalExport(1);

    // if ((intermed.n > 0.0) && (intermed.c / intermed.n) <= CNcrit()) {
    // Export(intermed);
    // ((organicMatter *) Owner) -> CO2emmision(intermedCO2);
    // intermed.SetAll(0.0, 0.0, 0.0, 0.0);
    // intermedCO2.SetAll(0.0, 0.0, 0.0, 0.0);
    // }
    // TestState(); CK: is not only a test, can change values...
    // if (carbon.n < -1E-10) {
    // theMessage -> FatalError("bioMatter::Update - negative N content");
    // }
    //
    // if ((carbon.c > 0) && (carbon.n <= 0)) {
    // theMessage -> FatalError("bioMatter::Update - C present but no N (2)");
    // } else {
    // double cnrat = carbon.c / carbon.n;
    //
    // if (fabs(cnrat - cnRatio) > 0.01) {
    // theMessage -> FatalError("bioMatter::Update - illegal C/N ratio");
    // }
    // }
}

bool bioMatter::ExtraTurnover(double Fraction) {
    double dr     = deathRate;
    double mr     = maintenance;
    double factor = Fraction / (dr + mr);

    dr *= factor;
    mr *= factor;

    double tr = dr + mr;

    // if (fabs(tr - Fraction) > 1E-10) {
    // theMessage -> FatalError("bioMatter::ExtraTurnover - coding error!");
    // }
    // tr          = Fraction;
    cnMatter totalLoss = carbon * tr;

    intermed    = totalLoss * dr / tr;
    intermedCO2 = totalLoss * mr / tr;
    carbon      = carbon - intermedCO2 - intermed;

    // TestState(); CK:
    intermed.n      += intermedCO2.n;
    intermedCO2.n   = 0.0;
    intermed.n15    += intermedCO2.n15;
    intermedCO2.n15 = 0.0;

    FinalExport(1);

    // if (intermed.n > 0.0 && (intermed.c / intermed.n) <= CNcrit()) {
    // Export(intermed);
    // ((organicMatter *) Owner) -> CO2emmision(intermedCO2);
    // intermed.SetAll(0.0, 0.0, 0.0, 0.0);
    // intermedCO2.SetAll(0.0, 0.0, 0.0, 0.0);
    // }
    // double cnrat = carbon.c / carbon.n;
    //
    // if (fabs(cnrat - cnRatio) > 0.01) {
    // theMessage -> FatalError("bioMatter::Update - illegal C/N ratio");
    // }
    return true;
}

/*
 * Adds all import to the pool, and returns the current carbon content.
 * Calculates respiration from nurishment utilisation and transfers the carbon
 * to the CO2 book-keeping function in the owner class.
 * content - C content in pool
 */
void bioMatter::CatchInflux(cnMatter & content) {
    cnMatter utilizationRespiration;

    utilizationRespiration.c     = influx.c * (1 - utilizationEfficiency);
    utilizationRespiration.c_iso = influx.c_iso * (1 - utilizationEfficiency);
    carbon.c                     = carbon.c + (influx.c + added.c) * utilizationEfficiency;
    carbon.c_iso                 = carbon.c_iso + (influx.c_iso + added.c_iso) * utilizationEfficiency;
    carbon.n                     += influx.n + added.n;
    carbon.n15                   += influx.n15 + added.n15;

    ((organicMatter *) Owner) -> CO2emmision(utilizationRespiration);

    if (carbon.c > 0.0) {
        if (cnRatio <= 0.0) {
            theMessage -> FatalError("bioMatter::CatchInflux - 'cnRatio' has illegal value for this instance");
        }

        nitrogen nit;

        nit.n = (influx.n + added.n) - utilizationEfficiency * (influx.c + added.c) / cnRatio;

        if (nit.n > 0.0) {
            nit.n15 = carbon.n15 / carbon.n * nit.n;
        }

        carbon.n -= nit.n;

        ((organicMatter *) Owner) -> ExchangeNitrogen(&nit);

        carbon.n15 -= nit.n15;
    }

    influx.SetAll(0.0, 0.0, 0.0, 0.0);
    added.SetAll(0.0, 0.0, 0.0, 0.0);

    content = carbon;
}

void bioMatter::SetDeathAndMaintence(double val) {
    if (val > 0.9999) {
        val = 0.9999;
    } else if (val < 0.0001) {
        val = 0.0001;
    }

    double tot = deathRate + maintenance;

    if (tot <= 0.0) {
        theMessage -> FatalError("bioMatter::SetDeathAndMaintence - sum of rates must be above zero");
    }

    deathRate   = tot * val;
    maintenance = tot - deathRate;

    if (fabs(tot - deathRate - maintenance) > 1E-5) {
        cout << "Warning: bioMatter::SetDeathAndMaintence - possible programming error 2";

#ifdef __BCplusplus__
        cout << "Press any key to continue" << endl;

        getch();
#endif

    }

    if (fabs(val - deathRate / (deathRate + maintenance)) > 1E-5) {
        cout << "Warning: bioMatter::SetDeathAndMaintence - possible programming error 3";

#ifdef __BCplusplus__
        cout << "Press any key to continue" << endl;

        getch();
#endif

    }
}

cnMatter bioMatter::GetCarbon() {
    return carbon + intermed + intermedCO2 + influx + added;
}

double bioMatter::GetCarbonC() {
    return carbon.c + intermed.c + intermedCO2.c + influx.c + added.c;
}

double bioMatter::GetNitrogenN() {
    return carbon.n + intermed.n + intermedCO2.n + influx.n + added.n;
}

double bioMatter::GetNitrogenN15() {
    return carbon.n15 + intermed.n15 + intermedCO2.n15 + influx.n15 + added.n15;
}

nitrogen bioMatter::GetNitrogen() {
    nitrogen tmp;

    tmp.SetBoth(GetNitrogenN(), GetNitrogenN15());

    return tmp;
}

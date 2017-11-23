
#include "../base/common.h"
#include "organicMatter.h"
#include "addedMatter.h"
#include "bioMatter.h"
#include "../base/message.h"
#include "../base/bstime.h"
#include "../tools/compare.h"
#include <string.h>

organicMatter::organicMatter(string       Pname,
                             const int    Index,
                             const base * owner):
    base(Pname,
         Index,
         owner)
{

    // Parameters
    clayContentLimit   = clayContent = 0.0;
    ncSlope            = 0.0;
    useNCfractionation = false;
    ncIntercept        = 0.0;
    maxAOM2            = 1.0;
    exchangeRate       = 1E+36;        // Default full exchange, regardless of time step "scaling"
    NH4allwaysLeft     = 2E-9;
    beta               = 0.05;         // Parameter for Langmuir equation (value taken from Mary et al. 1998)
    temperatureAdjust  = -1.0;         // Adjustment parameter for temperature response
    Kirschbaum1        = -3.432;
    Kirschbaum2        = 0.168;
    KirschbaumTopt     = 36.9;
    Arrhenius          = 6352.0;       // deg. Kelvin
    VantHoffQ10        = 2.0;
    refT               = 30.0;
    Rzs                = 0.0112372;    // Default ratio of 13C to 12C (PDB)
    clayContentLimit   = 0.25;         // Upper limit for effect of clay content on decay
    clayRateLimit      = 0.5;          // Relative decay at the upper clay limit
    clayParameter1     = 3.0895;
    clayParameter2     = 2.672;
    clayParameter3     = -7.86;
    clayParameter4     = 1.67;
    clayParameter5     = 1.0;
    waterResponseType  = 1;            // Use SOILN water response by default
    tResponseType      = 1;            // Use Kirschbaum temperature response by default

    // 1: Effect on turnover rates (only specified pools)
    // 2: Effect on biomass maintenance
    clayEffect1       = 1.0;    // Get from conditionfile
    clayEffect2       = 1.0;
    temperatureEffect = -1.0;
    waterEffect       = -1.0;

    // SOILN water parameters below
    // W1-W4 are be soil dependent, and these initialisations can hence be
    // overwritten by the conditionfile
    W1       = 5.0;     // PWP
    W2Offset = 10.0;    // PWP + 10%
    W3Offset = -8.0;    // Saturation - 8%
    W4       = 50.0;    // Saturation
    Y1       = 0.0;     // Parameter for water effect
    Y2       = 0.6;     // --------""--------
    m        = 1.0;     // --------""--------

    // State variables
    carbon_avalibility = 0.0;

    for (int i = 0; i < maxPools; i++) {
        poolList[i] = nullptr;
    }

    todayCO2.Clear();
    NO3.Clear();
    NH4.Clear();
    NH4near.Clear();
}

/*
 * Initiates all soil matter pools with data which each instance reads
 * from the file.
 * filename   input file name
 */
void organicMatter::InitStructure(commonData *& data) {
    data -> setCritical(false);

    int n, first;

    data -> getSectionsNumbers("AddedMatter", &first, &n);

    int numberOfPools = 0;

    for (int i = first; i <= (first + n); i++) {
        matter * m = new addedMatter("AddedMatter", i, this);

        m -> ReadParameters(data);

        poolList[m -> GetPoolName()] = m;

        if (m -> GetPoolName() == 3) {
            cout << flush;
        }

        numberOfPools++;
    }

    data -> getSectionsNumbers("BioMatter", &first, &n);

    for (int i = first; i <= (first + n); i++) {
        matter * m = new bioMatter("BioMatter", i, this);

        m -> ReadParameters(data);

        if (m -> GetPoolName() == 3) {
            cout << flush;
        }

        poolList[m -> GetPoolName()] = m;

        numberOfPools++;
    }

    data -> getSectionsNumbers("Matter", &first, &n);

    for (int i = first; i <= (first + n); i++) {
        matter * m = new matter("Matter", i, this);

        m -> ReadParameters(data);

        poolList[m -> GetPoolName()] = m;

        if (m -> GetPoolName() == 3) {
            cout << flush;
        }

        numberOfPools++;
    }

    if (numberOfPools < 1) {
        theMessage -> FatalError("Pools are not properly defined in structurefile.");
    }

    if (data -> FindSection("Parameters")) {
        data -> FindItem("TemperatureResponse", &tResponseType);
        data -> FindItem("WaterResponse", &waterResponseType);
        data -> FindItem("W1", &W1);
        data -> FindItem("W4", &W4);
        data -> FindItem("W2Offset", &W2Offset);
        data -> FindItem("W3Offset", &W3Offset);
        data -> FindItem("Y1", &Y1);
        data -> FindItem("Y2", &Y2);
        data -> FindItem("m", &m);
        data -> FindItem("ClayContentLimit", &clayContentLimit);
        data -> FindItem("ClayRateLimit", &clayRateLimit);
        data -> FindItem("TemperatureAdjust", &temperatureAdjust);
        data -> FindItem("Kirschbaum1", &Kirschbaum1);
        data -> FindItem("Kirschbaum2", &Kirschbaum2);
        data -> FindItem("KirschbaumTopt", &KirschbaumTopt);
        data -> FindItem("Arrhenius", &Arrhenius);
        data -> FindItem("VantHoffQ10", &VantHoffQ10);
        data -> FindItem("ReferenceTemperature", &refT);
        data -> FindItem("ClayParameter1", &clayParameter1);
        data -> FindItem("ClayParameter2", &clayParameter2);
        data -> FindItem("ClayParameter3", &clayParameter3);
        data -> FindItem("ClayParameter4", &clayParameter4);
        data -> FindItem("ClayParameter5", &clayParameter5);

        if (data -> FindItem("NCslope", &ncSlope)) {
            useNCfractionation = true;

            data -> FindItem("NCintercept", &ncIntercept);
            data -> FindItem("MaxAOM2", &maxAOM2);

            if ((maxAOM2 > 1.0) || (maxAOM2 < 0.0)) {
                theMessage -> FatalError("Model parameter 'MaxAOM2' has illegal value");
            }
        }
    }
}

double organicMatter::GetRatio(int poolName) {
    bioMatter * b = nullptr;

    b = (bioMatter *) GetPoolPointer(poolName);

    double d = b -> GetDeathRate();
    double m = b -> GetMaintenanceRate();

    return d / (d + m);
}

double organicMatter::GetE(int poolName) {
    bioMatter * b = (bioMatter *) GetPoolPointer(poolName);

    return b -> GetUtilizationEfficiency();
}

double organicMatter::GetfSMB2() {
    addedMatter * a = nullptr;

    a = (addedMatter *) GetPoolPointer(1);

    return a -> GetConnectionFraction(3);
}

/*
 * Calc effect of clay content on processes
 * @param clay - clay fraction (0-1)
 */
void organicMatter::updateClayEffects(double clay) {
    clayEffect1 = 1.0 - 2.0 * min(0.25, clay);    // Clay function taken from DAISY;

    // clayeffect2 like Petersen 2005 CN-Sim I l(clay)
    bioMatter * bio = (bioMatter *) GetPoolPointer(2);    // pool 2 is SMB1

    // Double principle December 2002. Model pools AOM1, AOM2, SMB1, SMB2, SMR, NOM, IOM.
    // Effect on total turnover like DAISY, and unity at 0 % clay.
    // if (clayParameter5 <= 0.0) { CK: not used in this function
    // theMessage -> FatalError("'ClayParameter5 must be above zero");
    double E = GetE(2);    // pool 2 is SMB1

    // an utilisation efficiency (ESMB) of carbon in assimilated organic substrate.
    // The fraction 1-ESMB of the incoming carbon is lost from the
    // system as CO2 . ESMB is set to 0.6, according to Knapp et al.
    // (1983), Molina et al. (1983) and Hansen et al. (1991).
    double fSMB2  = GetfSMB2();
    double fSMB1  = 1 - fSMB2;
    double r2     = GetRatio(3);    // pool 3 is SMB2
    double R0     = clayParameter4 * (1.85 + 1.6);
    double h0     = (1.0 / (R0 + 1.0));
    double r1     = h0 / (E * (fSMB1 + fSMB2 * E * r2));
    double rCheck = bio -> GetTurnoverRatio();

    rCheck = fabs(r1 / rCheck - 1.0);

    if (rCheck > 0.02) {
        cout << endl << "Turnover rate for 'SMB1' " << bio -> GetTurnoverRatio()
             << " inconsistent with clay function. New value set at " << r1 << endl;
    }

    bio -> SetDeathAndMaintence(r1);

    // eq6: R = 1.67 * (1.85 + 1.6 * exp(-7.86 * clay)
    // eq7: h = 1 / (R+1)
    double R  = clayParameter4 * (1.85 + 1.6 * exp(clayParameter3 * clay));
    double h  = 1.0 / (R + 1.0);
    double lX = (-r1 * fSMB1 + sqrt(pow(r1 * fSMB1, 2) + 4.0 * h * r1 * r2 * fSMB2))
    		/ (2.0 * E * r1 * r2 * fSMB2);

    // CN-SIm I: The function (l(X)) is scaled by a constant, so that it is one at zero clay
    // content.
    double lX0 = (-r1 * fSMB1 + sqrt(pow(r1 * fSMB1, 2) + 4.0 * h0 * r1 * r2 * fSMB2))
                 / (2.0 * E * r1 * r2 * fSMB2);    // l(clay=0)

    double h_eq10 = E * r1 * lX * (fSMB1 + (fSMB2 * E * r2 * lX));

    //cout << "clay " << clay << ", lX: " << lX << ", lX0: " << lX0
    	//	<< ", h: " << h << ", h(eq10): " << h_eq10 << endl;
    // CK: lX0 is zero, so it works

    clayEffect2 = lX;

    if ((clayEffect2 < 0.0) || (clayEffect2 > 1) || (clayEffect1 < 0.0) || (clayEffect1 > 1)) {
        theMessage -> WarningWithDisplay("organicMatter::updateClayEffects - ",
                                         "'clayEffect2' or 'clayEffect1' cannot be out of [0,1]");
    }
}

/*
 * Returns the effect of temperature on processes
 * temp - Soil temperature [°C]
 * @param temp
 * @param responseType
 * @return
 * responseType
 * 0 : Temperature response not used
 * 1 : Kirschbaum   - default adjusted to unity at 10 deg.
 * 2 : ROTHC
 * 3 : DAISY
 * 4 : Arrhenius    - default adjusted to unity at 10 deg.
 * 5 : Van't Hoff   - defualt adjusted to unity at 10 deg.
 * 6 : Value used directly
 */
double organicMatter::TemperatureEffect(double temp,
        int                                    responseType) {
    if (temperatureEffect > 0.0) {
        return temperatureEffect;    // Return "fixed" value, regardless of settings
    }

    double retVal = 1.0;

    if (temperatureAdjust < 0) {
        temperatureAdjust = 1.0;

        if ((responseType == 1)              // Kirschbaum
                || (responseType == 4)       // Arrhenius
                || (responseType == 5)) {    // Van't Hoff

            // ... CK: why not for ROTHC / Daisy?
            // Adjust to give unity output at 10 deg.
            temperatureAdjust = 1 / TemperatureEffect(10.0, responseType);
        }
    }

    switch (responseType) {
        case 0 :    // Temperature response not used
            retVal = 1.0;

            break;

        case 1 :    // Kirschbaum
            retVal = temperatureAdjust * exp(Kirschbaum1 + Kirschbaum2 * temp * (1.0 - 0.5 * temp / KirschbaumTopt));

            break;

        case 2 :    // ROTHC
            if (temp <= -17) {
                retVal = 0.0;
            } else {

                // Theoretically -18.3, but return value is so low for -17
                // that this is choosen to avoid numerical problems
                retVal = temperatureAdjust * 47.9 / (1.0 + exp(106.0 / (temp + 18.3)));
            }

            break;    // To get almost exact matches with RothC,

        // use parameters 106.058 a 18.2715 instead. Don't know why.
        case 3 :    // DAISY
            if (temp <= 0) {
                retVal = 0.0;
            } else if (temp <= 20) {
                retVal = 0.1 * temperatureAdjust * temp;
            } else {
                retVal = temperatureAdjust * exp(0.47 - 0.027 * temp + 0.00193 * temp * temp);
            }

            break;

        case 4 :    // Arrhenius
            retVal = temperatureAdjust * exp(Arrhenius * (temp - refT) / ((refT + 273.2) * (temp + 273.2)));

            break;

        case 5 :    // Van't Hoff
            retVal = temperatureAdjust * pow(VantHoffQ10, (temp - refT) / 10.0);

            break;

        default :
            theMessage -> FatalError("Illegal state of temperature effect type");

            break;
    }

    if (retVal < 0.0) {
        theMessage -> FatalError("Temperature effect negative");
    }

    return retVal;
}

/*
 * Returns the effect of soil water status on soil organic matter turnover
 * Return value range 0-1
 * water - Soil water status
 * responseType
 * 0 : Water response not used
 * 1 : Like SOILN
 * 2 : Value used directly
 */
double organicMatter::SoilWaterEffect(double water,
        int                                  responseType) {
    if (waterEffect > 0.0) {
        return waterEffect;    // Return "fixed" value, regardless of settings
    }

    double f = 1.0;

    switch (responseType) {
        case 0 :    // Water response not used
            f = 1.0;

            break;

        case 1 : {
            if (W2Offset < 0) {
                theMessage -> FatalError("W2Offset can not be negative (soil water function)");
            }

            if (W3Offset > 0) {
                theMessage -> FatalError("W3Offset can not be positive (soil water function)");
            }

            double W2 = W1 + W2Offset;
            double W3 = W4 + W3Offset;

            if (W2 > W3) {
                theMessage -> FatalError("W2 is larger than W3 (soil water function)");
            }

            f = Y1;

            if (water > W1) {
                f = 1.0;

                if (water <= W2) {
                    f = Y1 + (1.0 - Y1) * pow((water - W1) / (W2 - W1), m);
                } else if (water > W3) {
                    f = 1.0 - (1.0 - Y2) * pow((water - W3) / (W4 - W3), m);
                }

                f = max(0.0, f);
            }

            break;
        }

        case 2 :    // Value used directly
            f = max(0.0, water);

            break;

        default :
            theMessage -> FatalError("Illegal state of water effect type");

            break;
    }

    if (f < 0.0) {
        theMessage -> FatalError("Water effect negative");
    }

    return f;
}

/*
 *
 * Returns a pointer to a given pool
 * poolName - name of pool for which a pointer is requested
 *
 */
matter * organicMatter::GetPoolPointer(int poolName) {
    if (poolName == 8) {
        return nullptr;
    } else if (!poolList[poolName]) {
        theMessage -> FatalError("Pool [", conv.IntToString(poolName), "] not found");

        return nullptr;
    } else {
        return poolList[poolName];
    }
}

double organicMatter::GetCarbonInPoolC(int s) {
    return GetPoolPointer(s) -> GetCarbonC();
}

nitrogen organicMatter::GetNitrogenInPool(int s) {
    return GetPoolPointer(s) -> GetNitrogen();
}

double organicMatter::GetNitrogenInPoolN(int s) {
    return GetPoolPointer(s) -> GetNitrogenN();
}

double organicMatter::GetNitrogenInPoolN15(int s) {
    return GetPoolPointer(s) -> GetNitrogenN15();
}

double organicMatter::FromAtomExecessToInternal(double excess) {
    return (excess + 0.3663033) / 100.0;
}

void organicMatter::SetFractionation(organicProduct * prod) {
    double pc, pn, nc, fAOM2;
    int    n    = prod -> GetNumOfFractions();
    double fsum = 0.0;

    for (int i = 0; i < n; i++) {
        fsum += prod -> GetCFraction(i);
    }

    if ((fsum < 0.9999)
            && (prod -> GetAmount().c > 0.0)) {    // Fractions have no default value, set according to N/C ratio

        // Assumed to have AOM1 (0) and AOM2 (1), AOM2 beeing the easily decomposable
        pc    = prod -> GetAmount().c;
        pn    = prod -> GetAmount().n;
        nc    = pn / pc;
        fAOM2 = ncIntercept + ncSlope * nc;
        fAOM2 = max(0.0, min(maxAOM2, fAOM2));

        prod -> SetCFraction(0, 1.0 - fAOM2);
        prod -> SetCFraction(1, fAOM2);
        prod -> SetNFraction(0, 1.0 - fAOM2);    // No method for N fractionation decided yet !!!
        prod -> SetNFraction(1, fAOM2);
    }
}

/*
 * Updates CO2 emmision variables
 * co2 - CO2-C emmision
 */
void organicMatter::CO2emmision(cnMatter co2) {
    todayCO2 = todayCO2 + co2;
}

void organicMatter::SetMineralNitrogen(nitrogen nitrate,
        nitrogen                                ammonium) {
    if ((NH4.n < -1E-12) || (NO3.n < -1E-12)) {
        theMessage -> FatalError("organicMatter::SetMineralNitrogen - negative value for mineral nitrogen");
    }

    NH4 = ammonium;
    NO3 = nitrate;
}

void organicMatter::GetMineralNitrogen(nitrogen & nitrate,
        nitrogen &                                ammonium) {
    ammonium = NH4;
    nitrate  = NO3;
}

/*
 * A comparatively crude mimic of the indirect-MIT theory
 */
void organicMatter::EquilibrateN15(cnMatter & c) {
    if ((c.n > 1E-36) && (NH4.n > 1E-36)) {
        double q = (c.n15 + NH4.n15) / (c.n + NH4.n);

        NH4.n15 = NH4.n * q;
        c.n15   = c.n * q;
    }
}

void organicMatter::ExchangeNitrogen(nitrogen * nit) {
    if ((NO3.n + NH4.n) < -nit -> n) {
        theMessage -> FatalError("organicMatter::ExchangeNitrogen - total mineral N content below zero");
    }

    if (NH4.n < -1E-9) {
        theMessage -> FatalError("organicMatter::ExchangeNitrogen - NH4 content below zero");
    }

    if (NO3.n < -1E-9) {
        theMessage -> FatalError("organicMatter::ExchangeNitrogen - NO3 content below zero");
    }

    if (nit -> n < 0) {            // Imobilisation
        double f    = 1.0;
        double ntot = max(0.0, NH4.n - NH4allwaysLeft) + NO3.n;

        if (NH4.n <= NH4allwaysLeft) {
            f = 0.0;
        } else {
            if (NO3.n > 0.0) {     // Langmuir equation
                f = (NH4.n / ntot) / (beta + (1.0 - beta) * NH4.n / ntot);
            }

            f = min(f, max(0.0, NH4.n - NH4allwaysLeft) / (-nit -> n));
        }

        if ((f < 0.0) || (f > 1.0)) {
            theMessage -> Warning("organicMatter::ExchangeNitrogen - f must be in the range [0;1]");
        }

        f = max(0.0, min(1.0, f));

        nitrogen fromNH4 = *nit * f;
        nitrogen fromNO3 = *nit * (1.0 - f);
        double   f1      = 0.0;    // From "near" NH4 pool

        if (NH4near.n > 1E-9) {
            if (NH4near.n >= -fromNH4.n) {
                f1 = 1.0;
            } else {
                f1 = NH4near.n / (-fromNH4.n);
            }
        }

        fromNH4.n15 = 0.0;

        if (NH4.n > 1E-15) {
            if ((NH4near.n > 1E-9) && (NH4.n - NH4near.n) > 1E-9) {
                fromNH4.n15 = fromNH4.n
                              * ((1.0 - f1) * (NH4.n15 - NH4near.n15) / (NH4.n - NH4near.n)
                                 + f1 * NH4near.n15 / NH4near.n);
            } else {
                fromNH4.n15 = fromNH4.n * NH4.n15 / NH4.n;
            }
        }

        fromNO3.n15 = 0.0;

        if (NO3.n > 1E-15) {
            fromNO3.n15 = fromNO3.n * NO3.n15 / NO3.n;
        }

        NH4         = NH4 + fromNH4;
        NH4near.n   = max(0.0, NH4near.n + fromNH4.n);
        NH4near.n15 = max(0.0, NH4near.n15 + fromNH4.n15);
        NO3         = NO3 + fromNO3;

        if (NO3.n < -1E-9) {
            theMessage -> FatalError("organicMatter::ExchangeNitrogen - NO3 content below zero");
        }

        nit -> n15 = fromNH4.n15 + fromNO3.n15;
    } else {    // Mineralisation
        NH4     = NH4 + *nit;
        NH4near = NH4near + *nit;
    }

    if (NH4.n < -1E-9) {
        theMessage -> FatalError("organicMatter::ExchangeNitrogen - NH4 content below zero");
    }

    if (NO3.n < -1E-9) {
        theMessage -> FatalError("organicMatter::ExchangeNitrogen - NO3 content below zero");
    }
}

double organicMatter::GetTotalSystemN() {
    double retval = 0;

    for (int i = 0; i < maxPools; i++) {
        if (poolList[i]) {
            retval = retval + poolList[i] -> GetNitrogenN();
        }
    }

    retval += NH4.n + NO3.n;

    return retval;
}

double organicMatter::GetTotalCarbonC() {
    double retval = 0;

    for (int i = 0; i < maxPools; i++) {
        if (poolList[i]) {
            retval = retval + poolList[i] -> GetCarbonC();
        }
    }

    return retval;
}

double organicMatter::GetTotalNitrogenN() {
    double retval = 0;

    for (int i = 0; i < maxPools; i++) {
        if (poolList[i]) {
            retval = retval + poolList[i] -> GetNitrogenN();
        }
    }

    return retval;
}

double organicMatter::GetTotalNitrogenN15() {
    double retval = 0;

    for (int i = 0; i < maxPools; i++) {
        if (poolList[i]) {
            retval = retval + poolList[i] -> GetNitrogenN15();
        }
    }

    return retval;
}

nitrogen organicMatter::GetTotalNitrogen() {
    nitrogen retval;

    for (int i = 0; i < maxPools; i++) {
        if (poolList[i]) {
            retval = retval + poolList[i] -> GetNitrogen();
        }
    }

    return retval;
}

/*
 * Initializes the organic pool object
 * clay       - Clay content of soil layer [fraction]
 * carbon     - C content of layer [g/m2]
 * f          - Parameter file
 */
void organicMatter::Initialize(double       clay,
                               double       carbon,
                               commonData * data,
                               int          index,
                               commonData * soilData) {
    clayContent = clay;

    double TotalC = carbon;

    InitStructure(soilData);

    if (data -> FindSection("SoilLayer", index)) {    // Top of inheritance hierarcy
        double checkSum = 0.0;
        double cnRatio  = 11;

        data -> FindItem("CNRatio", &cnRatio);
        data -> FindItem("MinContent", &W1);
        data -> FindItem("MaxContent", &W4);

        double d = 0.0;

        if (data -> FindItem("NH4", &d)) {
            NH4.n = d;
        }

        if (data -> FindItem("NH4.N15", &d)) {
            NH4.n15 = NH4.n * FromAtomExecessToInternal(d);
        }

        if (data -> FindItem("NO3", &d)) {
            NO3.n = d;
        }

        if (data -> FindItem("NO3.N15", &d)) {
            NO3.n15 = NO3.n * FromAtomExecessToInternal(d);
        }

        d = 0.0;

        for (int i = 0; i < maxPools; i++) {
            if (poolList[i]) {
                if (data -> FindItem(poolList[i] -> GetPoolNameChar(), &d)) {
                    cnMatter c;

                    c.c      = d * TotalC;
                    checkSum += d;

                    string s = poolList[i] -> GetPoolNameChar() + ".N";

                    if (data -> FindItem(s, &d)) {

                        // if (poolList[i]->GetCNratio()>0 && !poolList[i]->IsBiomass())
                        // theMessage->FatalError("Can not set N content in '",poolList[i]->GetPoolName(),"' as this pool has a fixed C/N ratio");
                        c.n = d * TotalC;
                    } else {
                        double r = poolList[i] -> GetDefaultCNratio();

                        if ((r <= 0) && (c.c > 1E-9)) {
                            theMessage -> FatalError("C/N ratio must be set for pool '",
                                                     poolList[i] -> GetPoolNameChar());
                        }

                        if (r > 0) {
                            c.n = c.c / r;
                        }
                    }

                    s = poolList[i] -> GetPoolNameChar() + ".N15";

                    if (data -> FindItem(s, &d)) {
                        c.n15 = c.n * FromAtomExecessToInternal(d);
                    }

                    poolList[i] -> SetCarbon(c);
                }
            }
        }

        if (fabs(1.0 - checkSum) > 1E-3) {
            theMessage -> FatalError("organicMatter::Initialise - Pool carbon fractions do not add to one.");
        }

        double clayContent;

        if (data -> FindItem("ClayContent", &clayContent)) {
            if (clayContent > 1.0) {
                theMessage -> FatalError("Clay fraction can not be bigger than one.");
            }

            updateClayEffects(clayContent);    // update clayEffect1 and clayEffect2

           // cout << "clayEffect1: " << clayEffect1 << ", clayEffect2: " << clayEffect2 << ", fSMB2: " << GetfSMB2()
             //    << ", r2: " << GetRatio(3) << endl;
        }

        if (data -> FindItem("ClayEffect", &clayEffect1)) {                 // Overwrites clay content
            if (clayEffect1 < 0.0) {
                theMessage -> FatalError("The parameter ClayEffect can not be below zero");
            }
        }

        if (data -> FindItem("TemperatureEffect", &temperatureEffect)) {    // "Fixed" temperature effect
            if (temperatureEffect < 0.0) {
                theMessage -> FatalError("The parameter TemperatureEffect can not be below zero");
            }
        }

        if (data -> FindItem("WaterEffect", &waterEffect)) {                // "Fixed" water effect
            if (waterEffect < 0.0) {
                theMessage -> FatalError("The parameter WaterEffect can not be below zero");
            }
        }

        for (int i = 0; i < maxPools; i++) {
            if (poolList[i]) {
                poolList[i] -> Initialize(clayEffect1, clayEffect2);
            }
        }
    }
}

/*
 * Performs mineralisation of the organic matter pools in the soil layer.
 * temp          - Soil temperature [ï¿½C]
 * pFValue       - Soil water status as a pF-value
 * nitrate_mob   - Nitrate in the mobile water phase [g N/mï¿½/d]
 * nitrate_imob  - Nitrate in the immobile water phase [g N/mï¿½/d]
 * ammonium_mob  - Ammonium in the mobile water phase [g N/mï¿½/d]
 * ammonium_imob - Ammonium in the immobile water phase [g N/mï¿½/d]
 * Returns CO2 evolution
 */
double organicMatter::Update(double     temp,
                             double     pFValue,
                             nitrogen * nitrate,
                             nitrogen * ammonium) {
    if (nitrate -> n < -1E-9) {
        theMessage -> FatalError("organicMatter::Update - function called with nitrate below zero");
    }

    if (ammonium -> n < -1E-9) {
        theMessage -> FatalError("organicMatter::Update - function called with anmmnium below zero");
    }

    carbon_avalibility = 0.0;

    // Calculate frations of nitrate and ammonium in mobile water phases
    NO3 = *nitrate;
    NH4 = *ammonium;

    double tot = GetTotalSystemN();

    // Decompose
    cnMatter c;

    // 1. Update all pools, outflux that would cause immobilisation in
    // a receiver pool is stored in a "transit" pool.
    double fT                = TemperatureEffect(temp, tResponseType);
    double environmentEffect = fT * SoilWaterEffect(pFValue, waterResponseType);

    for (int j = 0; j < maxPools; j++) {
        if (poolList[j]) {
            if (!poolList[j] -> IsBiomass()) {
                carbon_avalibility += poolList[j] -> PotentialCarbonTurnover();
            }

            c = poolList[j] -> GetCarbon();

            poolList[j] -> Update(environmentEffect);
        }
    }

    // 2. Catch all influx, none off which would cause immobilisation
    for (int j = 0; j < maxPools; j++) {
        if (poolList[j]) {
            poolList[j] -> CatchInflux(c);
        }
    }

    // 3. Calculate the total potential immobilisation and the available N
    double availN = max(0.0, NO3.n + max(0.0, NH4.n - NH4allwaysLeft));

    // max takes care of minute rounding errors
    // All N considered avaliable !!!
    // 4. Calculate the remaining total C turnover
    double Cpot = 0.0;

    for (int j = 0; j < maxPools; j++) {
        if (poolList[j]) {
            Cpot += poolList[j] -> ResidualCarbon();
        }
    }

    // 5. For each pool determine the individual ratio, export this ratio
    // and put the rest back
    for (int j = 0; j < maxPools; j++) {
        if (poolList[j]) {
            double Nwanted = -poolList[j] -> PotentialNitrogenMineralisation();

            if (Nwanted > 0.0) {
                double Cf = poolList[j] -> ResidualCarbon() / Cpot;
                double rt = min(1.0, availN * Cf / Nwanted);

                poolList[j] -> FinalExport(rt);
            } else {
                poolList[j] -> FinalExport(1.0);    // Probably unnescescary
            }
        }
    }

    NH4near = NH4near * max(0.0, min(1.0, (1.0 - exchangeRate)));

    // 6. Transfer the last influxes to the pool. All theses influxes will
    // cause immobilisation.
    for (int j = 0; j < maxPools; j++) {
        if (poolList[j]) {
            poolList[j] -> CatchInflux(c);
        }
    }

    // Decomposition finished
    if (NO3.n < 0.0) {
        theMessage -> Warning("organicMatter::Update - NO3 below zero");
        NO3.Clear();
    }

    if (NH4.n < 0.0) {
        theMessage -> Warning("organicMatter::Update - NH4 below zero");
        NH4.Clear();
    }

    *nitrate  = NO3;
    *ammonium = NH4;

    double tot1 = GetTotalSystemN();

    if (fabs(tot - tot1) > 1E-5) {
        theMessage -> Warning("organicMatter::Update - imbalance in N");
    }

    cnMatter RetVal = todayCO2;

    todayCO2.Clear();

    return RetVal.c;
}

/*
 *
 * Return carbon in organic matter pools [g C/mï¿½]
 *
 */
double organicMatter::GetCarbon() {
    double cn = GetTotalCarbonC();

    if (cn < 0.0) {
        theMessage -> FatalError("organicMatter::GetCarbon - returns negative value");
    }

    return cn;
}

/*
 * Return nitrogen in organic matter pools [g N/mï¿½]
 */
nitrogen organicMatter::GetNitrogen() {
    nitrogen TotalNitrogen = GetTotalNitrogen();

    if (TotalNitrogen.n < 0.0) {
        theMessage -> FatalError("organicMatter::GetNitrogen - returns negative value");
    }

    return TotalNitrogen;
}

double organicMatter::GetNitrogenN15() {
    double TotalNitrogen = GetTotalNitrogenN15();

    if (TotalNitrogen < 0.0) {
        theMessage -> FatalError("organicMatter::GetNitrogen - returns negative value");
    }

    return TotalNitrogen;
}

double organicMatter::GetNitrogenN() {
    double TotalNitrogen = GetTotalNitrogenN();

    if (TotalNitrogen < 0.0) {
        theMessage -> FatalError("organicMatter::GetNitrogen - returns negative value");
    }

    return TotalNitrogen;
}

/*
 * Return carbon in organic matter pools with a specific name [g C/mï¿½]
 * name - Pool name
 */
double organicMatter::GetPoolCarbon(int name) {
    return GetCarbonInPoolC(name);
}

/*
 * Return nitrogen in organic matter pools with a specific name [g N/mï¿½]
 * name - Pool name
 */
nitrogen organicMatter::GetPoolNitrogen(int name) {
    return GetNitrogenInPool(name);
}

double organicMatter::GetPoolNitrogenN(int name) {
    return GetNitrogenInPoolN(name);
}

double organicMatter::GetPoolNitrogenN15(int name) {
    return GetNitrogenInPoolN15(name);
}

/*
 * Add an organic product to the organic matter
 * product - Organic product object
 */
void organicMatter::AddProduct(organicProduct * product) {
    double tot = GetTotalSystemN() + product -> GetTotalNitrogen().n;

    if (product -> GetOrganicNitrogenN() < -1E-36) {
        theMessage -> FatalError("organicMatter::AddProduct - The added product has negative N content");
    }

    if (useNCfractionation) {
        SetFractionation(product);
    }

    double f = product -> GetCarbonFraction();

    if ((product -> GetAmount() * product -> GetCarbonFraction()).n < 0.0) {
        theMessage -> FatalError("organicMatter::AddProduct - negative amount of N added");
    }

    if ((product -> GetAmount() * product -> GetCarbonFraction()).c < 0.0) {
        theMessage -> FatalError("organicMatter::AddProduct - negative amount of C added");
    }

    double check = 0.0;

    for (int i = 0; i < product -> GetNumOfFractions(); i++) {
        if (product -> GetDestPool(i) == 8) {
            cnMatter CO2directly = product -> GetAmount() * product -> GetCFraction(i) * f;

            check += CO2directly.c;

            CO2emmision(CO2directly);    // Direct release of CO2
        }
    }

    // Test if animal manure needs readjustment of C fractionation
    // Coding specific for the CN-SIM model, january 2003
    double   f0  = product -> GetCFraction(0);
    double   f1  = product -> GetCFraction(1);
    double   f2  = product -> GetCFraction(2);
    matter * nom = nullptr;

    nom = (matter *) GetPoolPointer(5);

    if (!nom) {
        theMessage -> FatalError("The pool 'NOM' was not found");
    }

    double maxCToNOM = 0.0;

    if (product -> GetCNratio() > 0.0) {
        maxCToNOM = nom -> GetCNratio() / product -> GetCNratio();
    }

    if (maxCToNOM < f2) {    // Adjust to avoid program termination
        double diff   = f2 - maxCToNOM;
        double factor = (diff + f0 + f1) / (f0 + f1);

        product -> SetCFraction(0, f0 * factor);
        product -> SetCFraction(1, f1 * factor);
        product -> SetCFraction(2, maxCToNOM);
    }

    double fsum = 0.0;
    double used = 0.0;
    double CN;
    int    num;

    for (int i = 0; i < maxPools; i++) {    // For pools with fixed C/N ratio
        if (poolList[i]) {
            num = 0;

            poolList[i] -> TestAddProduct(product, &check, &CN, &num, false);

            if (num > 0) {                  // Match found
                fsum += product -> GetNFraction(num);
                used += product -> GetCFraction(num) * product -> GetCNratio() / CN;
            }
        }
    }

    if (doubleGreater(used, 1.0, 0.0000001)) {
        theMessage -> FatalError(
            "organicMatter::AddProduct - The added product did not contain enough N to match consistently with pools");
    }

    double   factor = (1.0 - used) / (1.0 - fsum);
    cnMatter amount = product -> GetAmount();

    amount.n   = amount.n * factor;
    amount.n15 = amount.n15 * factor;

    product -> SetAmount(amount);

    for (int i = 0; i < maxPools; i++) {    // For pools without fixed C/N ratio
        if (poolList[i]) {
            poolList[i] -> TestAddProduct(product, &check, &CN, &num, true);
        }
    }

    if (fabs(check - amount.c * f) > 1E-6) {
        theMessage -> FatalError(
            "organicMatter::AddProduct - The added product was not consistently matched with pools for C");
    }

    double tot1 = GetTotalSystemN();

    if (fabs(tot - tot1) > 1E-5) {
        theMessage -> Warning("organicMatter::Update - imbalance in N");
    }
}

/*
 * Fraction (0-1) : fraction of SMB killed
 */
void organicMatter::KillSMB(double Fraction) {
    int count = 0;

    for (int i = 0; i < maxPools; i++) {
        if (poolList[i]) {
            if ((2 == poolList[i] -> GetPoolName()) || (3 == poolList[i] -> GetPoolName())) {
                count++;

                // can only be biomatter pool... perhaps it is dirty?
                (dynamic_cast<bioMatter *>(poolList[i])) -> ExtraTurnover(Fraction);
            }
        }
    }

    if (count != 2) {
        theMessage -> FatalError("organicMatter::KillSMB - SMB pool(s) not found");
    }
}

double organicMatter::GetPoolDecompositionRate(int name) {
    return GetPoolPointer(name) -> GetDecompositionRate();
}

organicMatter::organicMatter(const organicMatter & source):
    base(source)
{
    if (&source) {
        clayContentLimit   = clayContent = 0.0;
        NH4allwaysLeft     = 0.0;
        ncSlope            = source.ncSlope;
        useNCfractionation = source.useNCfractionation;
        ncIntercept        = source.ncIntercept;
        maxAOM2            = source.maxAOM2;
        exchangeRate       = source.exchangeRate;
        beta               = source.beta;
        temperatureAdjust  = source.temperatureAdjust;
        Kirschbaum1        = source.Kirschbaum1;
        Kirschbaum2        = source.Kirschbaum2;
        KirschbaumTopt     = source.KirschbaumTopt;
        Arrhenius          = source.Arrhenius;
        VantHoffQ10        = source.VantHoffQ10;
        refT               = source.refT;
        Rzs                = source.Rzs;
        clayContentLimit   = source.clayContentLimit;
        clayRateLimit      = source.clayRateLimit;
        clayParameter1     = source.clayParameter1;
        clayParameter2     = source.clayParameter2;
        clayParameter3     = source.clayParameter3;
        clayParameter4     = source.clayParameter4;
        clayParameter5     = source.clayParameter5;
        clayEffect1        = source.clayEffect1;
        clayEffect2        = source.clayEffect2;
        waterResponseType  = source.waterResponseType;
        tResponseType      = source.tResponseType;
        temperatureEffect  = source.temperatureEffect;
        waterEffect        = source.waterEffect;
        W1                 = source.W1;
        W2Offset           = source.W2Offset;
        W3Offset           = source.W3Offset;
        W4                 = source.W4;
        Y1                 = source.Y1;
        Y2                 = source.Y2;
        m                  = source.m;

        // State variables
        carbon_avalibility = source.carbon_avalibility;

        for (int i = 0; i < maxPools; i++) {
            if (source.poolList[i]) {
                poolList[i] = source.poolList[i] -> clone();
            } else {
                poolList[i] = nullptr;
            }
        }

        for (int i = 0; i < maxPools; i++) {    // To interconnect pools in the new structure
            if (poolList[i]) {
                poolList[i] -> SetOwner(this);
                poolList[i] -> Initialize(clayEffect1, clayEffect2);
            }
        }

        todayCO2 = source.todayCO2;
        NO3      = source.NO3;
        NH4      = source.NH4;
        NH4near  = source.NH4near;
    } else {
        theMessage -> FatalError("Attempt to copy 'organicMatter' with nullptr pointer");
    }
}

organicMatter::~organicMatter() {
    for (int i = 0; i < maxPools; i++) {
        if (poolList[i]) {
            delete poolList[i];
        }
    }
}

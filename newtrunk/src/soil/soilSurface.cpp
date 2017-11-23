// Soil surface
// (c) Jørgen E. Olesen, Statens Planteavlsforsøg
// E-mail: jeo@pvf.sp.dk
// Changes:
// JEO 27-07-1997: Comments added
// JEO 30-07-1997: Snow accumulation removed
// JEO 27-10-1997: New soil evaporation formula



#include "../base/common.h"
#include "soilSurface.h"
#include "../base/message.h"
#include "../base/bstime.h"
#include "../base/settings.h"

soilSurface::soilSurface():
    base()
{
    BeforenitrateSolids = BeforenitrateDisolved = false;
    beforeAmmoniumSolid = beforeAmmoniumDisolved = false;
    firstProduct        = nullptr;
    lastProduct         = nullptr;

    ammoniumSolid.Clear();
    nitrateSolid.Clear();
    ammoniumDisolved.Clear();
    nitrateDisolved.Clear();
    ammoniaVolatilisation.Clear();
    RunOff_N.Clear();

    nitrogenDisolvedConstant = 0.05;
    nitrogenLeachedConstant  = 0.05;
    evaporation              = 0;
    temperature              = 0;
    water                    = 0;
    RunOff                   = 0;
    Chloride                 = 0;
}

void soilSurface::Initialize(string soilFileName) {
    commonData data;

    data.setCritical(false);
    data.readFile(soilFileName);
    data.FindSection("SurfaceParameters");
    data.FindItem("nitrogenDisolvedConstant", &nitrogenDisolvedConstant);
    data.FindItem("nitrogenLeachedConstant", &nitrogenLeachedConstant);
}

/*
 * Updates the soil surface corresponding to the passing of one day.
 * surplus            - Water to be added (and drained) from soil surface [mm/d]
 * ePotToSoil         - Potential evapotranspiration from soil [mm/d]
 * sumWaterContent    - Content of plant available water in the evaporation depth [mm]
 * sumWaterCapacity   - Capacity for plant availble water in the evaporation depth [mm]
 * airTemperature     - Air temperature [°C]
 */
void soilSurface::Update(double * surplus,
                         double   ePotToSoil,
                         double   relativeSurfaceWater,
                         double   maxEvaporation,
                         double   evaporationContent,
                         double   airTemperature) {
    temperature = airTemperature;

    // Calculate soil evaporation
    double E;

    // New method. Modified after Plauborg & Olesen (MVTOOL) and Plauborg (Ph.D. dissertation)
    if (ePotToSoil <= evaporationContent) {
        E = 1.0;
    } else {
        E = 0.65 * relativeSurfaceWater;
    }

    evaporation = min(maxEvaporation, E * ePotToSoil);

    // Update mineral nitrogen contents
    double disolved = min(1.0, nitrogenDisolvedConstant * relativeSurfaceWater + nitrogenLeachedConstant * (*surplus));

    nitrateDisolved  = nitrateDisolved + nitrateSolid * disolved;
    nitrateSolid     = nitrateSolid - nitrateSolid * disolved;
    disolved         = min(1.0, nitrogenDisolvedConstant * relativeSurfaceWater + nitrogenLeachedConstant * (*surplus));
    ammoniumDisolved = ammoniumDisolved + ammoniumSolid * disolved;
    ammoniumSolid    = ammoniumSolid - ammoniumSolid * disolved;

    if ((ammoniumDisolved.n < -1E-6) || (nitrateDisolved.n < -1E-6) || (ammoniumSolid.n < -1E-6)
            || (nitrateSolid.n < -1E-6)) {
        theMessage -> WarningWithDisplay("SoilSurface::Update - negative n concentrations");
    }
}

void soilSurface::SurfaceRunOff() {
    const double maxWater = 25.0;    // 125.0;

    RunOff = 0.0;

    RunOff_N.Clear();

    if (water > maxWater) {
        double f = max(0.0, (water - maxWater)) * 0.1 / water;

        nitrateDisolved  = nitrateDisolved + nitrateSolid * 0.5;
        ammoniumDisolved = ammoniumDisolved + ammoniumSolid * 0.5;
        RunOff           = f * water;
        RunOff_N         = (nitrateDisolved + ammoniumDisolved) * f;
        water            -= f * water;
        nitrateDisolved  = nitrateDisolved - nitrateDisolved * f;
        ammoniumDisolved = ammoniumDisolved - ammoniumDisolved * f;
        nitrateDisolved  = nitrateDisolved + nitrateSolid * 0.5;
        ammoniumDisolved = ammoniumDisolved + ammoniumSolid * 0.5;

        nitrateSolid.Clear();
        ammoniumSolid.Clear();
    }

    if ((ammoniumDisolved.n < -1E-6) || (nitrateDisolved.n < -1E-6) || (ammoniumSolid.n < -1E-6)
            || (nitrateSolid.n < -1E-6)) {
        theMessage -> WarningWithDisplay("SoilSurface::SurfaceRunOff - negative n concentrations");
    }
}

void soilSurface::ExtractWater(double * ew) {
    double extract = min(water, *ew);

    water -= extract;
    *ew   = extract;
}

double soilSurface::ExtractWater() {
    double extract = water;

    water = 0.0;

    return extract;
}

double soilSurface::ExtractChloride() {
    double extract = Chloride;

    Chloride = 0.0;

    return extract;
}

nitrogen soilSurface::GetTotalNitrogen() {
    nitrogen TotalN = GetAmmoniumSolid() + GetNitrateSolid() + GetAmmoniumDisolved() + GetNitrateDisolved()
                      + GetOrganicNitrogen();

    return TotalN;
}

double soilSurface::GetTotalNitrogenN() {
    double TotalN = GetAmmoniumSolidN() + GetNitrateSolidN() + GetAmmoniumDisolvedN() + GetNitrateDisolvedN()
                    + GetOrganicNitrogenN();

    return TotalN;
}

double soilSurface::GetTotalNitrogenN15() {
    double TotalN = GetAmmoniumSolidN15() + GetNitrateSolidN15() + GetAmmoniumDisolvedN15() + GetNitrateDisolvedN15()
                    + GetOrganicNitrogenN15();

    return TotalN;
}

/*
 * Adds nitrogen to the soil surface
 * addNitrate    - Nitrate to be added [g N/m²]
 * addAmmonium   - Ammonium to be added [g N/m²]
 */
void soilSurface::AddSolidNitrogen(nitrogen addNitrate,
                                   nitrogen addAmmonium) {
    nitrateSolid  = nitrateSolid + addNitrate;
    ammoniumSolid = ammoniumSolid + addAmmonium;

    if ((ammoniumDisolved.n < -1E-6) || (nitrateDisolved.n < -1E-6) || (ammoniumSolid.n < -1E-6)
            || (nitrateSolid.n < -1E-6)) {
        theMessage -> WarningWithDisplay("SoilSurface::AddNitrogen - negative n concentrations");
    }
}

void soilSurface::AddDisolvedNitrogen(nitrogen addNitrate,
        nitrogen                               addAmmonium) {
    nitrateDisolved  = nitrateDisolved + addNitrate;
    ammoniumDisolved = ammoniumDisolved + addAmmonium;

    if ((ammoniumDisolved.n < -1E-6) || (nitrateDisolved.n < -1E-6) || (ammoniumSolid.n < -1E-6)
            || (nitrateSolid.n < -1E-6)) {
        theMessage -> WarningWithDisplay("SoilSurface::AddNitrogen - negative n concentrations");
    }
}

/*
 * Adds an organic (or inorganic) product to the the soil surface
 * product - Organic product object
 */
void soilSurface::AddProduct(const organicProduct * product) {
    organicProduct * p = new organicProduct(*product);

    AddDisolvedNitrogen(p -> GetNitrate(), p -> GetAmmonium());
    p -> ClearNO3_content();
    p -> ClearNH4_content();

    if (!firstProduct) {
        firstProduct = lastProduct = p;
    } else {
        bool             poolResemblance = false;
        organicProduct * aProduct        = firstProduct;

        while (aProduct) {
            if (!poolResemblance && (aProduct -> ProductSimilarity(p))) {
                aProduct -> AddNext(p);

                poolResemblance = true;
            }

            aProduct = aProduct -> Next();
        }

        if (!poolResemblance) {
            bool productAdded = lastProduct -> AddNext(p);

            if (productAdded) {
                lastProduct = p;
            }
        }
    }
}

/*
 * Removes part of an organic product from the soil surface. The removed product
 * is returned as an object in the function call.
 * No   - Product number in list to remove (0..)
 * frac - Fraction of product to be removed
 */
organicProduct * soilSurface::RemoveProduct(int No,
        double                                  frac) {
    organicProduct * p  = firstProduct;
    organicProduct * np = nullptr;

    for (int i = 0; i < No; i++) {
        if (p) {
            p = p -> Next();
        }
    }

    if (p) {
        np = p -> GetFraction(frac);

        p -> Scale(1.0 - frac);
    }

    return np;
}

/*
 * Extract ammonium
 */
nitrogen soilSurface::ExtractAmmoniumDisolved() {
    nitrogen am = ammoniumDisolved;

    ammoniumDisolved.Clear();

    return am;
}

/*
 * Extract nitrate
 */
nitrogen soilSurface::ExtractNitrateDisolved() {
    nitrogen n = nitrateDisolved;

    nitrateDisolved.Clear();

    return n;
}

/*
 * Returns carbon content in products stored on soil surface [g C/m2].
 */
double soilSurface::GetCarbon() {
    organicProduct * p   = firstProduct;
    double           sum = 0.;

    while (p) {
        sum += p -> GetCarbon();
        p   = p -> Next();
    }

    return sum;
}

/*
 * Returns content of organic nitrogen in products on soil surface [g N/m2].
 */
nitrogen soilSurface::GetOrganicNitrogen() {
    organicProduct * p = firstProduct;
    nitrogen         sum;

    sum.Clear();

    while (p) {
        sum = sum + p -> GetOrganicNitrogen();
        p   = p -> Next();
    }

    return sum;
}

int soilSurface::GetProduct(string productName) {
    int              n       = -1;
    organicProduct * current = firstProduct;

    while (current) {
        n++;

        string name = current -> GetName();

        if (name == productName) {
            break;
        }

        current = current -> Next();
    }

    return n;
}

double soilSurface::GetOrganicNitrogenN() {
    organicProduct * p   = firstProduct;
    double           sum = 0;

    while (p) {
        sum = sum + p -> GetOrganicNitrogenN();
        p   = p -> Next();
    }

    return sum;
}

double soilSurface::GetOrganicNitrogenN15() {
    organicProduct * p   = firstProduct;
    double           sum = 0;

    while (p) {
        sum = sum + p -> GetOrganicNitrogenN15();
        p   = p -> Next();
    }

    return sum;
}

/*
 * Returns number of organic product objects stored on soil surface
 */
int soilSurface::GetNumberOfProducts() {
    int              n       = 0;
    organicProduct * current = firstProduct;

    while (current) {
        n++;

        current = current -> Next();
    }

    return n;
}

void soilSurface::PrintNitrateDisolved() {
    static ofstream NitrateDisolvedContent;
    static bool     NitroDisolvedUsed = false;

    if (!NitroDisolvedUsed) {
        NitrateDisolvedContent.open((globalSettings -> getOutputDirectory() + "nitrateDisolved.xls").c_str(), ios::out);

        NitrateDisolvedContent << theTime << "\t";

        NitroDisolvedUsed = true;
    } else {
        NitrateDisolvedContent.open((globalSettings -> getOutputDirectory() + "nitrateDisolved.xls").c_str(), ios::app);

        NitrateDisolvedContent << theTime << "\t";
    }

    NitrateDisolvedContent << BeforenitrateDisolved << "\t" << nitrateDisolved.n << endl;

    NitrateDisolvedContent.close();
}

void soilSurface::PrintNitrateSolid() {
    static ofstream NitrateSolidContent;
    static bool     NitrateSolidUsed = false;

    if (!NitrateSolidUsed) {
        NitrateSolidContent.open((globalSettings -> getOutputDirectory() + "NitrateSolid.xls").c_str(), ios::out);

        NitrateSolidContent << theTime << "\t";

        NitrateSolidUsed = true;
    } else {
        NitrateSolidContent.open((globalSettings -> getOutputDirectory() + "NitrateSolid.xls").c_str(), ios::app);

        NitrateSolidContent << theTime << "\t";
    }

    NitrateSolidContent << BeforenitrateSolids << "\t" << nitrateSolid.n << endl;

    NitrateSolidContent.close();
}

void soilSurface::PrintAmmoniumSolid() {
    static ofstream AmmoniumSolidconent;
    static bool     AmmoniumSolidUsed = false;

    if (!AmmoniumSolidUsed) {
        AmmoniumSolidconent.open((globalSettings -> getOutputDirectory() + "AmmoniumSolidUsed.xls").c_str(), ios::out);

        AmmoniumSolidconent << theTime << "\t";

        AmmoniumSolidUsed = true;
    } else {
        AmmoniumSolidconent.open((globalSettings -> getOutputDirectory() + "AmmoniumSolidUsed.xls").c_str(), ios::app);

        AmmoniumSolidconent << theTime << "\t";
    }

    AmmoniumSolidconent << beforeAmmoniumSolid << "\t" << ammoniumSolid.n << endl;

    AmmoniumSolidconent.close();
}

void soilSurface::PrintAmmoniumDisolved() {
    static ofstream ammoniumDisolvedconent;
    static bool     ammoniumDisolvedUsed = false;

    if (!ammoniumDisolvedUsed) {
        ammoniumDisolvedconent.open((globalSettings -> getOutputDirectory() + "ammoniumDisolved.xls").c_str(), ios::out);

        ammoniumDisolvedconent << theTime << "\t";

        ammoniumDisolvedUsed = true;
    } else {
        ammoniumDisolvedconent.open((globalSettings -> getOutputDirectory() + "ammoniumDisolved.xls").c_str(), ios::app);

        ammoniumDisolvedconent << theTime << "\t";
    }

    ammoniumDisolvedconent << beforeAmmoniumDisolved << "\t" << ammoniumDisolved.n << endl;

    ammoniumDisolvedconent.close();
}

soilSurface::soilSurface(const soilSurface & surface):
    base(surface)
{
    firstProduct = nullptr;
    lastProduct  = nullptr;

    if (&surface) {
        ammoniumSolid            = surface.ammoniumSolid;
        nitrateSolid             = surface.nitrateSolid;
        ammoniumDisolved         = surface.ammoniumDisolved;
        nitrateDisolved          = surface.nitrateDisolved;
        nitrogenDisolvedConstant = surface.nitrogenDisolvedConstant;
        nitrogenLeachedConstant  = surface.nitrogenLeachedConstant;
        evaporation              = surface.evaporation;
        ammoniaVolatilisation    = surface.ammoniaVolatilisation;
        temperature              = surface.temperature;
        water                    = surface.water;
        RunOff                   = surface.RunOff;
        RunOff_N                 = surface.RunOff_N;
        Chloride                 = surface.Chloride;

        organicProduct * current;
        organicProduct * tempProduct;

        current = surface.firstProduct;

        while (current) {
            tempProduct = new organicProduct(*current);

            AddProduct(tempProduct);

            current = current -> Next();

            delete tempProduct;
        }
    }
}

soilSurface::~soilSurface() {
    organicProduct * p1;
    organicProduct * p2;

    p1 = firstProduct;

    while (p1) {
        p2 = p1;
        p1 = p1 -> Next();

        delete p2;
    }
}

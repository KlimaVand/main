/*
 * Created by NJH - 2.6.99
 * Describes a separate free-standing store)
 */

#include "../base/common.h"
#include "slurrystore.h"
#include "../base/IndicatorOutput.h"
#include "../base/climate.h"
#include "../base/controlParameters.h"
#include "../base/nixcbits.h"
#include "../base/settings.h"
#include "../base/constants.h"

slurrystore::~slurrystore() {
    external = true;
}

slurrystore::slurrystore(string        aName,
                         const int     aIndex,
                         const base *  aOwner,
                         commonData *& runData,
                         commonData *& original):
    manurestore(aName,
                aIndex,
                aOwner,
                runData,
                original),
    building(aName,
             aIndex,
             aOwner,
             runData,
             original)
{
    kOMi   = 0;
    kOMs   = 0;
    kOMf   = 0;
    kOMvfa = 0;

    OMi.Clear();
    OMs.Clear();
    OMf.Clear();
    OMvfa.Clear();

    StorageCapacity = 0.0;
    surfaceArea     = 0.0;

    runData -> setCritical(true);
    runData -> FindSection(Name, Index);
    runData -> FindItem("Alias", &Alias);
    runData -> setCritical(false);
    original -> setCritical(true);

    // string sectionNameOrg(aName);
    int    indexInOld;
    string place;

    original -> findSection(aName, 0, "Alias", Alias, &place, &indexInOld);
    original -> FindSection(place, indexInOld);
    original -> FindItem("CrustResistance", &crustResistance);
    runData -> FindItem("CrustResistance", &crustResistance);
    original -> FindItem("SpecificResistance", &specificResistance);
    runData -> FindItem("SpecificResistance", &specificResistance);
    original -> FindItem("StorageCapacity", &StorageCapacity);
    runData -> FindItem("StorageCapacity", &StorageCapacity);
    original -> FindItem("CoverResistance", &coverResistance);
    runData -> FindItem("CoverResistance", &coverResistance);
    original -> FindItem("Kirschbaum1", &Kirschbaum1);
    runData -> FindItem("Kirschbaum1", &Kirschbaum1);
    original -> FindItem("Kirschbaum2", &Kirschbaum2);
    runData -> FindItem("Kirschbaum2", &Kirschbaum2);
    original -> FindItem("KirschbaumOpt", &KirschbaumOpt);
    runData -> FindItem("KirschbaumOpt", &KirschbaumOpt);

    if (globalSettings -> DetailsData.getManureManageDetails()) {
        OpenDetailsFile();
    } else {
        detailsFile = nullptr;
    }

    external = true;
}

slurrystore::slurrystore(slurrystore & s):
    manurestore(s),
    building(s)
{
    theMessage -> FatalError("slurrystore::slurrystore has not been used for a while");

    crustResistance    = s.crustResistance;       // crust resistance to volatization
    specificResistance = s.specificResistance;    // building spec. resistance to volatization
    coverResistance    = s.coverResistance;       // Covers on slurrystore resistance --||--
    ALFAMtype          = s.ALFAMtype;
    kOMi               = 0;
    kOMs               = 0;
    kOMf               = 0;
    kOMvfa             = 0;

    OMi.Clear();
    OMs.Clear();
    OMf.Clear();
    OMvfa.Clear();

    Kirschbaum1 = Kirschbaum2 = KirschbaumOpt = StorageCapacity = 0.0;
}

/*
 * Dailyactions that updates volatAmmonia, N2Emission, N2OEmission, CO2Emission and CH4Emission. Does update Indicat-file with that information
 */
void slurrystore::DailyUpdate() {

    /*
     *      fstream * filehandle = theMessage->GiveDebugHandle();
     * filehandle << " storeV " << GetAmountManure() << " storeN " << GetTotalNStored() << " ";
     * theMessage->CloseDebugHandle();
     */
    building::DailyUpdate();

    double volatAmmonia = 0.0,
           N2Emission   = 0.0,
           N2OEmission  = 0.0,
           CH4Emission  = 0.0,
           CO2Emission  = 0.0;

    switch (theControlParameters -> GetvolatModel()) {
        case 0 : {
            double Nlost, Clost;

            Volatize(&Nlost, &Clost, &volatAmmonia, &N2Emission, &N2OEmission, &CH4Emission, &CO2Emission);
            Nbudget.AddOutput(Nlost);
            Cbudget.AddOutput(Clost);
        }

        break;

        case 1 :
            VolatizeSlurry(&volatAmmonia, &N2Emission, &N2OEmission, &CO2Emission, &CH4Emission);

            break;
    }

    manurestore::DailyUpdate();
    theOutput -> AddIndicator(3543, "35.43 NH3-N volatilisation from manure storage", "kg N", volatAmmonia * 1000.0);
    theOutput -> AddIndicator(3544, "35.44 N2-N volatilisation from manure storage", "kg N", N2Emission * 1000.0);
    theOutput -> AddIndicator(3545, "35.45 N2O-N volatilisation from manure storage", "kg N", N2OEmission * 1000.0);
    theOutput -> AddIndicator(4543, "45.43 CO2-C volatilisation from manure storage", "kg C", CO2Emission * 1000.0);
    theOutput -> AddIndicator(4544, "45.44 CH4-C volatilisation from manure storage", "kg C", CH4Emission * 1000.0);
    GiveIndicator(environmentalIndicator);
}

/*
 * Does callculate volatAmmonia, N2Emission ,N2OEmission, CO2Emission and CH4Emission
 */
void slurrystore::VolatizeSlurry(double * volatAmmonia,
                                 double * N2Emission,
                                 double * N2OEmission,
                                 double * CO2Emission,
                                 double * CH4Emission) {
    if (GetAmountInManureStore() > 0.0) {
        double waterEvap = 0.0;
        double rb, ra, resistance;
        double Ameantemp, Amin_temp, Amax_temp, Aprecip, Aradiation, AEPot, Awindspeed, Ahumidity;

        theClimate -> GetClimate(Ameantemp, Amin_temp, Amax_temp, Aprecip, Aradiation, AEPot, Awindspeed, Ahumidity);

        if (Awindspeed < 0.0001) {
            Awindspeed = 0.0001;
        }

        double height         = 2.0;
        double displacementHt = 0.6 * height;
        double zeroPlane      = 0.001;
        double ustar          = GetFrictionVelocity(height, displacementHt, zeroPlane, Awindspeed);

        rb         = 6.2 * pow(ustar, -0.67);    // Olesen & Summer 94  boundary layer res.   86400 (s->day) inserted by JB
        ra         = (log(height / zeroPlane)) / (VonKarman * ustar);    // ------||--------    Aerodynamic res.
        resistance = rb + ra + coverResistance + crustResistance
                     + specificResistance;                               // rC is the crusts resistance to volatization

        manurestore::Volatize(Ameantemp, Ahumidity, Aprecip, surfaceArea, resistance, volatAmmonia, &waterEvap,
                              N2Emission, N2OEmission, CO2Emission, CH4Emission);

        /*
         *    *theMessage->GiveDebugHandle() << theTime.GetDay() << "," << theTime.GetMonth() << "," << theTime.GetYear() << ","
         * << Ameantemp << "," << volat*1000.0 << endl;
         * theMessage->CloseDebugHandle();
         */
    }
}

void slurrystore::GiveIndicator(int indicatorType) {
    if (indicatorType == environmentalIndicator) {
        double   Vt      = 0,
                 N       = 0,
                 C       = 0;
        manure * aManure = (manure *) theStore -> GetProductPointer();

        if (aManure) {
            Vt = aManure -> GetAmount();           // The total amount of slurry in kg
            N  = Vt * aManure -> GetTotalN().n;    // The total N-fraction
            C  = Vt * aManure -> GetC_content();
        }

        theOutput -> AddStateIndicator(3560, "35.60 N bound in manure storage", "kg N", N * 1000.0);
        theOutput -> AddStateIndicator(4560, "45.60 C bound in manure storage", "kg C", C * 1000.0);

        // theOutput->AddStateIndicator(
        // 1170,"11.70 Slurry in slurrystore", "t", Vt / 1000);
    }
}

/*
 * Calls manurestore's routine - is defined in slurrystore to help in debugging
 */
void slurrystore::RcvManure(const manure * someManure) {
    double       aAmount;
    double       inputC_OMi, inputC_OMs, inputC_OMf, inputC_OMvfa;
    double       inputN_OMi, inputN_OMs, inputN_OMf;
    double const CNratio_OMi = 10.0;

    aAmount      = someManure -> GetAmount();
    inputC_OMi   = aAmount * (1 - someManure -> GetAOM1_fraction() - someManure -> GetAOM2_fraction());
    inputC_OMs   = aAmount * someManure -> GetAOM1_fraction();
    inputC_OMf   = aAmount * someManure -> GetAOM2_fraction() * (1 - someManure -> GetVFA_fraction());
    inputC_OMvfa = aAmount * someManure -> GetAOM2_fraction() * someManure -> GetVFA_fraction();
    inputN_OMi   = inputC_OMi / CNratio_OMi;

    /* necessary to update TAN - either some N is removed or added to TAN to ensure CNratio = 10 in OMi */
    inputN_OMs = aAmount * someManure -> GetAOM1_Nfraction();
    inputN_OMf = aAmount * someManure -> GetAOM2_Nfraction() * (1 - someManure -> GetVFA_fraction());

    cnMatter * inputcnmatter;

    inputcnmatter = new cnMatter(inputC_OMi, 0.0, inputN_OMi, 0.0);

    OMi + *inputcnmatter;

    inputcnmatter = new cnMatter(inputC_OMs, 0.0, inputN_OMs, 0.0);

    OMs + *inputcnmatter;

    inputcnmatter = new cnMatter(inputC_OMf, 0.0, inputN_OMf, 0.0);

    OMf + *inputcnmatter;

    inputcnmatter = new cnMatter(inputC_OMvfa, 0.0, 0.0, 0.0);

    OMvfa + *inputcnmatter;

    delete inputcnmatter;

    manurestore::RcvManure(someManure);
    theOutput -> AddIndicator(1972, "19.72 Slurry produced", "t", someManure -> GetAmount());

    // if (theStore->Getamountstored()>theStore->GetCapacity())
    // theMessage->FatalError(manurestore::GetName()," slurry storage capacity exceeded");
}

/*
 *      Scale the manure storage that receives material from this instore  Note the cumulative effect on capacity
 */
void slurrystore::Scale(double annualAmount) {
    double newCapacity = (StorageCapacity * annualAmount) / 365.0 + GetCapacity();

    manurestore::Scale(newCapacity);    // scales capcity and area via manurestore
}

ostream & operator <<(ostream &   os,
                      slurrystore s) {

    /*
     *  os << setiosflags(ios::left) << setw(40) << s.rT << endl;
     * os << setiosflags(ios::left) << setw(40) << "Cover resistance " << s.rCover << endl;
     */
    return os;
}

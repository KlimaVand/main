
#ifdef NITROSCAPE

#include "../Nitroscape/ecosystinterface.h"
#include "../products/plantItem.h"
#include "../base/unix_util.h"
#include <sstream>
ecosystinterfaceClass::ecosystinterfaceClass(int aIndex,
        string                                   soilFileName,
        double                                   anArea) {
    Index        = aIndex;
    theEcosystem = new ecosystem("ecosytstem", aIndex, this);

    theEcosystem.Initialise(soilFileName, aIndex, anArea);

    harvestStuff              = new linkList<harvInfoClass>;
    totalLAI                  = 0.0;
    ReadyForHarvestOrLater    = true;
    IrrigationDemand          = 0.0;
    AddedChloride             = 0.0;
    irrigationWater           = 0.0;
    irrigationN               = 0.0;
    killAllCrops              = false;    // needs to be reset daily
    TotalEcosystemN           = 0.0;
    TotalEcosystemC           = 0.0;
    MaxCropHeight             = 0.0;
    StandingDMdensity         = 0.0;
    AvailableStandingDM       = 0.0;
    AvailableStandingDMHeight = 0.0;
    dryMatterGrazed           = 0.0;
    grazedHt                  = 0.0;
    thesurfNitrate            = 0.0;
    thesurfAmmonium           = 0.0;
    thesoilNitrate            = 0.0;
    thesoilAmmonium           = 0.0;
    theDepth                  = 0.0;      // Tillage Depth
    theFractionIncorporated   = 0.0;      // Fraction of surface organic matter incorporated
    harvestCode               = 0;
    propGrainHarvested        = 0.0;
    propStrawHarvested        = 0.0;
    ecoOutput                 = new IndicatorOutput;


    string indicatFileName = "Eco";

    std::stringstream IndexString;

    IndexString << Index;

    indicatFileName += IndexString.str() + ".csv";


    string FN1 = indicatFileName;

    ecoOutput -> Initialize(FN1);
}

ecosystinterfaceClass::~ecosystinterfaceClass() {
    delete theEcosystem;
    delete harvestStuff;
    delete ecoOutput;
}

;
void ecosystinterfaceClass::AddDecomposable(string decompName,
        double                                     decompAmount,
        double                                     decompNH4_N,
        double                                     decompNO3_N,
        double                                     decompOrg_N,
        double                                     decompC) {
    decomposable * adecomp = new decomposable(decompName, 0, NULL);

    adecomp -> Setamount(decompAmount);
    adecomp -> SetNO3_content(decompNO3_N / decompAmount);
    adecomp -> SetNH4_content(decompNH4_N / decompAmount);
    adecomp -> SetorgN_content(decompOrg_N / decompAmount);
    adecomp -> SetDryMatter(1.0);
    theEcosystem.AddDecomposable(adecomp);
}

;
void ecosystinterfaceClass::Sow(int    seedIndex,
                                double amountSeedDM,
                                double NitrogenInSeed,
                                double CarbonInSeed) {
    char achar1[20];

    AllocateCrop(achar1, seedIndex);
    theEcosystem.Sow(achar1, amountSeedDM, 1.0, NitrogenInSeed, CarbonInSeed);
}

;
void ecosystinterfaceClass::Tillage(int theType,
        double                          theDepth,
        double                          theFractionIncorporated) {
    soilTreatFields * TillageOp = new soilTreatFields();

    TillageOp -> SetOperationId(theType);
    TillageOp -> SetDepth(theDepth);
    TillageOp -> SetFractionIncorporated(theFractionIncorporated);
    theEcosystem.Tillage(TillageOp);
}

void ecosystinterfaceClass::SetirrigationWater(double amountWater,
        double                                        amountN) {
    water * aWater = new water();

    aWater -> Setamount(amountWater);
    aWater -> SetN_content(amountN / amountWater);
    theEcosystem.SetirrigationWater(aWater);
}

void ecosystinterfaceClass::HarvestOperations(int harvestCode,
        double                                    propGrainHarvested,
        double                                    propStrawHarvested) {
    harvestFields * hrv = new harvestFields();

    hrv -> SetOperationId(harvestCode);
    hrv -> SetFractionGrainHarvested(propGrainHarvested);
    hrv -> SetFractionStrawHarvested(propStrawHarvested);
    theEcosystem.HarvestOperations(hrv);

    double      totalN = 0.0;
    plantItem * anitem;

    for (int i = 0; i < 4; i++) {
        if (hrv -> GetMainProduct(i)) {
            anitem = hrv -> GetMainProduct(i);
            totalN += anitem -> GetAmount() * anitem -> GetAllN().n;
        }

        if (hrv -> GetStrawProduct(i)) {
            anitem = hrv -> GetStrawProduct(i);
            totalN += anitem -> GetAmount() * anitem -> GetAllN().n;
        }
    }

    theOutput -> AddIndicator(environmentalIndicator, "35.20 N in sold plant products", "kg N", totalN * 1000.0);
}

void ecosystinterfaceClass::UpdateStatus() {

    // double totalLAI;
    ReadyForHarvestOrLater = theEcosystem.GetReadyForHarvestOrLater();
    IrrigationDemand       = theEcosystem.GetIrrigationDemand();

    if (killAllCrops) {
        theEcosystem.KillAllCrops();
    }

    MaxCropHeight       = theEcosystem.GetMaxCropHeight();
    StandingDMdensity   = theEcosystem.GetStandingDMdensity();
    AvailableStandingDM = theEcosystem.GetAvailableStandingDM(AvailableStandingDMHeight);
    TotalEcosystemN     = theEcosystem.GetTotalNitrogen();

    crop * aCrop;
    int    numOfCrops = theEcosystem.GetCrops() -> NumOfNodes();

    harvestStuff -> Reset();

    for (int i = 0; i < numOfCrops; i++) {
        aCrop = theEcosystem.GetCrops() -> ElementAtNumber(i);

        harvInfoClass * aharvInfoClass = new harvInfoClass();

        aharvInfoClass -> SetproductCode(0);

        string achar = aCrop -> GetPlantItemName();

        if (achar.length() == 0) {
            achar = "No crop";
        }

        aharvInfoClass -> SetproductName(achar);
        aharvInfoClass -> SetamountDM(aCrop -> GiveDryMatterStorage());
        aharvInfoClass -> SetamountN(aCrop -> NitrogenInStorage().n);
        aharvInfoClass -> SetamountC(aharvInfoClass -> GetamountDM() * 0.46);
        harvestStuff -> InsertLast(aharvInfoClass);

        aharvInfoClass = new harvInfoClass();

        aharvInfoClass -> SetproductCode(1);

        string achar2 = aCrop -> GetStrawItemName();

        aharvInfoClass -> SetproductName(achar2);
        aharvInfoClass -> SetamountDM(aCrop -> GiveDryMatterVegTop());
        aharvInfoClass -> SetamountN(aCrop -> NitrogenInVegTop().n);
        aharvInfoClass -> SetamountC(aharvInfoClass -> GetamountDM() * 0.46);
        harvestStuff -> InsertLast(aharvInfoClass);
    }
}

void ecosystinterfaceClass::DoDailyEco() {
    theEcosystem.CropSoilExchange();
    theEcosystem.GiveIndicators();
    theOutput -> DailyUpdate();
    UpdateStatus();
}

;
void ecosystinterfaceClass::AllocateCrop(string cropID,
        int                                     cropCode) {
    switch (cropCode) {
        case 1 :
            cropID = "W1";

            break;

        case 2 :
            cropID = "RW";

            break;

        case 3 :
            cropID = "B1";

            break;

        case 4 :
            cropID = "G1";

            break;

        case 5 :
            cropID = "B5";

            break;

        case 6 :
            cropID = "PE";

            break;

        case 7 :
            cropID = "W5";

            break;

        case 8 :
            cropID = "BE";

            break;

        case 9 :
            cropID = "C1";

            break;

        case 10 :
            cropID = "MA";

            break;

        case 11 :
            cropID = "G1";    // weeds

            break;

        case 12 :
            cropID = "O1";

            break;

        case 13 :
            cropID = "RY";

            break;

        case 14 :
            cropID = "RS";

            break;

        case 15 :
            cropID = "OR";

            break;

        case 16 :
            cropID = "PO";

            break;

        case 17 :
            cropID = "IR";

            break;

        case 18 :
            cropID = "SF";

            break;

        case 19 :
            cropID = "L1";

            break;

        case 20 :
            cropID = "SB";

            break;

        case 21 :
            cropID = "CH";

            break;

        case 22 :
            cropID = "DW";

            break;

        case 23 :
            cropID = "HV";

            break;

        case 24 :
            cropID = "S1";

            break;

        default :
            theMessage -> FatalError("ecosystinterfaceClass::AllocateCrop - crop not found");
    }
}

void ecosystinterfaceClass::AddOrganic(int theType,
        double                             theamount,
        double                             theDepth,
        double                             thepercentTotalN,
        double                             thepercentNH4N,
        double                             thepercentNO3N,
        double                             thepercentDM,
        double                             thepercentC) {

    // convert from g/m**2 to tonnes/ha (ecosystem will convert back to g/m**2)
    double NH4N    = thepercentNH4N / 100.0;
    double NO3N    = thepercentNO3N / 100.0;
    double totalN  = thepercentTotalN / 100.0;
    double orgN    = totalN - (NH4N + NO3N);
    double decompC = thepercentC / 100.0;

    theamount /= 100.0;

    int            type = int(theType);
    decomposable * aproduct;

    if (type > 100) {
        type     /= 100;
        aproduct = (decomposable *) theProducts -> GetProductPtr(plantItemObj, type);
    } else {
        aproduct = (decomposable *) theProducts -> GetProductPtr(manureObj, type);
    }

    aproduct -> Setamount(theamount);
    aproduct -> SetDryMatter(thepercentDM / 100.0);
    aproduct -> SetorgN_content(orgN / theamount);
    aproduct -> SetNH4_content(NH4N / theamount);
    aproduct -> SetNO3_content(NO3N / theamount);
    aproduct -> SetC_content(decompC / theamount);
    theEcosystem.AddDecomposable(aproduct);
    theEcosystem.Incorporate(theDepth, 1.0, false, "adding organic matter");
    theOutput -> AddIndicator(environmentalIndicator, "31.06 N from spread manure", "kg N",
                              totalN * theEcosystem.GetArea() * 1000.0);
    theOutput -> AddIndicator(environmentalIndicator, "35.02 N from imported manure", "kg N",
                              totalN * theEcosystem.GetArea() * 1000.0);
    theOutput -> AddIndicator(environmentalIndicator, "40.02 C from field-applied manure", "kg C",
                              decompC * theEcosystem.GetArea() * 1000.0);
}

void ecosystinterfaceClass::SetFertilisation(double theNH4N,
        double                                      theNO3N) {
    nitrogen surfNitrate  = theNO3N;
    nitrogen surfAmmonium = theNH4N;
    nitrogen soilNitrate  = 0.0;
    nitrogen soilAmmonium = 0.0;

    theEcosystem.AddNutrient(surfNitrate, surfAmmonium, soilNitrate, soilAmmonium);

    nitrogen Nfert = surfNitrate + surfAmmonium + soilNitrate + soilAmmonium;

    theOutput -> AddIndicator(environmentalIndicator, "31.01 N from mineral fertilizer", "kg N",
                              Nfert.n * theEcosystem.GetArea() * 10.0);
}

void ecosystinterfaceClass::Grazing(double DMGrazed) {
    theEcosystem.SetGrazed(0.0, DMGrazed);
}

void ecosystinterfaceClass::OutputPlantDetails(bool initialise) {
    if (initialise) {
        char filename[40];
        char leader[100];

        strcpy(leader, globalSettings -> getOutputDirectory().c_str());
        sprintf(filename, "ecosyst");

        convert conv;

        strcat(filename, conv.IntToString(Index - 1).c_str());
        strcat(filename, ".xls");
        strcat(leader, filename);
        plantDetailFile.open(leader, ios::out);
        plantDetailFile.precision(4);

        plantDetailFile << "date";
        plantDetailFile << "\tcrop number\tpClover";

        for (int i = 0; i < MaxPlants; i++) {
            plantDetailFile << "\tCropName\tphase\tTempSum\tGLAI\tYLAI\tDMRoot\tDMRootPL\tDMRootTubers\tDMtop\tDMStorage\tDMTotalTop";
            plantDetailFile << "\tTranspirationRatio\tHeight\tRootDepth\tTotalRootLength\tInterceptedRadiation";
            plantDetailFile << "\tNitrogen\tN15\tNmin()\tNmax()\tNStorage\tNTop\tNVegTop\tNRoot\tNRootPL\tNRootTubers\tfNitrogen\tfNitrogenCurve";
            plantDetailFile << "\tRootDeposit\tRootTrans\tNRootDeposit\tTopDeposit\tNTopDeposit";
            plantDetailFile << "\tTopProd\tNUptake\tNfix\tNfixAcc\tdeltaDM\tN15Root\tAccRootResp";

            for (unsigned int i = 0; i < globalSettings -> RootData.size(); i++) {
                plantDetailFile << "\tRootLength-" << globalSettings -> RootData[i].getStartDepth() << "-"
                                << globalSettings -> RootData[i].getEndDepth();
                plantDetailFile << "\tRootMass-" << globalSettings -> RootData[i].getStartDepth() << "-"
                                << globalSettings -> RootData[i].getEndDepth();
                plantDetailFile << "\tRootN-" << globalSettings -> RootData[i].getStartDepth() << "-"
                                << globalSettings -> RootData[i].getEndDepth();
            }
        }
    } else {
        plantDetailFile << theTime.GetYear() << "-" << theTime.GetMonth() << "-" << theTime.GetDay();

        theEcosystem.outputCropDetails(&plantDetailFile);
    }

    plantDetailFile << endl;
}

void ecosystinterfaceClass::ClosePlantDetails() {
    if (plantDetailFile) {
        plantDetailFile.close();
    }
}

void ecosystinterfaceClass::SetECOGlobals() {
    theOutput = ecoOutput;
}
#endif


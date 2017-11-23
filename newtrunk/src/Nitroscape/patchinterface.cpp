
#ifdef NITROSCAPE

#include "../Nitroscape/patchinterface.h"
#include "../products/products.h"
#include "../fields/patch.h"

patchInterfaceClass::patchInterfaceClass(string aName,
        const int                               aIndex,
        const base *                            aOwner):
    base(aName,
         aIndex,
         aOwner)
{
    theFractionIncorporated = 1.0;

    zeroOutputVariables();
}

void patchInterfaceClass::HarvestOperations(harvestFields * hrv) {
    harvestCode = hrv -> GetOperationId();

    if (harvestCode != 20)    // is not gathering bales
    {

        // the following two lines are a fudge. the values should be set in the input files
        if (hrv -> GetFractionGrainHarvested() == 0.0) {
            SetpropGrainHarvested(1.0);
        } else {
            SetpropGrainHarvested(hrv -> GetFractionGrainHarvested());
        }

        if (hrv -> GetFractionStrawHarvested() == 0) {
            SetpropStrawHarvested(1.0);
        } else {
            SetpropStrawHarvested(hrv -> GetFractionStrawHarvested());
        }

        for (int i = 0; i < MaxPlants; i++) {
            if (MainProduct[i]) {
                MainProduct[i] -> Setname(hrv -> GetMainProduct(i) -> GetName());
                hrv -> SetMainProduct(MainProduct[i], i);
            }

            if (StrawProduct[i]) {
                StrawProduct[i] -> Setname(hrv -> GetStrawProduct(i) -> GetName());
                hrv -> SetStrawProduct(StrawProduct[i], i);
            }

            // hrv->SetForageProduct(ForageProduct[i],i);
        }
    }
}

;
void patchInterfaceClass::zeroOutputVariables() {
    theType                = -1.0;
    theDepth               = 0.0;
    harvestCode            = -1;
    amountSeedDM           = 0.0;
    totalLAI               = 0.0;
    ReadyForHarvestOrLater = false;
    IrrigationDemand       = 0.0;
    ;
    irrigationWater        = 0.0;
    irrigationN            = 0.0;
    MaxCropHeight          = 0.0;
    dryMatterGrazed        = 0.0;

    for (int i = 0; i < MaxPlants; i++) {
        MainProduct[i]  = NULL;    // leak?
        StrawProduct[i] = NULL;
    }

    thecropID               = 0;
    seedN                   = 0.0;
    theNitrate              = 0.0;
    theAmmonium             = 0.0;
    theFractionIncorporated = 0.0;
    harvestCode             = 0;
    propGrainHarvested      = 0.0;
    propStrawHarvested      = 0.0;
    decompType              = 0.0;
    decompAmount            = 0.0;
    decompNH4_N             = 0.0;
    decompNO3_N             = 0.0;
    decompOrg_N             = 0.0;
    decompC                 = 0.0;
    decompPercentDM         = 0.0;
    killAllCrops            = false;
    NH3Emission             = 0.0;
}

;
void patchInterfaceClass::AddNutrient(double surfNitrate,
        double                               surfAmmonium,
        double                               soilNitrate,
        double                               soilAmmonium) {
    theNitrate  = surfNitrate + soilNitrate;
    theAmmonium = surfAmmonium + soilAmmonium;

    theOutput -> AddIndicator(environmentalIndicator, "35.22 N in exported manure", "kg N",
                              (theNitrate + theAmmonium) * area * 10.0);

    // N amounts already in g/m**2
}

void patchInterfaceClass::Sow(string cropID,
                              string cropName,
                              double SeedDensDryMatt,
                              double RelativeDensity,
                              double NitrogenInSeed,
                              double CarbonInSeed) {
    AllocateCrop(cropID, cropName);

    string    aName    = cropName;
    product * aproduct = theProducts -> GetProductElement(aName);

    thecropID    = aproduct -> GetIndex();
    amountSeedDM = SeedDensDryMatt;
    seedN        = NitrogenInSeed;
    seedC        = CarbonInSeed;

    // ??? RelativeDensity = RelativeDensity;
}

;
void patchInterfaceClass::Tillage(soilTreatFields * TillageOp) {
    theType                 = TillageOp -> GetOperationId();
    theDepth                = TillageOp -> GetDepth();
    theFractionIncorporated = TillageOp -> GetFractionIncorporated();
}

void patchInterfaceClass::SetGrazed(double grazing_height,
        double                             DMGrazed,
        double                             NGrazed) {
    dryMatterGrazed += DMGrazed;

    // ??? grazing_height= grazing_height;  //to quieten compiler
    theOutput -> AddIndicator(environmentalIndicator, "35.01 N from imported feed", "kg N", NGrazed);
}

;
void patchInterfaceClass::GetFeedResource(feedItem * aresource) {

    // delete aresource;
    string newname = "FEED";

    newname += StrawProduct[0] -> GetName();

    feedItem * tempResource = (feedItem *) theProducts -> GetProductElement(newname);

    // aresource = new feedItem(*(feedItem *) theProducts->GetProductElement(newname));
    *aresource + *tempResource;
    aresource -> SetOwner(this);
    aresource -> SetmainUnitName(2);

    for (int i = 0; i < 4; i++) {
        if (MainProduct[i]) {
            *aresource + *MainProduct[i];
        }

        if (StrawProduct[i]) {
            *aresource + *StrawProduct[i];
        }
    }
}

void patchInterfaceClass::AddDecomposable(decomposable * produc) {
    switch (produc -> GetObjType()) {
        case manureObj :
            decompType = (double) produc -> GetIndex();

            break;

        case plantItemObj :
            decompType = (double) produc -> GetIndex() * 100.0;

            break;

        default :
            cout << "error";
    }

    theOutput -> AddIndicator(environmentalIndicator, "35.22 N in exported manure", "kg N",
                              produc -> GetAmount() * produc -> GetAllN().n * area * 1000.0);

    // convert to g/m**2
    decompAmount    += produc -> GetAmount() * 100.0;
    decompNH4_N     += decompAmount * produc -> GetNH4_content().n;
    decompNO3_N     += decompAmount * produc -> GetNO3_content().n;
    decompOrg_N     += decompAmount * produc -> GetorgN_content().n;
    decompC         += decompAmount * produc -> GetC_content();
    decompPercentDM += produc -> GetdryMatter() * 100.0;
}

;
plantItem * patchInterfaceClass::createHarvestProduct(string aName,
        double                                               amountDM,
        double                                               amountN,
        double                                               amountC) {
    plantItem * aPlantItem = new plantItem(aName, 0, NULL);

    if (amountDM) {
        aPlantItem -> SetorgN_content(amountN / amountDM);
        aPlantItem -> SetC_content(amountC / amountDM);
    }

    aPlantItem -> SetDryMatter(1.0);
    aPlantItem -> Setamount(amountDM * ((patch *) GetOwner()) -> GetArea()
                            / 1000.0);    // multiply by patch area and divide by 1000 to convert from kg to tonnes

    return aPlantItem;
}

;
double patchInterfaceClass::GetAvailableStandingDM(double height) {
    double retVal = 0.0;

    for (int i = 0; i < 4; i++) {
        if (MainProduct[i]) {
            retVal += MainProduct[i] -> GetAmount();
        }

        if (StrawProduct[i]) {
            retVal += StrawProduct[i] -> GetAmount();
        }
    }

    return retVal;
}

;
double patchInterfaceClass::GetStandingDMdensity() {
    double retVal = GetAvailableStandingDM(0.0) * 1000.0 / area;    // mult by 1000 to go from t to kg/ha

    return retVal / 10.0;    // divide by 10 to go from kg/ha to g/metre sq
}

;
void patchInterfaceClass::SetEcoStatus(int theReadyForHarvestOrLater,
        double                             theIrrigationDemand,
        linkList<harvInfoClass> *          theHarvestProducts) {
    ReadyForHarvestOrLater = theReadyForHarvestOrLater;
    IrrigationDemand       = theIrrigationDemand;

    int mainCount       = 0;
    int strawCount      = 0;
    int numHarvProducts = theHarvestProducts -> NumOfNodes();

    for (int i = 0; i < 4; i++) {
        if (i < numHarvProducts) {
            harvInfoClass * aharvInfoClass = theHarvestProducts -> ElementAtNumber(i);
            plantItem *     aproduct       = createHarvestProduct(aharvInfoClass -> GetproductName(),
                                                 aharvInfoClass -> GetamountDM(), aharvInfoClass -> GetamountN(),
                                                 aharvInfoClass -> GetamountC());

            if (aharvInfoClass -> GetproductCode() == 0) {
                MainProduct[mainCount] = aproduct;

                mainCount++;
            } else {
                StrawProduct[strawCount] = aproduct;

                strawCount++;
            }
        } else {
            MainProduct[i]  = NULL;
            StrawProduct[i] = NULL;
        }
    }
}

double patchInterfaceClass::GetAbovegroundCropN() {

    // double retVal=0.0;
    double N = 0.0;

    for (int i = 0; i < 4; i++) {
        if (MainProduct[i]) {
            N += MainProduct[i] -> GetAmount() * MainProduct[i] -> GetAllN().n;
        }

        if (StrawProduct[i]) {
            N += StrawProduct[i] -> GetAmount() * StrawProduct[i] -> GetAllN().n;
        }
    }

    return N;
}

void patchInterfaceClass::AllocateCrop(string crop_id,
        string                                cropName) {
    if ((crop_id == "W1") || (crop_id == "W2")) {
        cropName = "SEEDWNWHT";
    } else if (crop_id == "RW") {
        cropName = "SEEDWNRPE";
    } else if (crop_id == "RS") {
        cropName = "SEEDSPRPE";
    } else if ((crop_id == "B1") || (crop_id == "B2")) {
        cropName = "SEEDSPBARL";
    } else if ((crop_id == "F1") || (crop_id == "G1") || (crop_id == "GR") || (crop_id == "G4")) {

        // || crop_id =="C1")==0)   NJH removed this Nov 2007
        cropName = "SEEDRYEGRASS";
    } else if ((crop_id == "B5") || (crop_id == "B6")) {
        cropName = "SEEDWNBRL";
    } else if (crop_id == "PE") {
        cropName = "SEEDPEA";
    } else if (crop_id == "W5") {
        cropName = "SEEDSPWHT";
    } else if (crop_id == "BE") {
        cropName = "SEEDBEET";
    } else if (crop_id == "PO") {
        cropName = "SEEDPOTATO";
    } else if (crop_id == "MA") {
        cropName = "SEEDMAIZE";
    } else if ((crop_id == "K1") || (crop_id == "C1")) {
        cropName = "SEEDCLOVER";
    } else if (crop_id == "RY") {
        cropName = "SEEDRYE";
    } else if ((crop_id == "O1") || (crop_id == "O2")) {
        cropName = "SEEDOAT";
    } else if (crop_id == "OR") {
        cropName = "SEEDOILRADISH";
    } else if (crop_id == "S1") {
        cropName = "SEEDRYEGRASS";
    } else if (crop_id == "IR") {
        cropName = "SEEDITALRYEGRASS";
    } else if (crop_id == "SB") {
        cropName = "SEEDSOYBEAN";
    } else if (crop_id == "SF") {
        cropName = "SEEDSUNFLOWER";
    } else if (crop_id == "L1") {
        cropName = "SEEDLUCERNE";
    }
}
#endif


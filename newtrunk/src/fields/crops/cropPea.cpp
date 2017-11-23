
#include "../../base/common.h"
#include "cropPea.h"
#include "../../base/bstime.h"
#include "../../products/products.h"
#include "../../base/climate.h"
#include "../../base/commonData.h"

cropPea::cropPea(string       aName,
                 const int    aIndex,
                 const base * aOwner,
                 string       cropName):
    crop(aName,
         aIndex,
         aOwner,
         cropName)
{

    // Basal parameters
    PlantItemName = "PEA";         // These names maps to products.dat !
    StrawItemName = "PEASTRAW";    // These names maps to products.dat !
    WinterSeed    = false;

    commonData * data = globalSettings -> CropInformation;

    NFixationThisDay = 0.0;
    PotNFix          = 0.0;

    data -> FindItem("MaxFixPerDM", &MaxFixPerDM);
    data -> FindItem("DMCostPerN", &DMCostPerN);
}

cropPea::cropPea(const cropPea & acrop):
    crop(acrop)
{
    MaxFixPerDM          = acrop.MaxFixPerDM;
    PotNFix              = acrop.PotNFix;
    NFixationThisDay     = acrop.NFixationThisDay;
    AccumulatedNFixation = acrop.AccumulatedNFixation;
    DMCostPerN           = acrop.DMCostPerN;
}

double cropPea::CalcPotentialNFixation(double dDryMatt) {
    if (Phenology -> GrainFillStart()) {
        return 0.0;
    }

    return dDryMatt * MaxFixPerDM;    // NOTE NEW TEST CONCEPT!!!!!! Not validated!!!!!
}

void cropPea::NitrogenAndTranspiration(nitrogen aNitrogenUptake,
        double                                  aTranspirationRatio) {
    nitrogen Nfix;

    TranspirationRatio = aTranspirationRatio;

    if ((TranspirationRatio < 0.5)
            && (YellowCropAreaIndex + GreenCropAreaIndex
                < 0.01)) {    // corrects problem with transpiration the day of germination
        TranspirationRatio = 1.0;
    }

    if (aNitrogenUptake.n > 1e-15) {
        Nitrogen               = Nitrogen + aNitrogenUptake;
        AccumulatedNProduction = AccumulatedNProduction + aNitrogenUptake;

        Nbudget.AddInput((aNitrogenUptake).n);    // Budgets
        N15budget.AddInput((aNitrogenUptake).n15);
    }

    if (aNitrogenUptake.n < 0.0) {
        cout << GetLongName() << endl;

        theMessage -> WarningWithDisplay("cropPea::NitrogenAndTranspiration - nitrogen uptake is negative");
    }

    double dDM = DeltaDryMatter();

    PotNFix          = CalcPotentialNFixation(dDM);
    NFixationThisDay = min(NitrogenDemand(), PotNFix);

    if (NFixationThisDay > 0.0) {
        Nfix.SetBoth(NFixationThisDay, 0.0);    // IMPROVE THIS TO ACCOUNT FOR REAL DISTRIBUTION OF N15!!!!

        Nitrogen = Nitrogen + Nfix;

        double DMCost = NFixationThisDay * DMCostPerN;

        DryMatterRoot   -= DMCost * FractionToRoot();
        DryMatterVegTop -= DMCost * (1 - FractionToRoot());

        DMbudget.AddOutput(DMCost);

        AccumulatedNProduction = AccumulatedNProduction + Nfix;

        Nbudget.AddInput((Nfix).n);             // Budgets
        N15budget.AddInput((Nfix).n15);

        AccumulatedNFixation += NFixationThisDay;
    } else {
        NFixationThisDay = 0.0;
    }

    PotNFix = 0.0;
}

/*
 * NEW Concept based on root translocation
 */
void cropPea::TransferDryMatterToRoot(double * dDryMatt) {
    double DeltaRoot = DryMatterRoot;

    crop::TransferDryMatterToRoot(&*dDryMatt);

    DeltaRoot = max(0.0, DryMatterRoot - DeltaRoot);    // Increase in root dry matter

    // PotNFix   = CalcPotentialNFixation(DeltaRoot);
}


#include "../../base/common.h"
#include "cropChickory.h"
#include "../../products/products.h"

cropChickory::cropChickory(string       aName,
                           const int    aIndex,
                           const base * aOwner,
                           string       cropName):
    cropSbarley(aName,
                aIndex,
                aOwner,
                cropName)
{

    // For complete nomenclature see base class 'crop'
    PlantItemName  = "CHICKORY";    // no primarly harvest item
    StrawItemName  = "CHICKORY";    // These names maps to products.dat !
    WinterSeed     = true;          // Is the present crop a wintercrop.
    GrowthContinue = false;
}

double cropChickory::Update(double ActivePar) {
    return cropSbarley::Update(ActivePar);
}

/*
 * Returns cut material in g/m2
 */
void cropChickory::Cut(plantItem * cutPlantMaterial,
                       double      CutHeight) {
    CutOrHarvested = true;

    nitrogen nVegTop  = NitrogenInVegTop();
    nitrogen nStorage = NitrogenInStorage();

    if (DryMatterVegTop > 0.0) {
        double fraction = 0.0;

        if (PlantHeight > 0.0) {
            fraction = 1.0 - CutHeight / PlantHeight;
        }

        if (fraction <= 0.0) {
            fraction = 0.0;

            theMessage -> WarningWithDisplay("cropChickory::Cut - crop to low to cut");
        } else {
            double CutDM = DryMatterStorage + fraction * DryMatterVegTop;

            cutPlantMaterial -> Setname(PlantItemName);
            theProducts -> GiveProductInformation(cutPlantMaterial);
            cutPlantMaterial -> Setamount(CutDM / cutPlantMaterial -> GetdryMatter());

            // Nitrogen
            nitrogen CutN = nVegTop * fraction + nStorage;

            if (cutPlantMaterial -> GetAmount() > 0) {
                cutPlantMaterial -> SetorgN_content(CutN / cutPlantMaterial -> GetAmount());
            }

            Nitrogen = Nitrogen - CutN;

          
            double cutFromStorage = min(CutDM, DryMatterStorage);

            DryMatterStorage -= cutFromStorage;
            DryMatterVegTop  -= max(0.0, (CutDM - cutFromStorage));

            if ((DryMatterVegTop < 0) || (CutN.n < 0) || (Nitrogen.n < 0) || (CutDM < 0)) {
                theMessage -> FatalError("CropChickory:: Values at harvest should not be negative");
            }

            Nbudget.AddOutput(CutN.n);        // Budgets
            N15budget.AddOutput(CutN.n15);    // Budgets
            DMbudget.AddOutput(CutDM);

            GreenCropAreaIndex  = (1.0 - fraction) * GreenCropAreaIndex;
            YellowCropAreaIndex = (1.0 - fraction) * YellowCropAreaIndex;
            PlantHeight         = CutHeight;
        }
    }

    double NRemain, DMRemain;

    EndBudget(&NRemain, &DMRemain);
}

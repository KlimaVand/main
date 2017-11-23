/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropDyerswoad.h"
#include "../../products/products.h"

/****************************************************************************\
\****************************************************************************/
cropDyersWoad::cropDyersWoad(const char * aName, const int aIndex, const base * aOwner, string cropName)
              :cropSbarley(aName,aIndex,aOwner, cropName)
{
   // For complete nomenclature see base class 'crop'

   PlantItemName  = "DYERSWOAD";                 // no primarly harvest item
   StrawItemName  = "DYERSWOAD";           // These names maps to products.dat !
	WinterSeed     = true;                     // Is the present crop a wintercrop.
   GrowthContinue = false;

   // Root growth
           // Maximum temperature for dry matter production

}
/****************************************************************************\
\****************************************************************************/
double cropDyersWoad::Update(double ActivePar)
{
  	double DailyDMProd = cropSbarley::Update(ActivePar);
   return DailyDMProd;
}

/****************************************************************************\
Returns cut material in g/m2
\****************************************************************************/
void cropDyersWoad::Cut(plantItem * cutPlantMaterial, double CutHeight)
{
   CutOrHarvested = true;
   if (DryMatterVegTop>0.0)
   {
      double fraction=0.0;
      if (PlantHeight>0.0)
         fraction=1.0-CutHeight/PlantHeight;
   	if (fraction<=0.0)
      {
         fraction=0.0;
         theMessage[id]->WarningWithDisplay("cropDyersWoad::Cut - crop to low to cut");
      }
   	else
   	{
      double CutDM = DryMatterStorage + fraction*DryMatterVegTop;

      cutPlantMaterial->Setname(PlantItemName);
      theProducts[id]->GiveProductInformation(cutPlantMaterial);
      cutPlantMaterial->Setamount(CutDM/cutPlantMaterial->GetdryMatter());

      // Nitrogen
      nitrogen CutN = NitrogenInVegTop()*fraction+NitrogenInStorage();

		if (cutPlantMaterial->GetAmount()>0)
         cutPlantMaterial->SetorgN_content(CutN/cutPlantMaterial->GetAmount());

      Nitrogen = Nitrogen - CutN;
      if(Nitrogen.n<=0)
      	 theMessage[id]->FatalError("problem");
      double cutFromStorage = min(CutDM,DryMatterStorage);
      DryMatterStorage -= cutFromStorage;
      DryMatterVegTop    -= max(0.0,(CutDM-cutFromStorage));

      if (DryMatterVegTop<0 || CutN.n<0 || Nitrogen.n<0 || CutDM<0)
      	theMessage[id]->FatalError("CropDyersWoad:: Values at harvest should not be negative");

      Nbudget.AddOutput(CutN.n);                    // Budgets
      N15budget.AddOutput(CutN.n15);                    // Budgets
      DMbudget.AddOutput(CutDM);

      GreenCropAreaIndex = (1.0-fraction)*GreenCropAreaIndex;
      YellowCropAreaIndex= (1.0-fraction)*YellowCropAreaIndex;
      PlantHeight        = CutHeight;
  	}
   }
   double NRemain, DMRemain;
   EndBudget(&NRemain,&DMRemain);
}



/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "cropchickory.h"
#include "products.h"

/****************************************************************************\
\****************************************************************************/
cropChickory::cropChickory(const char * aName, const int aIndex, const base * aOwner)
              :cropRyegrass(aName,aIndex,aOwner)
{
   // For complete nomenclature see base class 'crop'
   CropName       = "Chickory";
   PlantItemName  = "CHICKORY";                 // no primarly harvest item
   StrawItemName  = "CHICKORY";           // These names maps to products.dat !
/*	WinterSeed     = true;                     // Is the present crop a wintercrop.
   GrowthContinue = false;

   // Root growth
   Phenology->TS0          = 20;              // Sowing to emergence
   //Phenology->LinearLeafPhase = 250;
	//InitialCAI = 2;

	MaxRadUseEff 		 = 3.3;        // guestimate
	MaxRootDepthCrop   = 3.0;        // new estimate from KTK (18_3_3)
	RootPentrRate      = 5.0*0.001;  // guestimate
	RootDistrParm      = 1.0;        // Root density distribution parameter

   MinN_Root     = 0.01;           // Guesstimate Minimum content of nitrogen i root organs
	MaxN_Root     = 0.01;           // Guestimate Maximum content of nitrogen i root organs
*/
}
/****************************************************************************\
\****************************************************************************/
double cropChickory::Update(double ActivePar)
{
//	return cropRyegrass::Update(ActivePar);
   return 0.0;
}

/*void cropChickory::Cut(plantItem * cutPlantMaterial, double CutHeight)
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
         theMessage->Warning("crop::Cut - crop to low to cut");
      }
      double CutDMVeg = fraction*DryMatterVegTop;
      double CutDMStorage = fraction * DryMatterStorage;
      double CutDM = CutDMVeg + CutDMStorage;

      cutPlantMaterial->Setname(PlantItemName);
      theProducts->GiveProductInformation(cutPlantMaterial);
      cutPlantMaterial->Setamount(CutDM/cutPlantMaterial->GetdryMatter());

      // Nitrogen
      nitrogen CutNVeg = NitrogenInVegTop()*fraction;
      nitrogen CutNStorage = NitrogenInStorage()*fraction;
      nitrogen CutN = CutNVeg + CutNStorage;

		if (cutPlantMaterial->GetAmount()>0)
         cutPlantMaterial->SetorgN_content(CutN/cutPlantMaterial->GetAmount());

      Nitrogen = Nitrogen - CutN;
      DryMatterVegTop    -= CutDMVeg;
      DryMatterStorage    -= CutDMStorage;

      if (DryMatterVegTop<0 || DryMatterStorage<0 ||CutN.n<0 || Nitrogen.n<0 || CutDM<0)
      	theMessage->FatalError("Crop:: Values at harvest should not be negative");

      Nbudget.AddOutput(CutN.n);                    // Budgets
      N15budget.AddOutput(CutN.n15);                    // Budgets
      DMbudget.AddOutput(CutDM);

      GreenCropAreaIndex = (1.0-fraction)*GreenCropAreaIndex;
      YellowCropAreaIndex= (1.0-fraction)*YellowCropAreaIndex;
      PlantHeight        = CutHeight;
      topMatter          = NULL;
//      rootMatter         = NULL;
  	}
}                         */



#include "../../base/common.h"
#include "cropOilRadish.h"
#include "../../products/products.h"

/****************************************************************************\
\****************************************************************************/
cropOilRadish::cropOilRadish(const char * aName, const int aIndex, const base * aOwner)
              :cropBeet(aName,aIndex,aOwner)
{
   // For complete nomenclature see base class 'crop'
   CropName       			= "OilRadish";
   PlantItemName  			= "OILRADISH";                 // no primarly harvest item
   StrawItemName  			= "OILRADISH";           // These names maps to products.dat !
	WinterSeed     			= true;                     // Is the present crop a wintercrop.
   GrowthContinue 			= false;

   // Root growth
   MaxRootDepthCrop   		= 3.0;        	// new estimate from KTK (18_3_3)
	RootPentrRate      		= 0.0021;  		// Estimated from minirhizothon data in an organic rotation
	RootDistrParm      		= 1.0;        	// Root density distribution parameter
   SpecificRootLength		= 200;
	NitrateUptakeRate  		= 0.00006;     // Uptake rate for nitrate-N g/m/d (m root length)

   //Photosynthesis
	MaxRadUseEff 		 		= 4.0;        	// guestimate
	MinDMtemp         		= 2;           // Minimum temperature for dry matter production
	MaxDMtemp         		= 5;           // Maximum temperature for dry matter production

   //N concentrations
   MinN_Root     				= 0.01;        // Guesstimate Minimum content of nitrogen i root organs
	MaxN_Root     				= 0.03;        // Guestimate Maximum content of nitrogen i root organs
	NPctMax    					= 6.18;        // Following Justes concept adjusted (Colnelle et al 1998)
	NPctMin   					= 2.07;        // Following Justes concept adjusted (Colnelle et al 1998)

   //Allocation
	MinAllocToRoot				= 0.2;
   MaxAllocToRoot				= 0.4;
	MinAllocToStorage    	= 0.1; 	      // Minimal fraction of dry matter production that is allocated to the root - Value from DAISY
	MaxAllocToStorage 		= 0.20;        // Max. allocation to storage organs - DAISY: 0.6
   TempSumStorage1 			= 1500;        // Temperature sum for min allocation to storage - DAISY: 0
   TempSumStorage2 			= 2000;        // Temperature sum for max allocation to storage - from DAISY

   //Phenology
   Phenology->TS1    		= 500;         // Emergence to anthesis
   Phenology->TS2    		= 3650;        // Anthesis to end of grainfilling
   Phenology->TS3    		= 0;           // End of grainfilling to ripeness
   Phenology->DS_StartFill = 0.7;
}
/****************************************************************************\
\****************************************************************************/
double cropOilRadish::Update(double ActivePar)
{
  	double DailyDMProd = cropBeet::Update(ActivePar);
   return DailyDMProd;
}
/****************************************************************************\
Returns cut material in g/m2. Oilradish has belowground storage organs.
Therefore no cut of storage organs
\****************************************************************************/
void cropOilRadish::Cut(plantItem * cutPlantMaterial, double CutHeight)
{
   nitrogen NVegTop = NitrogenInVegTop();

   CutOrHarvested = true;
   if (DryMatterVegTop>0.0)
   {
      double fraction=0.0;
      if (PlantHeight>0.0)
         fraction=1.0-CutHeight/PlantHeight;
   	if (fraction<=0.0)
      {
         fraction=0.0;
         theMessage->WarningWithDisplay("cropOilRadish::Cut - crop to low to cut");
      }
   	else
   	{
      double CutDM =  fraction*DryMatterVegTop;

      cutPlantMaterial->Setname(PlantItemName);
      theProducts->GiveProductInformation(cutPlantMaterial);
      cutPlantMaterial->Setamount(CutDM/cutPlantMaterial->GetdryMatter());

      // Nitrogen
      nitrogen CutN = NVegTop*fraction;

		if (cutPlantMaterial->GetAmount()>0)
         cutPlantMaterial->SetorgN_content(CutN/cutPlantMaterial->GetAmount());

   Nitrogen = Nitrogen - CutN;
     DryMatterVegTop    -= max(0.0,(CutDM));

      if (DryMatterVegTop<0 || CutN.n<0 || Nitrogen.n<0 || CutDM<0)
      	theMessage->FatalError("CropOilRadish:: Values at harvest should not be negative");

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





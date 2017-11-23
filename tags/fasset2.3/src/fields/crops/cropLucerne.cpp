#include "../../base/common.h"
#include "cropLucerne.h"
#include "../../base/climate.h"
#include "../../data/commonData.h"
#include "../../data/cropParametersData.h"
/****************************************************************************\
This Lucerne module is a test version based on clover!!!!!!!!!!!!!! MEL 2006
Data from Avice et al. 1997. Plant and soil 188: 189-198 brugt til at estimere parametre
\****************************************************************************/
cropLucerne::cropLucerne(const char * aName, const int aIndex, const base * aOwner, string cropName)
         :cropRyegrass(aName,aIndex,aOwner, cropName)
{
   // For complete nomenclature see base class 'crop'

   PlantItemName  = "LUCERNE";                 // no primarly harvest item
   StrawItemName  = "LUCERNE";           // These names maps to products.dat !
	WinterSeed     = true;                     // Is the present crop a wintercrop.
   GrowthContinue = true;

   MaxFixPerDM=theCropData->getMaxFixPerDM();
         DMCostPerN=theCropData->getDMCostPerN();
         NFixationThisDay=theCropData->getNFixationThisDay();
         PotNFix=theCropData->getPowDM();
}

/****************************************************************************\
\****************************************************************************/
cropLucerne::cropLucerne(const cropLucerne& acrop)
    : cropRyegrass( acrop )
{
  MaxFixPerDM=acrop.MaxFixPerDM;
  DMCostPerN=acrop.DMCostPerN;
  PotNFix=acrop.PotNFix;
  NFixationThisDay=acrop.NFixationThisDay;
}




/****************************************************************************\
\****************************************************************************/
double cropLucerne::CalcPotentialNFixation(double dDryMatt)
{
   //if (Phenology->TempSumForLeaf<100)                             // estimated that fix first starts after 20 days with 10 oC (Crush: Nitrogen fixation)
   //	return 0.0;
   double fTemp = 1.0;                                            // temperature response from Wu
   if (temp>26)
   	fTemp = max(0.0,min(1.0,(30.0-temp)/4.0));
   else
   	fTemp = max(0.0,min(1.0,(temp-9.0)/4.0));

   return dDryMatt*MaxFixPerDM*fTemp;
}

/****************************************************************************\
\****************************************************************************/
void cropLucerne::NitrogenAndTranspiration(nitrogen aNitrogenUptake,double aTranspirationRatio)
{
	nitrogen Nfix;
   TranspirationRatio=aTranspirationRatio;
	if (aNitrogenUptake.n>0.0)
		Nitrogen = Nitrogen + aNitrogenUptake;
  	NFixationThisDay = min(NitrogenDemand(),PotNFix);    // TEST !!!!!!!
	if (NFixationThisDay>0.0 && Phenology->Emerged())
   {
      Nfix.SetBoth(NFixationThisDay,0.0);                   // IMPROVE THIS TO ACCOUNT FOR REAL DISTRIBUTION OF N15!11!!!
		Nitrogen = Nitrogen + Nfix;

      // "Pay" DM for N fixation
      double DMCost = NFixationThisDay*DMCostPerN;
#ifdef BIOMODTEST
	AgeClassTop[0] -= DMCost;
#else
      DryMatterRoot   -= DMCost*FractionToRoot();
      AgeClassTop[0]  -= DMCost*(1-FractionToRoot());
#endif
      DMbudget.AddOutput(DMCost);

   	AccumulatedDMProduction -= DMCost;                    // N fixation costs are not included!
      RootTranslocation -=  DMCost*FractionToRoot();
   }
   else
      NFixationThisDay=0.0;

   AccumulatedNFixation += NFixationThisDay;
   AccumulatedNProduction = AccumulatedNProduction + aNitrogenUptake + Nfix;

   Nbudget.AddInput((Nfix+aNitrogenUptake).n);              // Budgets
   N15budget.AddInput((Nfix+aNitrogenUptake).n15);
   PotNFix=0.0;
}

/****************************************************************************\
\****************************************************************************/
void cropLucerne::TransferDryMatterToRoot(double *dDryMatt)
{
   PotNFix=CalcPotentialNFixation(DeltaDryMatter());
   crop::TransferDryMatterToRoot(&*dDryMatt);
}

/****************************************************************************\
\****************************************************************************/



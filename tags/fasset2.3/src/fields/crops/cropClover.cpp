/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropClover.h"
#include "../../base/climate.h"
#include "../../data/cropParametersData.h"

/****************************************************************************\
\****************************************************************************/
cropClover::cropClover(const char * aName, const int aIndex, const base * aOwner, string cropname)
         :cropRyegrass(aName,aIndex,aOwner, cropname)
{
   // For complete nomenclature see base class 'crop'

   PlantItemName  = "CLOVER";                 // no primarly harvest item
   StrawItemName  = "CLOVER";           // These names maps to products.dat !
	WinterSeed     = true;                     // Is the present crop a wintercrop.
   GrowthContinue = true;
	HasBeenCut = false;

	MaxFixPerDM=theCropData->getMaxFixPerDM();
	DMCostPerN=theCropData->getDMCostPerN();
	//State variables

   NFixationThisDay= 0.0;
}

/****************************************************************************\
\****************************************************************************/
cropClover::cropClover(const cropClover& acrop)
    : cropRyegrass(acrop)
{
  MaxFixPerDM=acrop.MaxFixPerDM;
  DMCostPerN=acrop.DMCostPerN;

  NFixationThisDay=acrop.NFixationThisDay;
}

/****************************************************************************\
\****************************************************************************/
void cropClover::Add(crop* aCrop, double fraction)
{
   cropRyegrass::Add(aCrop,fraction);
   cropClover * c = (cropClover*)aCrop;

  	MaxFixPerDM=(1.0-fraction)*MaxFixPerDM + fraction*c->MaxFixPerDM;
  	DMCostPerN=(1.0-fraction)*DMCostPerN + fraction*c->DMCostPerN;

   NFixationThisDay=(1.0-fraction)*NFixationThisDay + fraction*c->NFixationThisDay;
}



/****************************************************************************\
\****************************************************************************/
double cropClover::CalcPotentialNFixation(double dDryMatt)
{
   if (Phenology->TempSumForLeaf<200)                             // estimated that fix first starts after 20 days with 10 oC (Crush: Nitrogen fixation)
   	return 0.0;
   double fTemp = 1.0;                                            // temperature response from Wu
   if (temp>26)
   	fTemp = max(0.0,min(1.0,(30.0-temp)/4.0));
   else
   	fTemp = max(0.0,min(1.0,(temp-9.0)/4.0));

   return dDryMatt*MaxFixPerDM*fTemp;
}
/**
 * New Version for nitrogen fixation, MEL 2009
 */
void cropClover::NitrogenAndTranspiration(nitrogen aNitrogenUptake,double aTranspirationRatio)
{
	nitrogen Nfix;
   TranspirationRatio = aTranspirationRatio;
   if ((TranspirationRatio<0.5) && (YellowCropAreaIndex+GreenCropAreaIndex<0.01))         // corrects problem with transpiration the day of germination
   	TranspirationRatio = 1.0;
   if (aNitrogenUptake.n>1e-15)
	{
      Nitrogen = Nitrogen + aNitrogenUptake;
   	AccumulatedNProduction = AccumulatedNProduction + aNitrogenUptake;
   	Nbudget.AddInput((aNitrogenUptake).n);              // Budgets
   	N15budget.AddInput((aNitrogenUptake).n15);
   }
   if (aNitrogenUptake.n<0.0)
   {
   	//cout << GetLongName() << endl;
      theMessage->WarningWithDisplay("cropClover::NitrogenAndTranspiration - nitrogen uptake is negative");
   }
   double dDM =  DeltaDryMatter();
   double PotNFix;
	PotNFix   = CalcPotentialNFixation(dDM);

   NFixationThisDay = min(NitrogenDemand(),PotNFix);

	if (NFixationThisDay>0.0)
   {
		Nfix.SetBoth(NFixationThisDay,0.0);                   // IMPROVE THIS TO ACCOUNT FOR REAL DISTRIBUTION OF N15!!!!
		Nitrogen = Nitrogen + Nfix;

      double DMCost = NFixationThisDay*DMCostPerN;
      DryMatterRoot   -= DMCost*FractionToRoot();
      AgeClassTop[0] -= DMCost*(1-FractionToRoot());
      DMbudget.AddOutput(DMCost);
   	AccumulatedNProduction = AccumulatedNProduction + Nfix;
   	Nbudget.AddInput((Nfix).n);              // Budgets
   	N15budget.AddInput((Nfix).n15);
   	AccumulatedNFixation += NFixationThisDay;
   }
   else
      NFixationThisDay=0.0;

}
/****************************************************************************\
\****************************************************************************/
void cropClover::TransferDryMatterToRoot(double *dDryMatt)
{
//   PotNFix=CalcPotentialNFixation(DeltaDryMatter());
   crop::TransferDryMatterToRoot(&*dDryMatt);
}




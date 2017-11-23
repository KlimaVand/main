/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropPea.h"
#include "../../base/bstime.h"
#include "../../products/products.h"
#include "../../base/climate.h"
#include "../../base/commonData.h"
cropPea::cropPea(const char * aName, const int aIndex, const base * aOwner, string cropName)
        :crop(aName,aIndex,aOwner, cropName)
{
   // Basal parameters

   PlantItemName = "PEA";            // These names maps to products.dat !
   StrawItemName = "PEASTRAW";       // These names maps to products.dat !
   WinterSeed = false;
   commonData * data=globalSettings->CropInformation;
   data->FindItem("MaxFixPerDM",&MaxFixPerDM);
   data->FindItem("DMCostPerN",&DMCostPerN);
}

/****************************************************************************\
\****************************************************************************/
cropPea::cropPea(const cropPea& acrop)
   : crop(acrop)
{

   MaxFixPerDM = acrop.MaxFixPerDM;
   PotNFix = acrop.PotNFix;
   NFixationThisDay = acrop.NFixationThisDay;
   AccumulatedNFixation = acrop.AccumulatedNFixation;
   DMCostPerN = acrop.DMCostPerN;
}

/****************************************************************************\
Note that boolean state variables of course not are interpolated
\****************************************************************************/
void cropPea::Add(crop* aCrop, double fraction)
{
   crop::Add(aCrop,fraction);
   cropPea * c = (cropPea*)aCrop;


   MaxFixPerDM = (1-fraction)*MaxFixPerDM + fraction*c->MaxFixPerDM;
   PotNFix = (1-fraction)*PotNFix + fraction*c->PotNFix;
   NFixationThisDay = (1-fraction)*NFixationThisDay + fraction*c->NFixationThisDay;
   AccumulatedNFixation = (1-fraction)*AccumulatedNFixation + fraction*c->AccumulatedNFixation;
   DMCostPerN = (1-fraction)*DMCostPerN + fraction*c->DMCostPerN;
}

/****************************************************************************\
\****************************************************************************/
double cropPea::CalcPotentialNFixation(double dDryMatt)
{
   if (Phenology->GrainFillStart()) return 0.0;
   return dDryMatt*MaxFixPerDM;                               // NOTE NEW TEST CONCEPT!!!!!! Not validated!!!!!
}
/****************************************************************************\
 New Version for nitrogen fixation, MEL 2009
\****************************************************************************/
void cropPea::NitrogenAndTranspiration(nitrogen aNitrogenUptake,double aTranspirationRatio)
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
	cout << GetLongName() << endl;
      theMessage->WarningWithDisplay("cropPea::NitrogenAndTranspiration - nitrogen uptake is negative");
   }
   double dDM =  DeltaDryMatter();
	PotNFix   = CalcPotentialNFixation(dDM);

   NFixationThisDay = min(NitrogenDemand(),PotNFix);

	if (NFixationThisDay>0.0)
   {
		Nfix.SetBoth(NFixationThisDay,0.0);                   // IMPROVE THIS TO ACCOUNT FOR REAL DISTRIBUTION OF N15!!!!
		Nitrogen = Nitrogen + Nfix;

      double DMCost = NFixationThisDay*DMCostPerN;
      DryMatterRoot   -= DMCost*FractionToRoot();
      DryMatterVegTop -= DMCost*(1-FractionToRoot());
      DMbudget.AddOutput(DMCost);
   	AccumulatedNProduction = AccumulatedNProduction + Nfix;
   	Nbudget.AddInput((Nfix).n);              // Budgets
   	N15budget.AddInput((Nfix).n15);
   	AccumulatedNFixation += NFixationThisDay;
   }
   else
      NFixationThisDay=0.0;

   PotNFix=0.0;
}
/****************************************************************************\
Original Version 2009
\****************************************************************************/
/*void cropPea::NitrogenAndTranspiration(nitrogen aNitrogenUptake,double aTranspirationRatio)
{
	nitrogen Nfix;
   TranspirationRatio = aTranspirationRatio;
	PotNFix   = CalcPotentialNFixation(DeltaDryMatter());
   if (aNitrogenUptake.n>0.0)
		Nitrogen = Nitrogen + aNitrogenUptake;
   NFixationThisDay = min(NitrogenDemand(),PotNFix);

	if (NFixationThisDay>0.0)
   {
		Nfix.SetBoth(NFixationThisDay,0.0);                   // IMPROVE THIS TO ACCOUNT FOR REAL DISTRIBUTION OF N15!!!!
		Nitrogen = Nitrogen + Nfix;

      //double DMCost = NFixationThisDay*DMCostPerN;
      double FixationFraction = 0.0;
      if (NFixationThisDay>0.0)
      	FixationFraction = 0.2*NFixationThisDay/(NFixationThisDay+aNitrogenUptake.n);
      double DMCost = FixationFraction*DeltaDryMatter();
      //DryMatterRoot   -= DMCost*FractionToRoot();
      //DryMatterVegTop -= DMCost*(1-FractionToRoot());
      DryMatterVegTop -= DMCost;
      DMbudget.AddOutput(DMCost);
   }
   else
      NFixationThisDay=0.0;

   AccumulatedNProduction = AccumulatedNProduction + aNitrogenUptake + Nfix;
#ifdef TESTRESP
	//k = 0.5; // Note

	//k = 0.65;
   double GreenFraction = 0.0;
   if (GreenCropAreaIndex+YellowCropAreaIndex>0.0)
   	GreenFraction = GreenCropAreaIndex/(GreenCropAreaIndex+YellowCropAreaIndex/Conversfactor);
	double respir = DryMatterVegTop*0.015*GreenFraction*TemperatureEffect(temp);
   DryMatterVegTop -= respir;
   DMbudget.AddOutput(respir);
#endif

   Nbudget.AddInput((Nfix+aNitrogenUptake).n);              // Budgets
   N15budget.AddInput((Nfix+aNitrogenUptake).n15);
   AccumulatedNFixation += NFixationThisDay;
   PotNFix=0.0;
}
*/


/****************************************************************************\
NEW Concept based on root translocation
\****************************************************************************/
void cropPea::TransferDryMatterToRoot(double *dDryMatt)
{
   double DeltaRoot = DryMatterRoot;

   crop::TransferDryMatterToRoot(&*dDryMatt);

   DeltaRoot = max(0.0,DryMatterRoot-DeltaRoot);                // Increase in root dry matter
   //PotNFix   = CalcPotentialNFixation(DeltaRoot);
}



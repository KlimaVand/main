/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropClover.h"
#include "../../base/climate.h"

/****************************************************************************\
\****************************************************************************/
cropClover::cropClover(const char * aName, const int aIndex, const base * aOwner)
         :cropRyegrass(aName,aIndex,aOwner)
{
   // For complete nomenclature see base class 'crop'
   CropName       = "Clover";
   PlantItemName  = "CLOVER";                 // no primarly harvest item
   StrawItemName  = "CLOVER";           // These names maps to products.dat !
	WinterSeed     = true;                     // Is the present crop a wintercrop.
   GrowthContinue = true;
	HasBeenCut = false;
   //---- Parameters -----------------------------------------------------------

   // Root growth
	MaxRootDepthCrop 				= 1.5;             // new estimate from KTK (18_3_3)
	RootPentrRate    				= 0.0008;          // new estimate from KTK (18_3_3)
	SpecificRootLength   		= 250;         // Specific root length, m pr. kg. (from Evans: Root Morpho.. NZ J. Arg.Res 1977, 20, 331-335)

   // N parameters
   MinN_Root  						= 0.025;            // Whitehead: Grassland Nitrogen
   MaxN_Root  						= 0.040; 				 // Guesstimate

   MaxPlantHeight					= 0.5;            // Max plant height - taken from grass

   // Phenology
   Phenology->TS0          	= 125;              // Sowing to emergence, estimated from irrigated and non-irrigated grass on Foulum
   Phenology->TS1          	= 300;           // Emergence to anthesis, estimated from irrigated and non-irrigated grass on Foulum
   Phenology->TS2             = 420;                 // Temperature sum for end grain fill
   Phenology->TS3             = 155;                 // Temperature sum for ripeness
   Phenology->TB0             = 0;                   // Base temperature until emergence
   Phenology->TB1             = 4.0;					   // Base temperature until flowering
   Phenology->TB2             = 6.0;						// Base temperature after flowering
   Phenology->TB3             = 4.0;                 // Base temperature for ripening
   Phenology->DB              = 7.0;						// Daylength response   (Weir et al.)
   Phenology->DMAX            = 20.0;		            // Daylength response
   Phenology->DS_StartFill 	= 1.1;

   WaterDeficitVegGrowth 		= 0.65;

   //Allocation
   StoreForFilling		 		= 0.2;        // Fraction of DM present at initiation of grain filling that is translocated to grain
   FillFactor						= 0.3;                  // Fraction of net production after anthesis that goes into grain

   // N fixation
   DMCostPerN      				= 10.3;          // "Cost" in dry matter to produce nitrogen (g/g). From pea.
	MaxFixPerDM     				= 0.022;         // Maximal nitrogen fixation per produced dry matter (g/g).

   // Canopy structure
   LAIDistributionPar 			= 1.1;        // Estimated from Lantinga et al
	k            					= 0.7;              // Taken from H�gh-Jensen ph.d. thesis and Lantinga et al (Agr.For.Met, 96 1999 71-83)
	LAIDMRatio 						= 0.008;                //boundary specific leaf area index m2/g     //from Dennis & Woledge, 1982
	InitialLAIIncrease 			= 2;
	CoeffGreenIdx 					= 0.018;            // Coefficient for leaf area (from Nassiri & Elgersma, 1998)

   MaxRadUseEff  = 4.5; 		      // Maximum radiation use efficiency
	MinDMtemp    = 0.0;              // Minimum temperature for dry matter production - guesstimate
   MaxDMtemp    = 5.0;             // Maximum temperature for dry matter production - guesstimate

   liveOMD = 0.8;
	deadOMD = 0.5;

	FractionNminInRoots = 0.1;        //Fraction of N in roots present as mineral N
	FractionNminInVegTop = 0.05;
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
void cropClover::ReadParameters(char* filename)
{
   cropRyegrass::ReadParameters(filename);
   if (OpenInputFile(filename))
   {
	   if (FindSection(CropName)  || FindSection("Crop"))
      {
			GetParameter("MaxFixPerDM",&MaxFixPerDM);
			GetParameter("DMCostPerN",&DMCostPerN);
		}
	}
   CloseInputFile();
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




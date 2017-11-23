/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "cropclover.h"
#include "climclas.h"

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

   //---- Parameters -----------------------------------------------------------

   // Root growth
	MaxRootDepthCrop = 1.5;             // new estimate from KTK (18_3_3)
	RootPentrRate    = 0.0008;          // new estimate from KTK (18_3_3)
//	TempSumR2        = 500;             // See p. 160 in DAISY-description
	SpecificRootLength   = 250;         // Specific root length, m pr. kg. (from Evans: Root Morpho.. NZ J. Arg.Res 1977, 20, 331-335)

   // N parameters
   MinN_Root  = 0.025;            // Whitehead: Grassland Nitrogen
   MaxN_Root  = 0.040; 				 // Guesstimate
	//MaxN_Straw = 0.055;            // Maximum content of nitrogen i drymatter organs (from book: Baker & Williams: White Clover p159)

   MaxPlantHeight=0.3;            // Max plant height - taken from grass

   // Phenology
   WaterDeficitVegGrowth = 0.65;

   // N fixation
   DMCostPerN      = 0.7*5.9/0.4;          // "Cost" in dry matter to produce nitrogen (g/g). From pea.
   MaxFixPerDM     = 0.06;//26;     // Maximal nitrogen fixation per produced dry matter (g/g). From pea.
   PotNFix         = 0.0;
   NFixationThisDay= 0.0;

   // Canopy structure
   LAIDistributionPar = 1.1;        // Estimated from Lantinga et al
	k            = 0.7;              // Taken from Høgh-Jensen ph.d. thesis and Lantinga et al (Agr.For.Met, 96 1999 71-83)
	BulkDensity  = 1400;             // g DM/m3 standing crop - guesstimated

	MinDMtemp    = 2.0;              // Minimum temperature for dry matter production - guesstimate
   MaxDMtemp    = 10.0;             // Maximum temperature for dry matter production - guesstimate

   // Parameters adjusted/set
	CoeffGreenIdx = 0.018; // 0.0325;            // Coefficient for leaf area (from Nassiri & Elgersma, 1998)

   MaxRadUseEff  = 1.9; 		      // Maximum radiation use efficiency
	MaxFixPerDM     = 0.011;         // Maximal nitrogen fixation per produced dry matter (g/g). From pea.
   /* Old values from BMP version ---------------------------------------
   MaxRadUseEff     = 3.2;//3.0;                 // Maximum radiation use efficiency (g/MJ) - fittet on data Foulum 88-90 and 90-92
	PowDM            = -0.32; //0.0; //-0.32;               // Power of dry matter in nitrogen status calculation
   NPctMax          = 6.0; //7.5; //5.0; //4.25; //4.25; //7.4;    // 5.0             // Following Justes concept adjusted to max as: 8.3*4.8/5.35
   DMCostPerN       = 10.0; //4.0/0.42; // From Cannell & Thornley //0.5*5.9/0.4;          // "Cost" in dry matter to produce nitrogen (g/g). From pea.
	MinDMtemp        = 6.0;              // Minimum temperature for dry matter production - guesstimate
   CoeffGreenIdx    = 0.01; //0.018; //(0.02 is a test) 0.0325;            // Coefficient for leaf area (from Nassiri & Elgersma, 1998)
   RootPentrRate    = 0.5*0.0008;          // new estimate from KTK (18_3_3)
	TransferRate     = 0.02; // TEST REMOVE TEST !!!!!!!!!!!!!0.02;           // Fraction (temperature-dependend) of the four age pools that is transferred to subsequent fraction
	NitrateUptakeRate= 0.5*0.00006; //0.2*0.00006; // NEW value from DAISY (JB 2002-8) 0.000052;     // (0.31) Old value 0.000052. Uptake rate for nitrate-N g/m/d (m root length)
	MaxFixPerDM      = 2.0*0.011;     // Maximal nitrogen fixation per produced dry matter (g/g). From pea.
   --------------------------------------------------------------------*/
   liveOMD = 0.8;
	deadOMD = 0.5;
}

/****************************************************************************\
\****************************************************************************/
cropClover::cropClover(const cropClover& acrop)
    : cropRyegrass(acrop)
{
  MaxFixPerDM=acrop.MaxFixPerDM;
  DMCostPerN=acrop.DMCostPerN;
  PotNFix=acrop.PotNFix;                           
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
  	PotNFix=(1.0-fraction)*PotNFix + fraction*c->PotNFix;
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
 New Version for nitrogen fixation, MEL 2009
\****************************************************************************/
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
   	cout << GetLongName() << endl;
      theMessage->WarningWithDisplay("cropClover::NitrogenAndTranspiration - nitrogen uptake is negative");
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
\****************************************************************************/
/*void cropClover::NitrogenAndTranspiration(nitrogen aNitrogenUptake,double aTranspirationRatio)
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
      DryMatterRoot   -= DMCost*FractionToRoot();
      AgeClassTop[0]  -= DMCost*(1.0-FractionToRoot());
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
} */

/****************************************************************************\
\****************************************************************************/
void cropClover::TransferDryMatterToRoot(double *dDryMatt)
{
//   PotNFix=CalcPotentialNFixation(DeltaDryMatter());
   crop::TransferDryMatterToRoot(&*dDryMatt);
}



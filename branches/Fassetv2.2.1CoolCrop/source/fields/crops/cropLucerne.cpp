#include "../../base/common.h"
#include "cropLucerne.h"
#include "../../base/climate.h"
#include "../../data/commonData.h"
/****************************************************************************\
This Lucerne module is a test version based on clover!!!!!!!!!!!!!! MEL 2006
Data from Avice et al. 1997. Plant and soil 188: 189-198 brugt til at estimere parametre
\****************************************************************************/
cropLucerne::cropLucerne(const char * aName, const int aIndex, const base * aOwner)
         :cropRyegrass(aName,aIndex,aOwner)
{
   // For complete nomenclature see base class 'crop'
   CropName       = "Lucerne";
   PlantItemName  = "LUCERNE";                 // no primarly harvest item
   StrawItemName  = "LUCERNE";           // These names maps to products.dat !
	WinterSeed     = true;                     // Is the present crop a wintercrop.
   GrowthContinue = true;

   //---- Parameters -----------------------------------------------------------

   // Root growth
	MaxRootDepthCrop = 2.0;             // MEL 2006 //1.5;   // new estimate from KTK (18_3_3)
	RootPentrRate    =  0.0008;          // new estimate from KTK (18_3_3)
	SpecificRootLength   = 250;         // Specific root length, m pr. kg. (from Evans: Root Morpho.. NZ J. Arg.Res 1977, 20, 331-335)

   // N parameters
   MinN_Root  = 0.01;//MEL 2006 0.025;            // Whitehead: Grassland Nitrogen
   MaxN_Root  = 0.025;//MEL 2006 0.040; 				 // Guesstimate


   MaxPlantHeight=0.7;            // MEL 2006 //0.3  // Max plant height - taken from grass

   // Phenology
   WaterDeficitVegGrowth = 0.65;

   // N fixation
   DMCostPerN      = 0.7*5.9/0.4;          // "Cost" in dry matter to produce nitrogen (g/g). From pea.
   MaxFixPerDM     = 0.027; // Maximal nitrogen fixation per produced dry matter (g/g). From pea.
   PotNFix         = 0.0;
   NFixationThisDay= 0.0;

   // Canopy structure
   LAIDistributionPar = 1.1;        // Estimated from Lantinga et al
	k            = 0.7;              // Taken from Høgh-Jensen ph.d. thesis and Lantinga et al (Agr.For.Met, 96 1999 71-83)
	BulkDensity  = 1400;             // g DM/m3 standing crop - guesstimated

	MinDMtemp    = 2.0;              // Minimum temperature for dry matter production - guesstimate
   MaxDMtemp    = 10.0;             // Maximum temperature for dry matter production - guesstimate

   // Parameters adjusted/set
	CoeffGreenIdx = 0.018; //(0.02 is a test) 0.0325;            // Coefficient for leaf area (from Nassiri & Elgersma, 1998)

   MaxRadUseEff  = 1.9; 					// Maximum radiation use efficiency

#ifdef BIOMODTEST
   MaxRadUseEff  = 3.8;//MEL 2006 3.2;//3.0;                 // Maximum radiation use efficiency (g/MJ) - fittet on data Foulum 88-90 and 90-92
	PowDM         = -0.32; //0.0; //-0.32;               // Power of dry matter in nitrogen status calculation
   NPctMax       = 6.0; //7.5; //5.0; //4.25; //4.25; //7.4;    // 5.0             // Following Justes concept adjusted to max as: 8.3*4.8/5.35
   DMCostPerN    = 10.0; //4.0/0.42; // From Cannell & Thornley //0.5*5.9/0.4;          // "Cost" in dry matter to produce nitrogen (g/g). From pea.
	MinDMtemp     = 6.0;              // Minimum temperature for dry matter production - guesstimate
   CoeffGreenIdx = 0.01; //0.018; //(0.02 is a test) 0.0325;            // Coefficient for leaf area (from Nassiri & Elgersma, 1998)
   RootPentrRate = 0.5*0.0008;          // new estimate from KTK (18_3_3)
	TransferRate  = 0.02; // TEST REMOVE TEST !!!!!!!!!!!!!0.02;           // Fraction (temperature-dependend) of the four age pools that is transferred to subsequent fraction
	NitrateUptakeRate= 0.5*0.00006; //0.2*0.00006; // NEW value from DAISY (JB 2002-8) 0.000052;     // (0.31) Old value 0.000052. Uptake rate for nitrate-N g/m/d (m root length)
	MaxFixPerDM     = 0.035;     // Maximal nitrogen fixation per produced dry matter (g/g). From pea.
#endif
   TransferRate    = 0.035;//MEL 2006  0.04; //0.02; // TEST REMOVE TEST !!!!!!!!!!!!!0.02;           // Fraction (temperature-dependend) of the four age pools that is transferred to subsequent fraction
   RespirationRoot = 0.003;//MEL 2006 0.005;          // Root respiration given as a daily fraction of DM at 10 deg. Celsius
   MinAllocToRoot    = 0.25; //MEL 2006         0.28 // Minimal fraction of dry matter production that is allocated to the root - Value from DAISY
   MaxAllocToRoot    = 0.3; //MEL 2006          0.28// Maximal fraction of dry matter production that is allocated to the root - Value from DAISY
   RhizoDepositFraction = 0.06;
   DecayRoot = 0.002;
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
void cropLucerne::Add(crop* aCrop, double fraction)
{
   cropRyegrass::Add(aCrop,fraction);
   cropLucerne * c = (cropLucerne*)aCrop;

  	MaxFixPerDM=(1-fraction)*MaxFixPerDM + fraction*c->MaxFixPerDM;
  	DMCostPerN=(1-fraction)*DMCostPerN + fraction*c->DMCostPerN;
  	PotNFix=(1-fraction)*PotNFix + fraction*c->PotNFix;
   NFixationThisDay=(1-fraction)*NFixationThisDay + fraction*c->NFixationThisDay;
}

/****************************************************************************\
\****************************************************************************/
void cropLucerne::ReadParameters(char* filename)
{
   commonData data;
   if (data.readFileWithoutExit("CropParameters.dat"))
   {
	   int sectionName=data.FindSection(CropName);
      int sectionCrop=data.FindSection("Crop");
      int maxes=max(sectionName,sectionCrop);
		if (maxes!=-1)
      {
			GetParameter("MaxFixPerDM",&MaxFixPerDM);
			GetParameter("DMCostPerN",&DMCostPerN);
		}
	}

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



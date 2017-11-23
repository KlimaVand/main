/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "croppea.h"
#include <bstime.h>
#include <products.h>
#include "climclas.h"

cropPea::cropPea(const char * aName, const int aIndex, const base * aOwner)
        :crop(aName,aIndex,aOwner)
{
   // Basal parameters
   CropName = "Pea";
   PlantItemName = "PEA";            // These names maps to products.dat !
   StrawItemName = "PEASTRAW";       // These names maps to products.dat !
   WinterSeed = 0;

   // Root parameters
	MaxRootDepthCrop = 0.7;           // Maximal root depth for this crop
	RootPentrRate    = 0.00086;       // New estimate from KTK (18_3_3)
//	TempSumR2=600;                    // See p. 160 in DAISY-description  Not validated for pea.
   SpecificRootLength=57.0;          // Specific root length, m pr. kg.  (corrected for change in rootradius)

   // DM Production and translocation
	MinDMtemp=4.0;                    // Minimum temperature for dry matter production
	MaxDMtemp=10.0;                   // Maximum temperature for dry matter production

	// ------ Nitrogen content estimated from E.S. Jensen doctoral thesis p. 32-33
   MinN_Root  = 0.028;
   MaxN_Root  = 0.037;
	MinN_Store = 0.025;                // Minimum content of nitrogen i storage organs
	MaxN_Store = 0.042;                // Maximum content of nitrogen i storage organs
   PowDM      = -0.44;                // Power of dry matter in nitrogen status calculation
   NPctMax    = 7.9;                  // Following Justes concept adjusted to max as: 8.3*5.08/5.35


   // LAI and canopy structure
	LAIDistributionPar = 1.5;          // Estimated by from ecological trial at Foulum (1999)
   k                  = 0.85;         // Estimated by from ecological trial at Foulum (1999)
   MaxPlantHeight     = 1.0;          // Max plant height (Value from DAISY)
   Conversfactor      = 0.2;          // Part of green leaf that is converted to yellow leaf by wilting
   MinimumSenescense  = 0.7;

   // Phenology
   // Pea optimised 2002/07 (BMP), using peak flowering estimated on the basis of BBHC 60
   // Reestimation of base temperatures not possible, especially for spring crops highly confounded with T sums

	Phenology->TS0          = 150;              // Sowing to emergence
   Phenology->TS1          = 500;              // Emergence to anthesis Optimised 2002/07 at a value of 425. Adjusted to match peak of LAIg (new definition!).
   Phenology->TS2          = 170;              // Anthesis to end of grainfilling  Optimised 2002/07
   Phenology->TS3          = 360;              // End of grainfilling to ripeness  Optimised 2002/07 Note considerably lower that MARKVAND (78)???
   Phenology->TB0          = 0;                // Corresponds to TS0
   Phenology->TB1          = 4;                // Corresponds to TS1
   Phenology->TB2          = 6;                // Corresponds to TS2
   Phenology->TB3          = 0;                // Corresponds to TS3
   Phenology->DS_Flagligule= 0.9; //0.9;   // 1.8           // Undef
   Phenology->DS_StartFill = 1.04;

	// Water deficits
   WaterDeficitVegGrowth = 0.65;
   WaterDeficitLagPhase  = 0.45;
   WaterDeficitGrainFill = 0.60;

   // Fixation
   DMCostPerN  = 15.0; // "Cost" in dry matter to produce nitrogen (g/g) value from Phillips: Ann. Rev. Plant Physiol. 1980 31:29-49 assuming 40% gC pr gDW.
   MaxFixPerDM = 0.027;// Cannot exceed 1/DMCostPerN. Maximal nitrogen fixation per produced dry matter thius day (g/g). Value from Jensen (1997) p. 23

   // Parameters for fitting  (Calibrated by JB 1/5/2000 harvest values: 1/4/2001)
   MaxRadUseEff = 4.0; //3.5; //4.5;//3.5;          		 // Maximum radiation use efficiency (g/MJ)
   FillFactor=0.4;                   // Fraction of net production after anthesis that goes into grain
   StoreForFilling=0.3;              // Fraction of DM present at initiation of grain filling that is translocated to grain

   // ----------------------- LAI parameters
   InitialCAI = 0.2;                //
	CoeffGreenIdx = 0.0125; //0.0125;          // Coefficient for leaf growth (Optimized)
   LAINitrogenRatio = 0.25; //1.0; //0.25; //0.25; //0.22; //0.42;         // Maximum ratio between LAI and nitrogen in veg top (Estimated from graphs)
   LAIDMRatio = 1.0; //0.01; //0.012; //0.02;      // Maximum ratio between LAI and DM in veg top (Estimated from graphs)

   // State variables
   NFixationThisDay=0.0;
   PotNFix = 0.0;
   AccumulatedNFixation = 0.0;

#ifdef TESTRESP
	MaxRadUseEff = 1.12*MaxRadUseEff*1.1;
#endif

#ifdef MELCAL     									//Based on PEA experiments at Foulum (atpea)
	MaxRootDepthCrop 			= 0.9;            // Maximal root depth for this crop
   MaxRadUseEff 				= 5.3;
   Phenology->TS0          = 100;            // Sowing to emergence
   Phenology->TS1          = 410;            // Emergence to anthesis
   Phenology->TS3          = 360;            // End of grainfilling to ripeness
   FillFactor					= 0.45;           // Fraction of net production after anthesis that goes into grain
   MaxFixPerDM 				= 0.1;            // Maximal nitrogen fixation per produced dry matter (g/g).
	CoeffGreenIdx 				= 0.015;          // Coefficient for leaf growth
   LAINitrogenRatio 			= 0.5;          	// Maximum ratio between LAI and nitrogen in veg top
   LAIDMRatio 					= 0.011;      		// Maximum ratio between LAI and DM in veg top
 #endif
}

/****************************************************************************\
\****************************************************************************/
cropPea::cropPea(const cropPea& acrop)
   : crop(acrop)
{

   fNitrogenAtFlowering = acrop.fNitrogenAtFlowering;
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

 	fNitrogenAtFlowering = (1-fraction)*fNitrogenAtFlowering + fraction*c->fNitrogenAtFlowering;
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
\****************************************************************************/
void cropPea::ReadParameters(char* filename)
{
   crop::ReadParameters(filename);
   if (OpenInputFile(filename))
   {
	   if (FindSection(CropName))
      {
			GetParameter("MaxFixPerDM",&MaxFixPerDM);
			GetParameter("DMCostPerN",&DMCostPerN);
		}
	}
   CloseInputFile();
}

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



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
#include "../../data/commonData.h"
cropPea::cropPea(const char * aName, const int aIndex, const base * aOwner)
        :crop(aName,aIndex,aOwner)
{
   // Basal parameters
   CropName = "Pea";
   PlantItemName = "PEA";            // These names maps to products.dat !
   StrawItemName = "PEASTRAW";       // These names maps to products.dat !
   WinterSeed = false;

   // Parameters (2009 MEL)
   //Root
	MaxRootDepthCrop 		 = 0.9;            // Maximal root depth for this crop (estimated from irrigated and non-irrigated peas at Foulum (MEL))
	RootPentrRate    		 = 0.00086;       // New estimate from KTK (18_3_3)
   SpecificRootLength	 = 57.0;          // Specific root length, m pr. kg.  (corrected for change in rootradius)

   // DM Production and translocation
	MinDMtemp				 = 4.0;                    // Minimum temperature for dry matter production
	MaxDMtemp				 = 10.0;                   // Maximum temperature for dry matter production
   FillFactor				= 0.5;           // Fraction of net production after anthesis that goes into grain  (estimated from irrigated and non-irrigated peas at Foulum (MEL))
   StoreForFilling		= 0.3;              // Fraction of DM present at initiation of grain filling that is translocated to grain

	// ------ Nitrogen content estimated from E.S. Jensen doctoral thesis p. 32-33
   MinN_Root  				 = 0.01;
   MaxN_Root  				 = 0.03;
	MinN_Store 				 = 0.037;                // Minimum content of nitrogen i storage organs, from fodermiddeltabellen
	MaxN_Store 				 = 0.042;                // Maximum content of nitrogen i storage organs, from fodermiddeltabellen
   PowDM      				 = -0.44;                // Power of dry matter in nitrogen status calculation
   NPctMax    				 = 7.9;                  // Following Justes concept adjusted to max as: 8.3*5.08/5.35
   ReducedNInYellow  	 = 0.8;             // The min and max N for yellow VegTop is reduced with this parameter

   // LAI and canopy structure
	LAIDistributionPar 	= 1.5;          // Estimated by from ecological trial at Foulum (1999)
   k                  	= 0.85;         // Estimated by from ecological trial at Foulum (1999)
   MaxPlantHeight     	= 1.0;          // Max plant height (Value from DAISY)
  	Conversfactor     	= 0.1;             // Part of green leaf that is converted to yellow leaf by wilting  (estimated from irrigated and non-irrigated peas at Foulum (MEL))
   MinimumSenescense  	= 0.7;
   InitialCAI 				= 0.2;                //
	CoeffGreenIdx 			= 0.0125;           // Coefficient for leaf growth (Optimized)
   LAINitrogenRatio 		= 0.25;        // Maximum ratio between LAI and nitrogen in veg top (Estimated from graphs)
   LAIDMRatio 				= 1.2;       // Maximum ratio between LAI and DM in veg top (Estimated from graphs)

   MaxRadUseEff 			= 5.0;            // (estimated from irrigated and non-irrigated peas at Foulum (MEL))

   // Phenology
   Phenology->TS0       = 100;            // Sowing to emergence (estimated from irrigated and non-irrigated peas at Foulum (MEL))
   Phenology->TS1       = 450;            // Emergence to anthesis (estimated from irrigated and non-irrigated peas at Foulum (MEL))
   Phenology->TS2       = 300;              // Anthesis to end of grainfilling (estimated from irrigated and non-irrigated peas at Foulum (MEL))
   Phenology->TS3       = 360;            // End of grainfilling to ripeness (estimated from irrigated and non-irrigated peas at Foulum (MEL))
   Phenology->TB0       = 0;                // Corresponds to TS0
   Phenology->TB1       = 4;                // Corresponds to TS1
   Phenology->TB2       = 6;                // Corresponds to TS2
   Phenology->TB3       = 0;                // Corresponds to TS3
   Phenology->DS_Flagligule	= 0.9;    // Undef
   Phenology->DS_StartFill = 1.04;

	// Water deficits
   WaterDeficitVegGrowth = 0.65;
   WaterDeficitLagPhase  = 0.45;
   WaterDeficitGrainFill = 0.60;

   // Fixation
   DMCostPerN  			= 15.0; // "Cost" in dry matter to produce nitrogen (g/g) value from Phillips: Ann. Rev. Plant Physiol. 1980 31:29-49 assuming 40% gC pr gDW.
   MaxFixPerDM 			= 0.02;// Cannot exceed 1/DMCostPerN. Maximal nitrogen fixation per produced dry matter thius day (g/g). Value from Jensen (1997) p. 23

   // State variables
   NFixationThisDay		= 0.0;
   PotNFix 					= 0.0;
   AccumulatedNFixation = 0.0;
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
void cropPea::ReadParameters(string filename)
{
	crop::ReadParameters(filename);
    commonData data;
   if (data.readFileWithoutExit("CropParameters.dat"))
   {
	   int sectionName=data.FindSection(CropName);


		if (sectionName!=-1)
      {
			data.FindItem("MaxFixPerDM",sectionName,MaxFixPerDM);
			data.FindItem("DMCostPerN",sectionName,DMCostPerN);
		}
	}

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



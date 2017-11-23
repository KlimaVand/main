/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "crop.h"
#include "../../base/bstime.h"
#include "../../base/base.h"


#include "../../base/climate.h"
#include "../../products/products.h"
#include "../../base/nixcbits.h"
#include "phenology.h"
#include "../../base/typer.h"
#include "../../base/commonData.h"
#include "../../tools/compare.h"
#include "../../base/settings.h"
/****************************************************************************\
\****************************************************************************/
crop::crop(const char * aName, const int aIndex, const base * aOwner, string cropName)
   :base(aName,aIndex,aOwner)
{
   // Parameters

   CropName = cropName;
   commonData * data=globalSettings[id]->CropInformation;
   data->FindSection(CropName);
   PlantItemName = "";                   // These names maps to products.dat !
   StrawItemName = "";                   // These names maps to products.dat !
   C4Photosynthesis = false;
   UnderSown = false;                    // indicates if a crop are not going to be harvested (i.e. undersown crops)
	GrowthContinue = false;
	WinterSeed = true;                    // Is the present crop a wintercrop
   terminated=false;
   CutOrHarvested = false;

   //-------------------------- Parameters usually unchanged -------------------
   // These parameters may not be changed here in class 'crop' at all !
   data->FindItem("DMCostPerN",&DMCostPerN);

   data->FindItem("MaxFixPerDM",&MaxFixPerDM);
   // Root
   data->FindItem("RootpF",&RootpF);         // Water potential at wilting point
   data->FindItem("RootPentrRate",&RootPentrRate);      // Root penetration rate (m d-1 deg C-1)
   data->FindItem("RootDistrParm",&RootDistrParm);        // Root density distribution parameter
   data->FindItem("TempRootMin",&TempRootMin);         // Minimum temperature for root growth  new value by JB 19_3_3
   data->FindItem("SpecificRootLength",&SpecificRootLength); //191;   // Specific root length, m pr. kg.
   data->FindItem("RootDensAtBottom",&RootDensAtBottom);//191;   // Rootdensity at rootdepth (100*191/100)

   data->FindItem("MinimumRootDens",&MinimumRootDens); //500;   BMP: why zero ???!!!
   data->FindItem("InitialRootDepth",&InitialRootDepth);	       // Root depth by germination
   data->FindItem("NitrateUptakeRate",&NitrateUptakeRate);    // Uptake rate for nitrate-N g/m/d (m root length)
   data->FindItem("AmmoniumUptakeRate",&AmmoniumUptakeRate);       // NEW value from DAISY (JB 2002-8) 0.0031;       // Uptake rate for ammonium-N g/m/d (m root length)    0.06*100/191
   data->FindItem("MinimumSoilNitrate",&MinimumSoilNitrate);       // Minimum concentration of nitrate-N left in soil water (g/l)
   data->FindItem("MinimumSoilAmmonium",&MinimumSoilAmmonium);       // Minimum concentration of ammonium-N left in soil water (g/l)
   data->FindItem("DecayRoot",&DecayRoot);         // Root decay given as a daily fraction of DM at 10 deg. Celsius
   data->FindItem("MaxRootDepth",&MaxRootDepthCrop);          // Maximal root depth for this crop

   // DM production and translocation
   data->FindItem("TopFraction",&TopFraction);       // Fraction of seed that goes into top
   data->FindItem("ConversionCoefficient",&ConversionCoefficient);        // Coefficent of DM conversion from vegetative to storage
   data->FindItem("GerminationConversion",&GerminationConversion);         // Conversion factor seed DM -> plant DM
   data->FindItem("MinAllocToRoot",&MinAllocToRoot);        // Minimal fraction of dry matter production that is allocated to the root
   data->FindItem("MaxAllocToRoot",&MaxAllocToRoot);       // Maximal fraction of dry matter production that is allocated to the root
   data->FindItem("MaxRadUseEff",&MaxRadUseEff);      // Maximum radiation use efficiency (g/MJ)
   data->FindItem("MinDMtemp",&MinDMtemp);          // Minimum temperature for dry matter production (4 in Hansen et al)
   data->FindItem("MaxDMtemp",&MaxDMtemp);        // Maximum temperature for dry matter production
   data->FindItem("StoreForFilling",&StoreForFilling);     // Fraction of DM present at initiation of grain filling that is translocated to grain
                                        // Estimated fra JEO data, but fits with 0.4 reported by de Vries et. al. "Simulation of ecophysiologival processes of growht in several annual crops"

   // Nitrogen
   data->FindItem("NCurveParam",&NCurveParam);                   // Parameter giving the N-curve shape

   data->FindItem("MinN_Store",&MinN_Store);             // Minimum content of nitrogen i storage organs (Estimeret fra Elly's data)
   data->FindItem("MaxN_Store",&MaxN_Store);              // Maximum content of nitrogen i storage organs
   data->FindItem("MinN_Root",&MinN_Root);         	 // Minimum content of nitrogen i root organs
   data->FindItem("MaxN_Root",&MaxN_Root);          	 // Maximum content of nitrogen i root organs
   data->FindItem("PowDM",&PowDM);        	 // Power of dry matter in nitrogen status calculation  (Justes et al., 1994)
   data->FindItem("NPctMax",&NPctMax);            	 // Following Justes concept (Justes et al., 1994)
   data->FindItem("NPctMin",&NPctMin);    	 		 // Following Justes concept (Justes et al., 1994)
   data->FindItem("ReducedNInYellow",&ReducedNInYellow);             // The min and max N for yellow VegTop is reduced with this parameter

	// LAI and canopy structure
   data->FindItem("k",&k);           // Extinction coefficient for photosynthetic active radiation
   data->FindItem("InterceptCoeff",&InterceptCoeff);            // Coeff. for interceptioncapacity.
   data->FindItem("PhotoSyntActPart",&PhotoSyntActPart);        // Photosynthetic active part of total global radiation
   data->FindItem("CoeffGreenIdx",&CoeffGreenIdx);        // Coefficient for leaf growth
   data->FindItem("LAINitrogenRatio",&LAINitrogenRatio);            // Maximum ratio between LAI and nitrogen in veg top
   data->FindItem("LAIDMRatio",&LAIDMRatio);         // Maximum ratio between LAI and DM in veg top
   data->FindItem("InitialCAI",&InitialCAI);           // Initial green leaf area index after 200 degree days
   data->FindItem("MinimumSenescense",&MinimumSenescense);        // Used to calculate minimum senescense - value as calibrated for winter wheat
   data->FindItem("MaxPlantHeight",&MaxPlantHeight);          // Max plant height (m)
   data->FindItem("LAIDistributionPar",&LAIDistributionPar);            // Distribution of LAI in canopy
   data->FindItem("Conversfactor",&Conversfactor);       // Part of green leaf that is converted to yellow leaf by wilting

   data->FindItem("ColonialisationRate",&ColonialisationRate);         // Expansion of fraction area occupied (per day per degree Celcius)

   data->FindItem("WaterDeficitVegGrowth",&WaterDeficitVegGrowth);
   data->FindItem("WaterDeficitLagPhase",&WaterDeficitLagPhase);
   data->FindItem("WaterDeficitGrainFill",&WaterDeficitGrainFill);

   data->FindItem("FractionNminInRoots",&FractionNminInRoots);       //Fraction of N in roots present as mineral N
   if(FractionNminInRoots<0)
	   theMessage[id]->FatalError("problem");
   data->FindItem("FractionNminInVegTop",&FractionNminInVegTop);     //Fraction of N in VegTop present as mineral N
   data->FindItem("CO2Effect",&CO2Effect);
   data->FindItem("MaxGLAICrop",&MaxGLAICrop);




   data->FindItem("RhizoDepositFraction",&RhizoDepositFraction);
   data->FindItem("FillFactor",&FillFactor);           // Fraction of net production after anthesis that goes into grain

   AccumulatedRootNDeposit.Clear();  // Accumulated root N deposition (state variable)
   AccumulatedTopNDeposit.Clear();   // Accumulated root N deposition (state variable)
   AccumulatedNProduction.Clear();   // Accumulated N production


	// State variables
   AccumulatedDMProduction = 0.0;    // Accumulated DM production
   AccumulatedNFixation = 0.0;       // Accumulated N fixation (used for output)
   AccumulatedRootDeposit = 0.0;     // Accumulated root deposition (state variable)
   AccumulatedRootRespiration = 0.0; // Measured in DM
   AccumulatedTopDeposit = 0.0;      // Accumulated root deposition (state variable)
   DeltaDMTop           = 0.0;       // Daily top DM increase (used for output)
   DryMatterRoot        = 0.0;       // Dry matter in root.
   DryMatterStorage     = 0.0;       // Dry matter in storage organs.
   DryMatterTotalStandVegTop = 0.0;  // Presently unused
   DryMatterVegTop      = 0.0;       // Dry matter in veg. parts above soil surface.
   fNitrogenAfterRipe   = 0.0;
   fractionOfSpace      = 1.0;       // Fraction of total area occupied by crop
   GreenCropAreaIndex   = 0.0;       // Green leaf crop index.
   InitialSeedDM        = 0.0;       // The initial DM in seed
   InterceptedRadiation = 0.0;       // Radiation intercepted
   maxGLAI              = 0.0;       // Maximum green crop area index reached
	MaxRootDepth=MaxRootDepthCrop;    // Max. root depth. May be reduced later, depending on soil.
   PlantHeight          = 0.0;       // Height (m)
   RelativeDensity      = 1.0;
   Rg                   = 0.0;
   RootDensAtSurf       = 0.0;       // Root density at soil surface.
   RootDepth            = 0.0;       // Root depth.
   RootRespiration      = 0.0;       // Daily root respiration (DM m-2)
   RootTranslocation    = 0.0;       // Accumulated root translocation (state variable)
   SeedDM               = 0.0;       // The DM content of seed
   temp                 = 0.0;       // temperature
   TempSumRoot          = 0.0;       // Temperature sum, root.
   totalLAIStand        = 0.0;       // Sum of leaf area index for all plants
   TotalRootLength      = 0.0;       // Total root length.
   TransferableStorage  = 0.0;       // Amount to be transfer from vegetative parts to storage
   TransferedDMToStorage= 0.0;       // Actual amount to be transfer from vegetavive parts to storage
   TranspirationRatio   = 1.0;
   YellowCropAreaIndex  = 0.0;       // Yellow leaf crop index.

	Nitrogen.Clear();                 // Nitrogen content (g/m^2).
//   FractionInStorage    = 0.0;       // Fraction of dry matter constituting of storage organs.
//   CurrentNPct          = 10.0;
//   rootMatter = NULL;                // Transfered to soil
   //next three lines added by NJH March 2009
	      rootMatter = new decomposable; // Remark: rootmatter is transfered to soil in patch.cpp !
	      rootMatter->Setname("ROOTMATTER");
	      theProducts[id]->GiveProductInformation(rootMatter);
   topMatter  = NULL;                // Transfered to soil
	NumOfLayers=MaxSoilLayers;        // Number of soil layers.
   for (int i=0;i<MaxSoilLayers;i++)
   {
		RootLengthList[i]=0.0;         // 'RootLengthList' contains the root length (m) per layer.
   	LayerThickness[i]=-1;          // Thickness (mm) per layer.
   }
   Nbudget.SetNames("crop","N");
   N15budget.SetNames("crop","N15");
   DMbudget.SetNames("crop","DM");
   aSoil        	= NULL;
   Phenology    	= new phenology;
}

/****************************************************************************\
Destructor.
\****************************************************************************/
crop::~crop()
{
	if (topMatter)
      delete topMatter;
   if (rootMatter)
      delete rootMatter;
   if (Phenology)
      delete Phenology;
   topMatter  = NULL;
   rootMatter = NULL;
   Phenology  = NULL;
}

/****************************************************************************\
\****************************************************************************/
crop::crop(const crop& acrop)
{
   CropName                 = acrop.CropName;
	PlantItemName				 = acrop.PlantItemName;
   StrawItemName            = acrop.StrawItemName;
   terminated               = acrop.terminated;
   PhotoSyntActPart         = acrop.PhotoSyntActPart;           // Photosynthetic active part of total global radiation
	RootpF                   = acrop.RootpF;                     // Water potential at wilting point
	MinDMtemp                = acrop.MinDMtemp;                  // Minimum temperature for dry matter production
	MaxDMtemp                = acrop.MaxDMtemp;                  // Maximum temperature for dry matter production
	MaxRadUseEff             = acrop.MaxRadUseEff;               // Maximum radiation use efficiency (g/MJ)
	CoeffGreenIdx            = acrop.CoeffGreenIdx;              // Coefficient for leaf growth, exp. phase
   LAINitrogenRatio         = acrop.LAINitrogenRatio;           // Maximum ratio between LAI and nitrogen in veg top
   LAIDMRatio               = acrop.LAIDMRatio;                 // Maximum ratio between LAI and DM in veg top
   MinimumSenescense        = acrop.MinimumSenescense;          // Used to calculate minimum senescense
   k                        = acrop.k;

	NCurveParam              = acrop.NCurveParam;              // Power parameter of nitrogen fraction
   MinN_Store               = acrop.MinN_Store;               // Minimum content of nitrogen i storage organs
	MaxN_Store               = acrop.MaxN_Store;               // Maximum content of nitrogen i storage organs
	MinN_Root   				 = acrop.MinN_Root;                // Minimum content of nitrogen i root organs
	MaxN_Root   				 = acrop.MaxN_Root;                // Maximum content of nitrogen i root organs
   PowDM       				 = acrop.PowDM;                    // Power of dry matter in nitrogen status calculation
   NPctMax       				 = acrop.NPctMax;                  // Following Justes concept
   NPctMin       				 = acrop.NPctMin;                  // Following Justes concept
   DryMatterTotalStandVegTop= acrop.DryMatterTotalStandVegTop;
   Conversfactor            = acrop.Conversfactor;             // Part of green leaf that is converted to yellow leaf by wilting
	InitialRootDepth         = acrop.InitialRootDepth;          // Root depth by germination
	RootPentrRate            = acrop.RootPentrRate;             // Root penetration rate
	TempRootMin              = acrop.TempRootMin;               // Minimum temperature for root growth
	SpecificRootLength       = acrop.SpecificRootLength;        // Specific root length, m pr. kg.
	MaxAllocToRoot           = acrop.MaxAllocToRoot;            // Maximal fraction of dry matter production that is allocated to the root
	MinAllocToRoot           = acrop.MinAllocToRoot;            // Minimal fraction of dry matter production that is allocated to the root
	MaxRootDepthCrop         = acrop.MaxRootDepthCrop;          // Maximal root depth for this crop
   DecayRoot                = acrop.DecayRoot;
	Phenology                = new phenology(*acrop.Phenology); // Phenology

   WaterDeficitVegGrowth = acrop.WaterDeficitVegGrowth;
   WaterDeficitLagPhase  = acrop.WaterDeficitLagPhase;
   WaterDeficitGrainFill = acrop.WaterDeficitGrainFill;

   FractionNminInRoots = acrop.FractionNminInRoots;
   if(FractionNminInRoots<0)
   	   theMessage[id]->FatalError("problem");
   FractionNminInVegTop = acrop.FractionNminInVegTop;

// TempSumR1  = acrop.TempSumR1;                               //     See p. 160 in DAISY-description
//	TempSumR2  = acrop.TempSumR2;                               //     See p. 160 in DAISY-description
	temp       = acrop.temp;

   MaxPlantHeight         = acrop.MaxPlantHeight;
//   DryMatterVegTopLinear  = acrop.DryMatterVegTopLinear;
   InitialSeedDM          = acrop.InitialSeedDM;
   InitialCAI             = acrop.InitialCAI;
   MinimumRootDens        = acrop.MinimumRootDens;
   ReducedNInYellow       = acrop.ReducedNInYellow;

	InterceptCoeff         = acrop.InterceptCoeff;                           // Coeff. for interceptioncapacity.
	NitrateUptakeRate      = acrop.NitrateUptakeRate;            // Uptake rate for nitrate-N g/m/d (m root length)
	AmmoniumUptakeRate     = acrop.AmmoniumUptakeRate;           // Uptake rate for ammonium-N g/m/d (m root length)
   MinimumSoilNitrate     = acrop.MinimumSoilNitrate;           // Minimum concentration of nitrate-N left in soil water (g/l)
   MinimumSoilAmmonium    = acrop.MinimumSoilAmmonium;          // Minimum concentration of ammonium-N left in soil water (g/l)
	WinterSeed             = acrop.WinterSeed;                   // Is the present crop a wintercrop.
   ConversionCoefficient  = acrop.ConversionCoefficient;        // Coefficent of DM conversion from vegetative to storage
   FillFactor             = acrop.FillFactor;
   GerminationConversion  = acrop.GerminationConversion;
   fNitrogenAfterRipe     = acrop.fNitrogenAfterRipe;
//   fNitrogenAtFlowering   = acrop.fNitrogenAtFlowering;
   RhizoDepositFraction   = acrop.RhizoDepositFraction;
   CO2Effect   			  = acrop.CO2Effect;
   MaxGLAICrop            = acrop.MaxGLAICrop;
   RelativeDensity        = acrop.RelativeDensity;
   TopFraction            = acrop.TopFraction;
   RootTranslocation      = acrop.RootTranslocation;
   AccumulatedDMProduction= acrop.AccumulatedDMProduction;
   AccumulatedTopDeposit  = acrop.AccumulatedTopDeposit;
   InterceptedRadiation   = acrop.InterceptedRadiation;
   StoreForFilling        = acrop.StoreForFilling;
//   CurrentNPct            = acrop.CurrentNPct;
   TransferedDMToStorage  = acrop.TransferedDMToStorage;
 	AccumulatedRootNDeposit= acrop.AccumulatedRootNDeposit;
   AccumulatedTopNDeposit = acrop.AccumulatedTopNDeposit;
 	AccumulatedNProduction = acrop.AccumulatedNProduction;
   CutOrHarvested         = acrop.CutOrHarvested;
   FillFlag               = acrop.FillFlag;
   C4Photosynthesis       = acrop.C4Photosynthesis;
	// State variables
   maxGLAI              =acrop.maxGLAI;                      // Maximum green crop area index reached
	RootDensAtBottom     =acrop.RootDensAtBottom;             // Rootdensity at rootdepth. Changes with seed density.
	MaxRootDepth         =acrop.MaxRootDepth;                 // Max. root depth at actual soil type.
	RootDistrParm        =acrop.RootDistrParm;                // Root density distribution parameter
	Nitrogen             =acrop.Nitrogen;                     // Nitrogen content (g/m^2).

	TempSumRoot          =acrop.TempSumRoot;                  // Temperature sum, root.
	RootDensAtSurf       =acrop.RootDensAtSurf;               // Root density at soil surface.
	TotalRootLength      =acrop.TotalRootLength;              // Total root length.
	RootDepth            =acrop.RootDepth;                    // Root depth.
   DryMatterVegTop      =acrop.DryMatterVegTop;              // Dry matter in veg. parts above soil surface.
	DryMatterRoot        =acrop.DryMatterRoot;                // Dry matter in root.
	DryMatterStorage     =acrop.DryMatterStorage;             // Dry matter in storage organs.
   ColonialisationRate  =acrop.ColonialisationRate;
   totalLAIStand        =acrop.totalLAIStand;
   fractionOfSpace      = acrop.fractionOfSpace;
//	FractionInStorage    =acrop.FractionInStorage;            // Fraction of dry matter constituting of storage organs.
	GreenCropAreaIndex   =acrop.GreenCropAreaIndex;           // Green leaf crop index.
	YellowCropAreaIndex  =acrop.YellowCropAreaIndex;          // Yellow leaf crop index.
	NumOfLayers          =acrop.NumOfLayers;                  // Number of soil layers.
   UnderSown            =acrop.UnderSown;                    // indicates if a crop are not going to be harvested (i.e. undersown crops)
	GrowthContinue       =acrop.GrowthContinue;
   PlantHeight          =acrop.PlantHeight;
   LAIDistributionPar   =acrop.LAIDistributionPar;
   if (acrop.rootMatter==NULL)
   	rootMatter	= NULL;
   else
      rootMatter        =	new decomposable(*acrop.rootMatter);
   if (acrop.topMatter==NULL)
      topMatter         = NULL;
   else
      topMatter         =new decomposable(*acrop.topMatter);
   SeedDM               =acrop.SeedDM;
   TranspirationRatio   =acrop.TranspirationRatio;
   TransferableStorage  =acrop.TransferableStorage;
   AccumulatedNFixation =acrop.AccumulatedNFixation;         // Accumulated N fixation (used for output)
   DeltaDMTop           =acrop.DeltaDMTop;                   // Daily top DM increase (used for output)

   /* Ignored for now !!!!!!!!!!!!!!!
   DateMeasured[MaxMeasuredPoints];
   DMmeasured[MaxMeasuredPoints]
   GLAIMeasured[MaxMeasuredPoints]
   NTopMeasured[MaxMeasuredPoints]
   DMmeasuredDeriv[MaxMeasuredPoints]
   GLAIMeasuredDeriv[MaxMeasuredPoints]
   NTopMeasuredDeriv[MaxMeasuredPoints];
   */

	for (int i=0;i<MaxSoilLayers;i++)
   {
		RootLengthList[i]=acrop.RootLengthList[i];             // 'RootLengthList' contains the root length (m) per layer.
		LayerThickness[i]=acrop.LayerThickness[i];
   }

	Rg         = acrop.Rg;
   aSoil      = NULL;        											// Set subsequently
   Nbudget    = acrop.Nbudget;
   N15budget  = acrop.N15budget;
   DMbudget   = acrop.DMbudget;
}

/**
 * Scales the radiation use efficiency according to crop rotation and
 *  pesticide use. Calling this function should remain optional.
 */
void crop::SetRUEfactor(double f)
{
   if (f>1.0 || f<0.0)
      theMessage[id]->FatalError("crop::SetRUEfactor - factor outside legal range");
   MaxRadUseEff=MaxRadUseEff*f;
}



/**
 * is incomplete. Should either be updated or not called
 */
void crop::Add(crop* aCrop, double fraction)
{
	theMessage[id]->FatalError("crop::Add Function is incomplete and needs update before use");
   Nbudget.Add(aCrop->Nbudget,fraction);
   N15budget.Add(aCrop->N15budget,fraction);
   DMbudget.Add(aCrop->DMbudget,fraction);
	if (CropName!=aCrop->GetCropName())
   	theMessage[id]->FatalError("crop::Add - Different crops can not be added");
   else
   {
		Nitrogen            = Nitrogen*(1-fraction) + aCrop->Nitrogen*fraction;
		if(Nitrogen.n<=0)
			 theMessage[id]->FatalError("problem");
		maxGLAI             = (1-fraction)*maxGLAI + fraction*aCrop->maxGLAI;
		RootDensAtBottom    = (1-fraction)*RootDensAtBottom + fraction*aCrop->RootDensAtBottom;
		RootDensAtSurf      = (1-fraction)*RootDensAtSurf + fraction*aCrop->RootDensAtSurf;
		MaxRootDepth        = (1-fraction)*MaxRootDepth + fraction*aCrop->MaxRootDepth;
		RootDistrParm       = (1-fraction)*RootDistrParm + fraction*aCrop->RootDistrParm;
   	DecayRoot			  = (1-fraction)*DecayRoot + fraction*aCrop->DecayRoot;
      TempSumRoot         = (1-fraction)*TempSumRoot + fraction*aCrop->TempSumRoot;
		TotalRootLength     = (1-fraction)*TotalRootLength + fraction*aCrop->TotalRootLength;
		RootDepth           = (1-fraction)*RootDepth + fraction*aCrop->RootDepth;
		DryMatterVegTop     = (1-fraction)*DryMatterVegTop + fraction*aCrop->DryMatterVegTop;
		DryMatterRoot       = (1-fraction)*DryMatterRoot + fraction*aCrop->DryMatterRoot;
		DryMatterStorage    = (1-fraction)*DryMatterStorage + fraction*aCrop->DryMatterStorage;
//		FractionInStorage   = (1-fraction)*FractionInStorage + fraction*aCrop->FractionInStorage;
		GreenCropAreaIndex  = (1-fraction)*GreenCropAreaIndex + fraction*aCrop->GreenCropAreaIndex;
		YellowCropAreaIndex = (1-fraction)*YellowCropAreaIndex + fraction*aCrop->YellowCropAreaIndex;
		for (int i=0;i<MaxSoilLayers;i++)
			RootLengthList[i] = (1-fraction)*RootLengthList[i] + fraction*aCrop->RootLengthList[i];

   	Phenology->Add(aCrop->Phenology,fraction);
   }

}

/**
 * Returns the effect of temperature on various processes
 * t - temperature (degreeC)
 */
double crop::TemperatureEffect(double t)
{
   double f = 0.0;
   if (t>0.0 && t<=20.0)
      f = 0.1*t;
   if (t>20.0)
      f = exp(0.47-0.027*t+0.00193*t*t);
   return f;
}

/**
 * returns extinction coefficient for photosynthetic active radiation
 */
double crop::GiveExtCoeff() const
{
	return k;
}

/**
 * updates the height of the plant
 */
void crop::UpdateHeight()
{
   if (Phenology->Emerged())
		PlantHeight = MaxPlantHeight*(1.0-Phenology->GetFractionToAnthesis());
   PlantHeight = min(PlantHeight,MaxPlantHeight);
}

/**
 * Gives leaf area fraction below a certain height
 */
double crop::LeafAreaFraction(double height) const
{
   double a=LAIDistributionPar;
   if (PlantHeight==0)
      return 0;
   if (height>=PlantHeight)
      return 1.0;
   return (a+1)*(a+2)*(pow(height,a+1))/(pow(PlantHeight,a+1))*(1/(a+1)-height/((a+2)*PlantHeight));

}

/**
 * Gives the total leaf area index in a certain layer of the canopy
 */
double crop::TotalLeafAreaIndex(double height,double thickness) const
{
	return (GreenCropAreaIndex+YellowCropAreaIndex)*(LeafAreaFraction(height+thickness)-LeafAreaFraction(height));
}

/**
 * Gives the green leaf area in a specific layer in canopy assuming that the yellow leaves are situated i the lower part of the canopy
 below the green leaves
 */
double crop::GreenLeafAreaIndex(double height,double thickness)
{
   // assumes that all yellow area are situated at the bottom of the canopy below the green area
   double UpperArea = max(0.0,(GreenCropAreaIndex+YellowCropAreaIndex)*LeafAreaFraction(height+thickness)-YellowCropAreaIndex);
   double LowerArea = max(0.0,(GreenCropAreaIndex+YellowCropAreaIndex)*LeafAreaFraction(height)-YellowCropAreaIndex);
   double value=UpperArea-LowerArea;
      if(value<0.00001)
      {
   	   return 0;
      }
      else
      {
    	  return value;
      }
}


/**
 * Update soil information, NumOfLayers and MaxRootDepth
 */
void crop::AssignRootParameters(soil* CropSoil)
{
   aSoil        = CropSoil;
   NumOfLayers  = min(aSoil->GetLayers(),(int) MaxSoilLayers);
	MaxRootDepth = min(aSoil->GetMaxRootDepth()/1000,MaxRootDepthCrop);
}

/**
 * An account of the DM lost via "GerminationConversion" must be added, in order
 * to keep total balance of DM/carbon !!!
 */
void crop::Sow(double SeedDensDryMatt,double aRelativeDensity,double NitrogenInSeed)
{
	if (Phenology->Sown())
      theMessage[id]->FatalError("crop::Sow - this crop instance has already been sown");
   //if (fabs(aRelativeDensity)<1e-5)
   //   theMessage[id]->FatalError("crop::Sow - density to low");

   Phenology->Sow();
	TotalRootLength   = 0.0;
	SeedDM            = SeedDensDryMatt*GerminationConversion;
   InitialSeedDM     = SeedDM;
	RootDepth         = InitialRootDepth;
	DryMatterRoot     = 0.0;
	DryMatterVegTop   = 0;
	DryMatterStorage  = 0.0;
	GreenCropAreaIndex= 0.0;

	FillFlag=0;
	Nitrogen.SetBoth(NitrogenInSeed,0.0);
   RelativeDensity = aRelativeDensity;

   Nbudget.SetInput(NitrogenInSeed);             // Budgets
   DMbudget.SetInput(SeedDensDryMatt*GerminationConversion);
   Nbudget.SetOutput(0);
   N15budget.SetOutput(0);
   DMbudget.SetOutput(0);
}

/**
 * harvest the current crop. The drymatter and straw is stored in the input parameters. Nitroget is updated
 * The plant is still alive
 */
void crop::Harvest(decomposable* Storage, decomposable* Straw)
{
	static int i=0;
		i++;
		if(i==12)
			cout<<flush;
  CutOrHarvested = true;
   Storage->Setname(PlantItemName);
   if (PlantItemName!="")
   {
	   theProducts[id]->GiveProductInformation(Storage);
	   //converting from drymatter to fresh weight
	   Storage->Setamount(DryMatterStorage/Storage->GetdryMatter());//??
   }

   Straw->Setname(StrawItemName);
   theProducts[id]->GiveProductInformation(Straw);
   //converting from drymatter to fresh weight
   Straw->Setamount(DryMatterVegTop/Straw->GetdryMatter()); //??

   // Nitrogen
	nitrogen RootN, StorageN, StrawN;

   //check amount of nitrogen at harvest
   double minimumN = Nmin();
   double maximumN = Nmax();

   if (minimumN > Nitrogen.n * 1.25)  //Corrected MEL 2009
   {
     // cout << GetLongName() << endl;
   	theMessage[id]->WarningWithDisplay("crop::Harvest - not enough nitrogen in plant at harvest");
   }
   if (maximumN < Nitrogen.n * 0.75)  //Corrected MEL 2009
   {
      //cout << GetLongName() << endl;
	   theMessage[id]->WarningWithDisplay("crop::Harvest - too much nitrogen in plant at harvest");
   }
   //partition between compartments
   double fN = fNitrogen();
   double TotalRootN  = (CorrectedMinN_Root()+fN*(CorrectedMaxN_Root()-CorrectedMinN_Root()))*DryMatterRoot;
   if(TotalRootN<0)
   {
	   cout<<CorrectedMinN_Root()<<endl;
	   cout<<fN<<endl;

   }
      double TotStorageN = (CorrectedMinN_Store()+fN*(CorrectedMaxN_Store()-CorrectedMinN_Store()))*DryMatterStorage;
   double N15Ratio    = Nitrogen.Get15NRatio();
   double TotalStrawN =  (NpctMinVegTop()/100+fN*(NpctMaxVegTop()-NpctMinVegTop())/100)*DryMatterVegTop;
   double Nrest = Nitrogen.n - TotalRootN - TotStorageN - TotalStrawN;

   //rest of N is allocated to root
   if ((Nrest<-0.01)||(Nrest>0.01))   //if Nrest positive: there is some unallocated N in the plant
   {
      theMessage[id]->WarningWithDisplay("crop::Harvest - Nitrogen content adjusted");
      TotalRootN = TotalRootN+Nrest;
   }


   RootN.SetBoth(TotalRootN,N15Ratio*TotalRootN);
   StorageN.SetBoth(TotStorageN,N15Ratio*TotStorageN);
   StrawN = Nitrogen - RootN - StorageN;
   if (Storage->GetAmount()>0)
   	Storage->SetorgN_content(StorageN/Storage->GetAmount());
   if (Straw->GetAmount()>0)
   {
  		Straw->SetNO3_content(StrawN/Straw->GetAmount()*(0.5*FractionNminInVegTop));
        Straw->SetNH4_content(StrawN/Straw->GetAmount()*(0.5*FractionNminInVegTop));
        Straw->SetorgN_content(StrawN/Straw->GetAmount()*(1.0-FractionNminInVegTop));
     }

	Nitrogen = Nitrogen-StorageN-StrawN;

	if(Nitrogen.n<=0)
		 theMessage[id]->FatalError("problem");
	if (!Phenology->Ripe())
      theMessage[id]->Warning("crop::Harvest - crop not ready for harvest");
   if (DryMatterVegTop<0 || DryMatterStorage<0 || StorageN.n<0 || StrawN.n<0 || Nitrogen.n<0)
   {
	   theMessage[id]->WarningWithDisplay("crop:: Harvest values should not be negative");
      StorageN.n = max(0.0,StorageN.n);
      StrawN.n = max(0.0,StrawN.n);
	}

   Nbudget.AddOutput(StorageN.n+StrawN.n);                    // Budgets
   N15budget.AddOutput(StorageN.n15+StrawN.n15);                    // Budgets
   DMbudget.AddOutput(DryMatterVegTop+DryMatterStorage);

	DryMatterVegTop    = 0.0;
	DryMatterStorage   = 0.0;
	GreenCropAreaIndex = 0.0;
	YellowCropAreaIndex= 0.0;
//	FractionInStorage  = 0.0;
   PlantHeight        = 0.1;                      // Modify to real cutting height!!!!!
	topMatter          = NULL;
//   rootMatter         = NULL;

   double NRemain, DMRemain;
   EndBudget(&NRemain,&DMRemain);
}

/**
 * Kills the plant. The length of the root, the amount of root and amount of straw is returned
 */
void crop::Terminate(decomposable* Straw,decomposable* DeadRoot,double *& RootLengthList)
{
	if (!Phenology->Ripe() && !GrowthContinue && !terminated)               // warning if a non-permanent crop is not ripe
      theMessage[id]->Warning("crop::Terminate - should not terminate crop at this phase");
   if (!Phenology->Emerged())
      theMessage[id]->WarningWithDisplay("crop::Terminate - attempt to terminate a crop that has not even emerged!!!");

   if (DryMatterVegTop+DryMatterStorage>0.0)
   {
   	Straw->Setname(StrawItemName);
	   theProducts[id]->GiveProductInformation(Straw);
      Straw->Setamount((DryMatterVegTop+DryMatterStorage)/Straw->GetdryMatter());

	   nitrogen StrawN = Nitrogen - NitrogenInRoot();
	   if (Straw->GetAmount()>0)
      {
			Straw->SetNO3_content(StrawN/Straw->GetAmount()*(0.5*FractionNminInVegTop));
         Straw->SetNH4_content(StrawN/Straw->GetAmount()*(0.5*FractionNminInVegTop));
         Straw->SetorgN_content(StrawN/Straw->GetAmount()*(1.0-FractionNminInVegTop));
 	   }
 	   Nitrogen = Nitrogen-StrawN;
   }

   RootLengthList= GiveRootLengthList();

   DeadRoot->Setname("ROOTMATTER");
   theProducts[id]->GiveProductInformation(DeadRoot);

   DeadRoot->Setamount(DryMatterRoot/DeadRoot->GetdryMatter());


   if (DryMatterRoot>0)
   {

   	DeadRoot->SetNH4_content(Nitrogen/DeadRoot->GetAmount()*0.5*FractionNminInRoots);
   	DeadRoot->SetNO3_content(Nitrogen/DeadRoot->GetAmount()*0.5*FractionNminInRoots);
   	DeadRoot->SetorgN_content(Nitrogen/DeadRoot->GetAmount()*(1.0 - FractionNminInRoots));


   }

   Nbudget.AddOutput(Nitrogen.n);                    // Budgets
   N15budget.AddOutput(Nitrogen.n15);
   DMbudget.AddOutput(DryMatterRoot);

	Nitrogen.Clear();
	RootDensAtSurf = 0.0;
	TotalRootLength= 0.0;
	RootDepth      = 0.0;
	DryMatterRoot  = 0.0;
   PlantHeight    = 0.0;

   terminated     = true;
}

/**
 * generated a warning if the plant is alive
 */
void crop::Graze(plantItem * cutPlantMaterial,double cut_height)
{
   // CutOrHarvested = true;
   if (!terminated)
      theMessage[id]->WarningWithDisplay("crop::Graze - this crop can not be grazed, operation ignored!");
}

/**
 * Calculate how much water that evaporates
 */
double crop::GiveEvapFactor()
{
	// New method according to Joergen Olesen, et al. Clim. Res. (2000)
   return 1.0+0.02*(GreenCropAreaIndex+YellowCropAreaIndex);
}

/** * MEL 2010: Effect of CO2 on the minimum and maximum concentrations of Nitrogen
  * MEL 2010: Effect of CO2 on the minimum and maximum concentrations of Nitrogen
 */
double crop::GiveNitrogenResponseCO2()
{
double CO2 = theClimate[id]->GetCO2Concentration();
  return max(1.0,0.6+141/CO2);
}

/**
 * MEL 2010: The minimum concentration of N in the storage organs as affected by CO2
 */
double crop::CorrectedMinN_Store()
{
	double NCO2 = GiveNitrogenResponseCO2();
   double NminVal = NCO2*MinN_Store;
	return NminVal;
}

/**
 * MEL 2010: The minimum concentration of N in the roots as affected by CO2
 */
double crop::CorrectedMinN_Root()
{
	double NCO2 = GiveNitrogenResponseCO2();
   double NminVal = NCO2*MinN_Root;
	return NminVal;
}

/**
 * MEL 2010: The maximum concentration of N in the storage organs as affected by CO2
 */
double crop::CorrectedMaxN_Store()
{
	double NCO2 = GiveNitrogenResponseCO2();
   double NminVal = NCO2*MaxN_Store;
	return NminVal;
}

/**
 * MEL 2010: The maximum concentration of N in the roots as affected by CO2
 */
double crop::CorrectedMaxN_Root()
{
	double NCO2 = GiveNitrogenResponseCO2();
   double NminVal = NCO2*MaxN_Root;
	return NminVal;
}



/**
 * MEL 2007: New calculation of Nmin. Calculate the minimum amount Nitrogen that the crop needs
 */
double crop::Nmin()
{
	 double NminVal = CorrectedMinN_Root()*max(0.0,DryMatterRoot)+CorrectedMinN_Store()*max(0.0,DryMatterStorage)
	   						+NpctMinVegTop()/100*max(0.0,DryMatterVegTop);
	return NminVal;
}

/**
 * MEL 2007:  New calculation of Nmax. Calculate the minimum amount Nitrogen that the crop needs
 */
double crop::Nmax()
{
	double NmaxVal = 0.0;
  	double N = 0.0;

   if (GreenCropAreaIndex<0.0)
   {
      theMessage[id]->WarningWithDisplay("crop::Nmax() - GreenCropAreaIndex negative");
      GreenCropAreaIndex=0.0;
   }
   double NPVTMax=NpctMaxVegTop();
	N = max(0.0,DryMatterVegTop)*NPVTMax/100.0; 					// g N in VegTop/m^2
	NmaxVal = N+CorrectedMaxN_Root()*max(0.0,DryMatterRoot)+CorrectedMaxN_Store()*max(0.0,DryMatterStorage);

		return NmaxVal;
}

/**
 * MEL 2007: Follows the concept of Justes et al. 1994. Annals of Botany 74:397-407.
 */
double crop::NpctMaxVegTop()
{

   double NpctVal;
   if (!(fractionOfSpace<1E-36))
	   NpctVal = NPctMax*pow(1.5/fractionOfSpace,PowDM);
   else
	   NpctVal = NPctMax*pow(1.5,PowDM);
   double NCO2 = GiveNitrogenResponseCO2();
   if (!(fractionOfSpace<1E-36))
   {
	   if (DryMatterVegTop/fractionOfSpace>150)
	   {
		   NpctVal = NPctMax*pow(DryMatterVegTop/(100.0*fractionOfSpace),PowDM);   	//for the green leafs and stem
	   }
   }
	if (YellowCropAreaIndex > 0.0)
   {
		//Corrected MEL 2009
      double RedFac = (ReducedNInYellow*YellowCropAreaIndex+GreenCropAreaIndex)/
                        (YellowCropAreaIndex+GreenCropAreaIndex);
      NpctVal = NpctVal*RedFac;                     									//for all leaf and stem
   }
	 return NpctVal*NCO2;
}

/**
 * MEL 2007: Follows the concept of Justes et al. 1994. Annals of Botany 74:397-407.
 */
double crop::NpctMinVegTop()
{

   double NpctVal;
      if (!(fractionOfSpace<1E-36))
   	   NpctVal = NPctMin*pow(1.5/fractionOfSpace,PowDM);
      else
   	   NpctVal = NPctMin*pow(1.5,PowDM);
      double NCO2 = GiveNitrogenResponseCO2();
      if (!(fractionOfSpace<1E-36))
      {
   	   if (DryMatterVegTop/fractionOfSpace>150)
   	   {
   		   NpctVal = NPctMin*pow(DryMatterVegTop/(100.0*fractionOfSpace),PowDM); //for the green leafs and stem
   	   }
      }
	if (YellowCropAreaIndex > 0.0)
   {
   	//the exsistance of yellow parts reduces the concentration of N in the vegtop
      double RedFac = (ReducedNInYellow*YellowCropAreaIndex+GreenCropAreaIndex)/
                        (YellowCropAreaIndex+GreenCropAreaIndex);
      NpctVal = NpctVal*RedFac;
   }
   return NpctVal*NCO2;
}

/**
 * Calculate how much Nitrogen that are in the top plant that is not planed for usage
 */
nitrogen crop::NitrogenInVegTop()
{
	//nitrogen ninRootVal=NitrogenInRoot();
	//nitrogen ninstorageVal=NitrogenInStorage();
   nitrogen retVal=Nitrogen-NitrogenInRoot()-NitrogenInStorage();

   if (retVal>Nitrogen)
      theMessage[id]->Warning("crop::NitrogenInVegTop - N content in veg. top is above normal range");
   if (retVal.n<-1E-4)
   {
   	theMessage[id]->WarningWithDisplay("crop::NitrogenInVegTop - N content in veg. top is below possible range!!!!!!!!!!!!!!!");
   }
	return retVal;
}

/**
 * Calculates how much N there is is in he root
 */
nitrogen crop::NitrogenInRoot()
{
	double fN = fNitrogen();
	  double NContent = (CorrectedMinN_Root()+fN*(CorrectedMaxN_Root()-CorrectedMinN_Root()))*DryMatterRoot;
   double N15Ratio = Nitrogen.Get15NRatio();
   if (NContent>(0.99999*Nitrogen.n)) // Not the outmost elegance!!!
   {
      if (Nitrogen.n<1E-30 && DryMatterRoot>1E-3)
      {
       //  cout << endl << "Crop object path : " << GetLongName() << endl;
         cout << "N in plant: " << Nitrogen.n << endl;
         cout << "DM in root: " << DryMatterRoot;
         theMessage[id]->FatalError("crop::NitrogenInRoot - no N but still dry matter in root");
      }
      else
      {
       	theMessage[id]->Warning("crop::NitrogenInRoot - N content in root is to high - postadjusted");
         NContent=0.99999*Nitrogen.n;
      }
   }
   nitrogen retVal;
   retVal.SetBoth(NContent,N15Ratio*NContent);
   if (retVal.n>(Nitrogen.n+0.00001))
   {
   	theMessage[id]->WarningWithDisplay("crop::NitrogenInRoot nitrogen in root exceeds nitrogen in plant");
 	retVal = CorrectedMinN_Root()*DryMatterRoot;
   }
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
nitrogen crop::NitrogenInStorage()
{
	double fN = fNitrogen();
	  double NContent = (CorrectedMinN_Store()+fN*(CorrectedMaxN_Store()-CorrectedMinN_Store()))*DryMatterStorage;
   double N15Ratio = Nitrogen.Get15NRatio();
   nitrogen retVal;
   retVal.SetBoth(NContent,N15Ratio*NContent);
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
double crop::InterceptionCapacity()
{
	return InterceptCoeff*(GreenCropAreaIndex+YellowCropAreaIndex);
}

/****************************************************************************\
\****************************************************************************/
double crop::fTW()
{
	return fTW(temp);
}

/****************************************************************************\
\****************************************************************************/
double crop::fTW(double temperature)
{
   if (temperature<=MinDMtemp)
		return 0.0;
   else if (temperature>=MaxDMtemp)
		return 1.0;
   else
		return (temperature-MinDMtemp)/(MaxDMtemp-MinDMtemp);
}

/**
 * returns a factor that describe how much N that are in each part of the plant (root, veg and store)
 */
double crop::fNitrogen()
{
	double N= Nitrogen.n;
	double retVal=1.0;
   if (!Phenology->Ripe())
   {
	   double nMax=Nmax();
	   double nMin=Nmin();
   	if (N>0.0 && nMax>nMin)
   	retVal = max(0.0,min(1.0,(N-nMin)/(nMax-nMin)));

   fNitrogenAfterRipe = retVal;
   }
   else
   	retVal = fNitrogenAfterRipe;
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
double crop::fNitrogenCurve()
{
	double fN =fNitrogen();
   double ScaleFactor = 1.05;  //according to Olesen et al., 2002 MEL 2009
	double Nstress = ScaleFactor*(1.0-exp(NCurveParam*fN));
   return max(0.0,min(1.0,Nstress));
}

/****************************************************************************\
\****************************************************************************/
nitrogen crop::GiveTotalNitrogen()
{
	return Nitrogen;
}

/****************************************************************************\
\****************************************************************************/
double crop::GiveLeafAreaIndex()
{
	return GreenCropAreaIndex;
}

/****************************************************************************\
\****************************************************************************/
double crop::GiveYellowLeafAreaIndex()
{
	return YellowCropAreaIndex;
}

/****************************************************************************\
\****************************************************************************/
double crop::GiveDryMatterStorage()
{
	return DryMatterStorage;
}

/****************************************************************************\
\****************************************************************************/
double crop::GiveDryMatterVegTop()
{
	return DryMatterVegTop;
}

/****************************************************************************\
\****************************************************************************/
double crop::GiveDryMatterRoot()
{
	return DryMatterRoot;
}
double crop::GiveRootDepth()
{
	return RootDepth;
}
/**
 * Input is either DryMatterRoot or NitrogenInRoot
 */
double crop::GiveRootInInterval(double startDep, double thick, double input)
{
	double TopOfLayer=0.0;
   double BottomOfLayer=0.0;
   double Sum=0.0;
	double Total=0.0;
	for (int i=0;i<MaxSoilLayers;i++)
      Total+=RootLengthList[i];
   if (Total==0.0)
      return 0.0;
   else
		for (int i=0;i<MaxSoilLayers;i++)
   	{
      	TopOfLayer=BottomOfLayer;
      	BottomOfLayer=TopOfLayer+0.05;
      	if (doubleGreater(TopOfLayer, startDep,0.0001,true) && doubleLess(BottomOfLayer,(startDep+thick), 0.0001, true)) // Simple case
      	{
         	Sum+=RootLengthList[i];
      	}
      	else
      	{
         	if (doubleLess(TopOfLayer, startDep,0.0001,false)  &&	doubleGreater(BottomOfLayer,(startDep+thick), 0.0001, true)) // Only bottom of layer no i is in interval
         	{
            	double fraction=(BottomOfLayer-startDep)/0.05;
            	Sum+=fraction*RootLengthList[i];
         	}
         	else if (doubleLess(TopOfLayer, startDep,0.0001,true)  && doubleGreater(BottomOfLayer,(startDep+thick), 0.0001, false)) // Only top of layer no i is in interval
            {
               	double fraction=(startDep+thick-TopOfLayer)/0.05;
               	Sum+=fraction*RootLengthList[i];
            }
      	}
   	}
  		return Sum*input/Total;
}


/****************************************************************************\
\****************************************************************************/
double crop::RootPenetrationReduction()
{
	double clay = aSoil->GetSoilProfile()->GetClayContent(250,500);
   double f = min(1.0,max(0.5,0.5+(clay-0.02)*0.5/(0.08-0.02)));
   return f;
}

/****************************************************************************\
\****************************************************************************/
void crop::CalcRootGrowth()
{
	if (Phenology->Sown())
   {
		if (temp>TempRootMin) // && GreenCropAreaIndex>0.2)
			RootDepth += RootPentrRate*RootPenetrationReduction()*(temp-TempRootMin);
		if (RootDepth>MaxRootDepth)
			RootDepth = MaxRootDepth;
		TotalRootLength = DryMatterRoot*SpecificRootLength;
		RootDensAtSurf = CalcRootDensAtSurf(RootDistrParm,RootDepth,RootDensAtBottom);
      RootDensAtBottom = RootDensAtSurf*exp(-RootDistrParm*RootDepth);
      CalcRootDistribution();
	}
}

/****************************************************************************\
\****************************************************************************/
double crop::TargetFunction(double RootDistrParm,double RootDepth,double TotalRootLength,double RootDensAtBottom)
{
	if (RootDistrParm>(1E-19))
		return ((TotalRootLength*RootDistrParm)/(1-exp(-RootDistrParm*RootDepth)))-(RootDensAtBottom/exp(-RootDistrParm*RootDepth));
	else
		return 1E+30;
}

/**
 * returns 0
 */
int crop::RootDistrIsLinear()
{
	return 0;

}

/****************************************************************************\
\****************************************************************************/
double crop::CalcRootDensAtSurf(long double RootDistrParm,double RootDepth,double RootDensAtBottom)
{
	double a = (1-exp(-RootDistrParm*RootDepth))/RootDistrParm+0.5*0.3*RootDepth*exp(-RootDistrParm*RootDepth);
   if (a==0) return RootDensAtBottom;
   double RootDensSurf = TotalRootLength/a;
   return RootDensSurf;
/* Old root growth code -----------------------------------------
	if (RootDepth>0.0)
   {
		if (RootDistrIsLinear())
      {
         if ((TotalRootLength/RootDepth)<=RootDensAtBottom)
            return TotalRootLength/RootDepth;                              // => same density at bottom and surface
			else
            return RootDensAtBottom+2.0*(TotalRootLength-RootDepth*RootDensAtBottom)/RootDepth; // BMP corrected this 05.11.1999
      }
		else
			return RootDensAtBottom/exp(-RootDistrParm*RootDepth);
	}
	else
		return 0.0;
---------------------------------------------------------------*/
}

/**
 * startdepth, thickness in meter
 */
double crop::RootLengthInInterval(double startdepth,double thickness)
{
	if (Phenology->Sown() && RootDepth>0)
   {
      if (RootDistrIsLinear())
      {
			if ((TotalRootLength/RootDepth)<=RootDensAtBottom)
				return TotalRootLength*thickness/RootDepth;
			else
         {
            double depth=startdepth+(thickness/2.0);
				return thickness*(RootDensAtSurf-(RootDensAtSurf-RootDensAtBottom)*depth/RootDepth); // BMP corrected this 05.11.1999
         }
      }

      // Length in exponential curve
      double ExtraRootDepth = 0.3;

      double EndDepth1 = min(RootDepth,startdepth+thickness);
      double EndDepth2 = min((1.0+ExtraRootDepth)*RootDepth,startdepth+thickness);
      double StartDepth2 = max(startdepth,RootDepth);
      double Length1 = 0.0;
      double Length2 = 0.0;

		if (startdepth<RootDepth)
			Length1 = RootDensAtSurf*(exp(-RootDistrParm*startdepth)-exp(-RootDistrParm*EndDepth1))/RootDistrParm+
         			     (EndDepth1-startdepth)*MinimumRootDens;

      // Length in linear declining curve
   	if (startdepth+thickness>RootDepth && startdepth<(1.0+ExtraRootDepth)*RootDepth)
		{
         double DensAtBottom = RootDensAtSurf*(exp(-RootDistrParm*RootDepth));
	      double Dens1 = DensAtBottom*(1.0-(StartDepth2-RootDepth)/(ExtraRootDepth*RootDepth));        // Note assumes that root systems extends 30% further than rootdepth
	      double Dens2 = DensAtBottom*(1.0-(EndDepth2-RootDepth)/(ExtraRootDepth*RootDepth));        // Note assumes that root systems extends 30% further than rootdepth
         Length2 = 0.5*(EndDepth2-StartDepth2)*(Dens1+Dens2);
      }
      return Length1+Length2;
	}
	else
		return 0.0;
}

/**
 * Alters the dry matter production relative to CO2 concentration
 * Unaltered at ppmCO2 = 377
 * CO2 function parameters only estimated for winter wheat
 */
double crop::DeltaDryMatter()
{
	double CO2conc = theClimate[id]->GetCO2Concentration();
	 double CO2PhotEffect = 1.0;
	   if(!C4Photosynthesis)
	      CO2PhotEffect = CO2Effect*exp(0.4537-170.97/CO2conc);
	InterceptedRadiation += Rg*PhotoSyntActPart;

 	double retVal;
   double PAR_Reduction = min(1.0,1.0-0.0445*(Rg*PhotoSyntActPart-5.0));
   retVal=Rg*PhotoSyntActPart*MaxRadUseEff
          *CO2PhotEffect// CO2 effect
          *TranspirationRatio
		    *fNitrogenCurve()
          *fTW()
          *PAR_Reduction;

   return retVal;
}


/****************************************************************************\
\****************************************************************************/
double crop::FractionToRoot()
{
	double ret=MinAllocToRoot;
   if (!Phenology->Emerged())   //MEL 2009 - only cosmetic
   	ret=MaxAllocToRoot;
   if (Phenology->Emerged() && !Phenology->Anthesis())
      ret =  MinAllocToRoot+(MaxAllocToRoot-MinAllocToRoot)*Phenology->GetFractionToAnthesis();
   if (Phenology->Anthesis() && !Phenology->GrainFillEnd())
      ret =  max(0.0,MinAllocToRoot*(Phenology->DS_StartFill - Phenology->DS));
   return ret;
}

/****************************************************************************\
\****************************************************************************/
void crop::TransferDryMatterToRoot(double *dDryMatt)
{

	double newDeltaRoot= max(0.0,*dDryMatt*FractionToRoot());
	RootTranslocation += newDeltaRoot;
	*dDryMatt-=newDeltaRoot;
	DryMatterRoot+=newDeltaRoot;
	RootDecay(newDeltaRoot);

}



/**
 * BPE and MEL 2007: New limitation to the transfer of DM to storage according
 * to the concept of Justes et al. 1994. Annals of Botany 74:397-407.
 */
void crop::TransferDryMatterToStorage(double * dDryMatt)
{
   double Store = min(0.9,StoreForFilling);
   double Fill = min(0.9,FillFactor);
	if (Phenology->Anthesis() && FillFlag==0)
   {
   	if (TransferableStorage<1E-8)
      	TransferableStorage = DryMatterVegTop*Store;
      TransferableStorage += *dDryMatt*Fill;
      TransferedDMToStorage = TransferableStorage;
   }
   // MEL 2009
   if (Phenology->GrainFillStart())
   {
   	FillFlag = 1;
      if (fNitrogen() > 0.0)
      {
      	double transfer = min(DryMatterVegTop,max(0.0,TransferableStorage*Phenology->GetFractionOfGrainFill()));     // linear fill of DM stored during lag phase

         DryMatterVegTop -= transfer;
         DryMatterStorage += transfer+*dDryMatt*Fill*ConversionCoefficient;
      	if (DryMatterVegTop < TransferableStorage)
      		TransferableStorage = 0.9* DryMatterVegTop;

   		double minimumN = Nmin();
   		double maximumN = Nmax();
      	if (minimumN > Nitrogen.n + 0.01)
         {
            double transferBack = max(0.0,min(transfer,(minimumN - Nitrogen.n)/CorrectedMinN_Store()));
         	DryMatterVegTop += transferBack;
         	DryMatterStorage -= transferBack;
         	transfer = transfer - transferBack;
         }
  		if (maximumN < Nitrogen.n - 0.01)
   		{
            double transferBack = max(0.0,min(transfer,(Nitrogen.n - maximumN)/CorrectedMaxN_Store()));
         	DryMatterVegTop += transferBack;
         	DryMatterStorage -= transferBack;
         	transfer = transfer - transferBack;
 	     	}

         DMbudget.AddOutput((*dDryMatt*Fill)*(1-ConversionCoefficient));
         *dDryMatt = *dDryMatt*(1-Fill);
         TransferedDMToStorage -= transfer;
      }
   }
   if (Phenology->GrainFillEnd() && TransferedDMToStorage>0)             // Check to see if all transferable storage is transfered
   {
      double transfer=max(0.0,TransferedDMToStorage);
   	DryMatterStorage += transfer;
      DryMatterVegTop -=  transfer;
      double minimumN = Nmin();
   	double maximumN = Nmax();
      if (minimumN > Nitrogen.n + 0.01)
      {
         double transferBack = max(0.0,min(transfer,(minimumN - Nitrogen.n)/CorrectedMinN_Store()));
      	DryMatterVegTop += transferBack;
        	DryMatterStorage -= transferBack;
         transfer = transfer - transferBack;
      }
   	if (maximumN < Nitrogen.n - 0.01)
   	{
         double transferBack = max(0.0,min(transfer,(Nitrogen.n - maximumN)/CorrectedMaxN_Store()));
       	DryMatterVegTop += transferBack;
        	DryMatterStorage -= transferBack;
         transfer = transfer - transferBack;
      }
      TransferedDMToStorage = 0.0;
   }
}

/****************************************************************************\
\****************************************************************************/
void crop::CalcLeafAreaIndices()
{
   if (GreenCropAreaIndex>maxGLAI)
      maxGLAI=GreenCropAreaIndex;     //state variable that holds the maximum achieved green LAI
   if (Phenology->Emerged())
   {
   	if (Phenology->TempSumForLeaf<=Phenology->LinearLeafPhase)
 //     	GreenCropAreaIndex =  InitialCAI*RelativeDensity*(Phenology->TempSumForLeaf/Phenology->LinearLeafPhase);      /Deleted MEL 2010 due to very large LAI at high sowing rates
      	GreenCropAreaIndex =  InitialCAI*(Phenology->TempSumForLeaf/Phenology->LinearLeafPhase);      // value from Porter ....
      else
      {
    	  double DS_num = 1.3;
    	 if (CropName == "WinterWheat")
    	   DS_num =1.0;
    	 if (Phenology->DS<DS_num)
         {
				double reducedGrowth = Phenology->GetFractionToFlagLigule();                 // Correct later
            reducedGrowth = 1.0/(1.0+pow(reducedGrowth,10));
            double expLeafExt = CoeffGreenIdx*GreenCropAreaIndex*max(0.0,temp);
            double expL = expLeafExt*min(1.0,TranspirationRatio/0.6)*reducedGrowth;

            double dmL  = LAIDMRatio*DryMatterVegTop-GreenCropAreaIndex;

         	double NL   = NitrogenInVegTop().n*LAINitrogenRatio-GreenCropAreaIndex;
            double potDGLAI = max(0.0,MaxGLAICrop-GreenCropAreaIndex);
         	double DGLAI = max(0.0,min(expL,min(dmL,min(NL,potDGLAI))));

            GreenCropAreaIndex += DGLAI;
         }
         if ((Phenology->LeafSenesence()>0) || (temp < 0.0))
         {

            double senescense = 1.0/(1.0-MinimumSenescense)*Phenology->LeafSenesence();  // Correct later
            if (temp < 0.0)  //if mean temp is below zero enhanced senescense MEL 2009
            	senescense=1.0/(1.0-MinimumSenescense)*0.005;
            double deltaCAIg=-max(0.0,senescense*maxGLAI*(1.0-TranspirationRatio*MinimumSenescense));

            if ((YellowCropAreaIndex>=0.0) || (GreenCropAreaIndex>=0.0))
            {
            	GreenCropAreaIndex+=deltaCAIg;
               if (GreenCropAreaIndex<0.0)
               {
               	deltaCAIg-=GreenCropAreaIndex;
                  GreenCropAreaIndex=0.0;
               }
               YellowCropAreaIndex-=deltaCAIg*Conversfactor;
            }
            else
            	theMessage[id]->WarningWithDisplay("crop::CalcLeafAreaIndices - variables in undefined state");
         }
      }
	}
}

/****************************************************************************\
\****************************************************************************/
bool crop::IrrigationDemand()
{
	double MaxWDeficit = 1.0;
   if (Phenology->Emerged() && Phenology->TempSumForLeaf>=Phenology->LinearLeafPhase)
   {
	   if (!Phenology->Anthesis())
	   {
      	if (theTime[id].GetMonth()>=10 || theTime[id].GetMonth()<=3)
         	MaxWDeficit = 1.0;
         else
	      	MaxWDeficit = WaterDeficitVegGrowth;
	   }
	   else
	   {
		   if (!Phenology->GrainFillStart())
			   MaxWDeficit = WaterDeficitLagPhase;
		   else
			   MaxWDeficit = WaterDeficitGrainFill;
	   }
   }
   if (aSoil && (MaxWDeficit<1.0) && RootDepth>0.0)
      {
      	double WaterStatus= (aSoil->GetAvailWater(0,RootDepth*1000.0)-aSoil->GetWiltCapacity(0,RootDepth*1000.0))
                            /(aSoil->GetFieldCapacity(0,RootDepth*1000.0)-aSoil->GetWiltCapacity(0,RootDepth*1000.0));
    //     return (max(WaterStatus,TranspirationRatio)<MaxWDeficit);
         return (WaterStatus<MaxWDeficit);
      }
   else
      return false;
}

/****************************************************************************\
\****************************************************************************/
double * crop::GiveRootLengthList()
{
	return (double *)&RootLengthList;
}

/****************************************************************************\
\****************************************************************************/
double crop::NitrogenDemand()
{
	double NitrogenDem=max(0.0,Nmax()-Nitrogen.n);
//  	double NitrogenDem=0.5*(Nmax()-Nitrogen.n);    //TEST MEL 2009: Changed to 50 %  to avoid problems with uptake of access nitrogen
  	if ((Nmax()-Nitrogen.n<-1e-10)&&(Phenology -> DS > 0.1))
{
  		theMessage[id]->Warning("crop::NitrogenDemand is below zero");
  	}
  	if (NitrogenDem<0.0)
  		NitrogenDem=0.0;
  	if (GreenCropAreaIndex<1E-6 )
 		NitrogenDem=0.0;
  	if(Phenology->GrainFillStart())
 	{
  		double fraction = (Phenology->DS-2.0)*1/(Phenology->DS_StartFill-2.0);
  		NitrogenDem=max(0.0,(fraction)*NitrogenDem);
 	}
    return NitrogenDem;
}

/**
 * Updating root each in each layer of the soil
 */
void crop::CalcRootDistribution()
{
   double startdepth=0.0;
 	for (int i=0;i<NumOfLayers;i++)
   {
      if (LayerThickness[i]<0)
         LayerThickness[i]=aSoil->GetLayerThickness(i)/1000;
      if (LayerThickness[i]>0)
    		RootLengthList[i]=RootLengthInInterval(startdepth,LayerThickness[i]);
      else
         RootLengthList[i]=0.0;

      if (i>0)
      {
         if (((RootLengthList[i-1]/LayerThickness[i-1])-(RootLengthList[i]/LayerThickness[i]))<-1e-10)
            theMessage[id]->FatalError("crop::CalcRootDistribution - root density is increasing with depth");
      }
      if (RootLengthList[i]<0.0)
      {
         if (RootLengthList[i]<(-1E-10))
            theMessage[id]->WarningWithDisplay("crop::CalcRootDistribution - root length can not be negative");
         RootLengthList[i]=0.0;
      }
      startdepth+=LayerThickness[i];
   }
}

/****************************************************************************\
\****************************************************************************/
void crop::GetStatus(double * aRootRadius,
							double * aRootpF,
							double * aNitrogenDemand,
							double * aNitrateUptakeRate,
							double * aAmmoniumUptakeRate,
                     double * aMinimumSoilNitrate,
                     double * aMinimumSoilAmmonium)
{
	if (Phenology->Sown() && !Phenology->Ripe() && aSoil)
   	*aNitrogenDemand=NitrogenDemand();
   else
	   *aNitrogenDemand=0.0;
  	*aRootRadius         = sqrt(1.0/(3.1415926535*0.075*SpecificRootLength));          // assumes that root system is cylindric and dm = 7.5%
  	*aRootpF             = RootpF;
  	*aNitrateUptakeRate  = NitrateUptakeRate;
  	*aAmmoniumUptakeRate = AmmoniumUptakeRate;
   *aMinimumSoilNitrate = MinimumSoilNitrate;
   *aMinimumSoilAmmonium= MinimumSoilAmmonium;
}

/**
 * Adding Nitrogen uptake to the crop and replace Transpiration Ratio
 */
void crop::NitrogenAndTranspiration(nitrogen aNitrogenUptake,double aTranspirationRatio)
{
   TranspirationRatio = aTranspirationRatio;
   if ((TranspirationRatio<0.5) && (YellowCropAreaIndex+GreenCropAreaIndex<0.01))         // corrects problem with transpiration the day of germination
   	TranspirationRatio = 1.0;
   if (aNitrogenUptake.n>1e-15)
   {
      Nitrogen = Nitrogen + aNitrogenUptake;
      if(Nitrogen.n<=0)
      	 theMessage[id]->FatalError("problem");
      AccumulatedNProduction = AccumulatedNProduction + aNitrogenUptake;
      Nbudget.AddInput(aNitrogenUptake.n);              // Budgets
      N15budget.AddInput(aNitrogenUptake.n15);
   }
   //MEL 2009
   double minimumN = Nmin();
   double maximumN = Nmax();

   if ((DryMatterStorage + DryMatterRoot + DryMatterVegTop > 0.0) && (Nitrogen.n > 0.5))
   {
   	if (minimumN > Nitrogen.n * 1.25)
      {
      	cout << GetLongName() << endl;
   		theMessage[id]->WarningWithDisplay("crop::NitrogenAndTranspiration - not enough nitrogen in plant");
   	}
   	if (maximumN < Nitrogen.n * 0.75)
   	{
      	cout << GetLongName() << endl;
	   	theMessage[id]->WarningWithDisplay("crop::NitrogenAndTranspiration - too much nitrogen in plant");
      }
   }

}

/**
 * Returns daily DM growth
 */
double crop::Update(double ActivePar)
{

   Rg 	= ActivePar;
   temp 	= theClimate[id]->tmean;
   double DailyDMGrowth = 0.0;
   DeltaDMTop=0.0;
   if (Phenology->Sown() && !Phenology->Ripe() && !terminated)
   {
      if (totalLAIStand>0.0)
      {
          double maxSpaceFraction=(GreenCropAreaIndex+YellowCropAreaIndex)/totalLAIStand;
          fractionOfSpace=max(0.0,min(maxSpaceFraction,fractionOfSpace+max(0.0,temp)*ColonialisationRate));
      }
	 double soilTemp=aSoil->GetTemperature(200);
      double DayLength = theClimate[id]->PhotoPeriod();
      Phenology->Update(temp,soilTemp,DayLength);      // updates phenology

      if (Phenology->Sown())
			TempSumRoot += max(0.0,temp);

      UpdateHeight();
		double deltaDryMatt = DeltaDryMatter();
      DailyDMGrowth = deltaDryMatt;
      DMbudget.AddInput(deltaDryMatt);              // Budgets
      AccumulatedDMProduction += deltaDryMatt;

      if (Phenology->TempSumForLeaf<=Phenology->LinearLeafPhase)                         // the same termal time as linear leaf growth
      {
         double DMTransfer=2.0*TopFraction*InitialSeedDM*(max(0.0,temp)/Phenology->LinearLeafPhase);
         DMTransfer = min(DMTransfer,SeedDM);
         DryMatterVegTop += 0.5*DMTransfer;
         DryMatterRoot   += 0.5*DMTransfer;
         SeedDM -= DMTransfer;
      }

      TransferDryMatterToRoot(&deltaDryMatt);
      CalcRootGrowth();
      TransferDryMatterToStorage(&deltaDryMatt);
      DryMatterVegTop += deltaDryMatt;
      DeltaDMTop = deltaDryMatt;                              // Used for output
      CalcLeafAreaIndices();
	}

   return DailyDMGrowth;
}

/**
 * Kills roots below a certain depth. Used for grazing and cutting pastures. Dead code since it is never called
 */
void crop::KillRootsBelow(double depth, decomposable *DeadRoot, double* RootList)
{
	DeadRoot->Setname("ROOTMATTER");
   theProducts[id]->GiveProductInformation(DeadRoot);

	double RootBelow = 0;
   double CurrentDepth = 0;
   if (TotalRootLength>0)
   {
      for (int i=0;i<MaxSoilLayers;i++)
      {
         RootList[i] = 0.0;
         if (depth<CurrentDepth+LayerThickness[i])
         {
            RootList[i] = max(1.0,(depth-CurrentDepth)/LayerThickness[i])*RootLengthList[i];
            RootBelow += RootList[i];
         }
         CurrentDepth += LayerThickness[i];
      }

      double DMdecay = DryMatterRoot*RootBelow/TotalRootLength;
      if (DMdecay>0 && DryMatterRoot>0)
      {
         nitrogen NConcentration =  NitrogenInRoot()/DryMatterRoot;
         if (NConcentration.n<CorrectedMinN_Root())
         {
            if (NConcentration.n==0)
            {
            	theMessage[id]->WarningWithDisplay("crop::KillRootsBelow - nitrogen concentration in roots zero");
               NConcentration = CorrectedMinN_Root();
            }
            theMessage[id]->WarningWithDisplay("crop::KillRootsBelow - nitrogen concentration in roots below possible value");
            NConcentration = NConcentration*CorrectedMinN_Root()/NConcentration.n;
         }

         nitrogen Ndecay = NConcentration*DMdecay;
         DryMatterRoot  -= DMdecay;
         Nitrogen 	    = Nitrogen - Ndecay;
         if(Nitrogen.n<=0)
         	 theMessage[id]->FatalError("problem");
         DeadRoot->Setamount(DMdecay/DeadRoot->GetdryMatter());                 	// Conversion from g/m2 to t/ha
         DeadRoot->SetorgN_content(Ndecay/(DMdecay/DeadRoot->GetdryMatter()));

         // update budgets
         Nbudget.AddOutput(Ndecay.n);
         N15budget.AddOutput(Ndecay.n15);
         DMbudget.AddOutput(DMdecay);

         RootDepth = depth;
      }
	}
}

/**
 * Rhizodeposition and root decay
 */
void crop::RootDecay(double RootTranslocation)
{
	if(!Phenology->Ripe())
   {
		nitrogen aNitrogenInRoot =  NitrogenInRoot();
		if (rootMatter)
			rootMatter->Setamount(0.0);

		double SoilTemp=aSoil->GetTemperature(200);

		if (DryMatterRoot>0 && temp>0 && Phenology->Emerged())
		{
     		/*if (Phenology->Anthesis())  // MEL 2007 Removed due to dying rootsystem
			 DecayRoot = 0.05;*/
			double DMdecay = DryMatterRoot*TemperatureEffect(SoilTemp)*DecayRoot+
      					  RootTranslocation*RhizoDepositFraction;
			DMdecay = min(DryMatterRoot,DMdecay);

			if (DMdecay>0)
			{
				nitrogen NConcentration =  aNitrogenInRoot/DryMatterRoot;
				if (doubleLess(NConcentration.n,CorrectedMinN_Root(),0.000001))
				{
         			if (NConcentration.n==0)
         			{
         				theMessage[id]->WarningWithDisplay("crop::RootDecay - nitrogen concentration in roots zero");
         				NConcentration = CorrectedMinN_Root();
         			}
					theMessage[id]->WarningWithDisplay("crop::RootDecay - nitrogen concentration in roots below possible");
					NConcentration = NConcentration*CorrectedMinN_Root()/NConcentration.n;
				}

				nitrogen Ndecay = NConcentration*DMdecay;
				DryMatterRoot  -= DMdecay;
				Nitrogen 	    = Nitrogen - Ndecay;
				if(Nitrogen.n<=0)
					 theMessage[id]->FatalError("problem");
				rootMatter->Setamount(DMdecay/rootMatter->GetdryMatter());
				rootMatter->SetorgN_content(Ndecay/(DMdecay/rootMatter->GetdryMatter()));

				// update budgets
				Nbudget.AddOutput(Ndecay.n);
				N15budget.AddOutput(Ndecay.n15);
				DMbudget.AddOutput(DMdecay);
				AccumulatedRootDeposit  += DMdecay;
				AccumulatedRootNDeposit = AccumulatedRootNDeposit + Ndecay;

			}
		}
   }
}

/**
 * Output crop variables to file
 */
void crop::StreamKeyData(fstream& os)
{


	double TSum = 0.0;
   double DS = 0.0;
   if (Phenology)
   {
   	DS = Phenology->DS;
      TSum = Phenology->TempSumForLeaf;

   }

   os << "\t" << CropName
      << "\t" << DS
      << "\t" << TSum
     << "\t" << theClimate[id]->GetCO2Concentration()
      << "\t" << GreenCropAreaIndex
      << "\t" << YellowCropAreaIndex
      << "\t" << DryMatterRoot;
  os    << "\t" << GiveRootInInterval(0.0,0.3, DryMatterRoot);
   if ((CropName == "Beet") || (CropName == "Potato") || (CropName == "OilRadish"))
   	os	<< "\t" << GiveDryMatterStorage()+ GiveDryMatterRoot();
   else
   	os << "\t" << GiveDryMatterRoot();
   os << "\t" << GiveDryMatterVegTop()
      << "\t" << DryMatterStorage;
   if ((CropName == "Beet") || (CropName == "Potato") || (CropName == "OilRadish"))  // Dry matter of total top
   	os	<< "\t" << GiveDryMatterVegTop();
   else
   	os << "\t" << DryMatterStorage+GiveDryMatterVegTop();

      os << "\t" << TranspirationRatio
     << "\t" << IrrigationDemand()
      << "\t" << PlantHeight
      << "\t" << RootDepth
      << "\t" << TotalRootLength
      << "\t" << InterceptedRadiation
      << "\t" << Nitrogen.n
      << "\t" << NitrogenInVegTop().n15
      << "\t" << Nmin()
      << "\t" << Nmax()
      << "\t" << NitrogenInStorage().n;
if ((CropName == "Beet") || (CropName == "Potato") || (CropName == "OilRadish"))     //Nitrogen in total top
   	os	<< "\t" << NitrogenInVegTop().n;
   else
   	os << "\t" << (NitrogenInVegTop()+NitrogenInStorage()).n;

      os << "\t" << NitrogenInVegTop().n
      << "\t" << NitrogenInRoot().n
      << "\t" << GiveRootInInterval(0.0,0.3, NitrogenInRoot().n);
   if ((CropName == "Beet") || (CropName == "Potato") || (CropName == "OilRadish"))      // Nitrogen in roots and tubers
   	os << "\t" << NitrogenInStorage().n+ NitrogenInRoot().n;
   else
      os << "\t" << NitrogenInRoot().n;
   if (Nitrogen.n>0)
         os << "\t" << fNitrogen()
            << "\t" << fNitrogenCurve();
   else
   	os << "\t1\t1";
   os << "\t" << AccumulatedRootDeposit
      << "\t" << RootTranslocation
      << "\t" << AccumulatedRootNDeposit.n
      << "\t" << AccumulatedTopDeposit
   	<< "\t" << AccumulatedTopNDeposit.n
      << "\t" << AccumulatedDMProduction
   	<< "\t" << AccumulatedNProduction.n
      << "\t" << GetNFixationThisDay()
   	<< "\t" << AccumulatedNFixation
      << "\t" << DeltaDMTop
      << "\t" << NitrogenInRoot().Get15NRatio()
      << "\t" << AccumulatedRootRespiration;

  		for(unsigned int i=0;i<globalSettings[id]->RootData.size();i++)
 		{
 			double StartDepth = globalSettings[id]->RootData[i].getStartDepth()/1000;
 			double Thickness = globalSettings[id]->RootData[i].getEndDepth()/1000-StartDepth;
         os << "\t" << RootLengthInInterval(StartDepth, Thickness)
	 		 	<< "\t" << GiveRootInInterval(StartDepth, Thickness, DryMatterRoot)
         	<< "\t" << GiveRootInInterval(StartDepth, Thickness, NitrogenInRoot().n);
      }



}
/****************************************************************************\
\****************************************************************************/
void crop::StartBudget()
{
	Nbudget.SetInput(Nitrogen.n);
	N15budget.SetInput(Nitrogen.n15);
  	DMbudget.SetInput(DryMatterVegTop+DryMatterRoot+DryMatterStorage);
  	Nbudget.SetOutput(0);
  	N15budget.SetOutput(0);
  	DMbudget.SetOutput(0);
}

/**
 * see if 3 budges classes wholes the right values
 * Nrmeain and DMRemain wholes the values of what is remaing
 */
bool crop::EndBudget(double * NRemain, double * DMRemain)
{
	bool retVal=true;
  	*NRemain=Nitrogen.n;
  	*DMRemain=GiveDryMatterVegTop()+DryMatterRoot+DryMatterStorage+SeedDM;
   if (!Nbudget.Balance(*NRemain))
   	retVal=false;
   if (!DMbudget.Balance(*DMRemain))
   {
   	retVal=false;
      cout << "Dry matter " << GiveDryMatterVegTop() << " " << DryMatterRoot
           << " " << DryMatterStorage << " " << SeedDM << endl; // !!!!!!
      cout << "";
   }
   double N15Remain = Nitrogen.n15;
   if (!N15budget.Balance(N15Remain))
   	retVal=false;
   return retVal;
}

/**
 * This function should not be called
 */
void crop::Cut(plantItem * cutPlantMaterial, double CutHeight)
{
   cout << GetLongName();
   theMessage[id]->FatalError("crop::Cut - this function should never be called, use 'Harvest' instead");
}

/**
 * Get really rough estimate of potential growth. Assumes complete interception of radiation
 * Used in grassland management
 */
double crop::PotentialDryMatter(double radiation, double temperature)
{
   double CO2conc = theClimate[id]->GetCO2Concentration();
 	double retVal, CO2PhotEffect=1.0;

   if(!C4Photosynthesis)
      CO2PhotEffect = CO2Effect*exp(0.4537-170.97/CO2conc);

   retVal=radiation*PhotoSyntActPart*MaxRadUseEff
          *CO2PhotEffect
          *fTW(temperature);

   return retVal;
}

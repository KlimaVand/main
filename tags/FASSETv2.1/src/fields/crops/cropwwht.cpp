/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "cropwwht.h"
#include <climclas.h>
#include <output.h>
#include <bstime.h>

/****************************************************************************\
\****************************************************************************/
cropWwheat::cropWwheat(const char * aName, const int aIndex, const base * aOwner)
           :crop(aName,aIndex,aOwner)
{
   CropName   = "WinterWheat";
   PlantItemName = "WNWHT";                   // These names maps to products.dat !
   StrawItemName = "WHEATSTRAW";              // These names maps to products.dat !

	WinterSeed = true;
   //---- Parameters ---------------------------------------------
	MaxRootDepthCrop = 1.5;             // new estimate from KTK (18_3_3)

	// Water deficits from MARKVAND
   WaterDeficitVegGrowth = 0.65;
   WaterDeficitLagPhase  = 0.45;
   WaterDeficitGrainFill = 0.60;

	// Drymatter production
	MinDMtemp=2.0; //4.0;             // From DAISY
	MaxDMtemp=10.0;                   // Maximum temperature for dry matter production
	MinAllocToRoot=0.1;               // Minimal fraction of dry matter production that is allocated to the root. Approx. 0.13 when using rhizodepositmodel
	MaxAllocToRoot=0.6;               // Maximal fraction of dry matter production that is allocated to the root
   StoreForFilling=0.39;             // Fraction of DM present at initiation of grain filling that is translocated to grain
   FillFactor=0.57;                  // Fraction of net production after anthesis that goes into grain
   MaxPlantHeight=1.0;               // Max plant height (Value from DAISY)
	Conversfactor=0.4;                // Part of green leaf that is converted to yellow leaf by wilting
                                     // After Aslyng & Hansen, 1982. Water balance and crop prodution simulation
                                     // The Royal Vetenary and Agricultural University, Copenhagen
   TopFraction=0.5;                  // Fraction of seed that goes into top

   // Phenology
   // Winter wheat optimised 2001
   // According to Olesen et al. 2002
   Phenology->TS0          = 125;              // Sowing to emergence
   Phenology->TS1          = 308+153;          // Emergence to anthesis
   Phenology->TS2          = 420;              // Anthesis to end of grainfilling
   Phenology->TS3          = 155;              // End of grainfilling to ripeness
   Phenology->TB0          = 0;                // Corresponds to TS0
   Phenology->TB1          = 4;                // Corresponds to TS1
   Phenology->TB2          = 6;                // Corresponds to TS2
   Phenology->TB3          = 0;                // Corresponds to TS3   (From Markvand)
   Phenology->DS_Flagligule= 308.0/Phenology->TS1;
   Phenology->DS_StartFill = 1.0+60.0/Phenology->TS2;

//	TempSumR2=700;                    // See p. 160 in DAISY-description

   // LAI and canopy structure
	CoeffGreenIdx = 0.0092;           // Coefficient for leaf growth
   LAINitrogenRatio = 0.4;           // Maximum ratio between LAI and nitrogen in veg top
   LAIDMRatio = 0.011;               // Maximum ratio between LAI and DM in veg top
   InitialCAI = 0.46;                // Initial green leaf area index after 200 degree days
   HeightA = 0.61;                   // See Olesen, 2002 for description
   HeightB = 0.65;

   //---- Parameters fittet on 3 N levels, 4 years (Foulum)-------
   MinimumSenescense=0.35;           // Used to calculate extra senescense caused by dryness
	MinAllocToRoot=0.1;
   MaxAllocToRoot=0.5;
   RhizoDepositFraction=0.5;
	NitrateUptakeRate  	= 0.6*0.00006; // Calibration based on data from Landforsøgene
   LAIDMRatio = 0.015;                 // Maximum ratio between LAI and DM in veg top
   MaxRadUseEff = 4.4;

   ReducedNInYellow  = 0.39;          //The min and max N% for yellow VegTop is reduced with this parameter compared to green. From CENTS(plowed, foulum)

#ifdef MELCAL
   //---- Parameters fittet on 3 N levels, 4 years (Foulum)-------
   MinN_Store    = 0.02;            // Minimum content of nitrogen i storage organs
	MaxN_Store    = 0.03;               // Maximum content of nitrogen i storage organs
   MaxRadUseEff = 5.2;
   Phenology->TS1          = 250;          // Emergence to anthesis
   Phenology->TS2          = 600;              // Anthesis to end of grainfilling
	MinAllocToRoot=0.15;
	MinN_Root     = 0.005;           	 // Minimum content of nitrogen in root organs
	MaxN_Root     = 0.04;           	 // Maximum content of nitrogen in root organs
   LAINitrogenRatio = 1.1;           // Maximum ratio between LAI and nitrogen in veg top
   RhizoDepositFraction=0.2;
   Conversfactor     = 0.3;             // Part of green leaf that is converted to yellow leaf by wilting
   ReducedNInYellow  = 0.5;          //The min and max N% for yellow VegTop is reduced with this parameter compared to green. From CENTS(plowed, foulum)
#endif
}

/****************************************************************************\
\****************************************************************************/
cropWwheat::~cropWwheat()
{
}

/****************************************************************************\
\****************************************************************************/
cropWwheat::cropWwheat(const cropWwheat& acrop)
  : crop( acrop )
{
   TransferableStorage = acrop.TransferableStorage;
   StoreForFilling = acrop.StoreForFilling;
}

/****************************************************************************\
Note that boolean state variables of course not are interpolated
\****************************************************************************/
void cropWwheat::Add(crop* aCrop, double fraction)
{
   crop::Add(aCrop,fraction);
}

/****************************************************************************\
\****************************************************************************/
void cropWwheat::ReadParameters(char* filename)
{
   crop::ReadParameters(filename);
   UnsetCritical();
   if (OpenInputFile(filename))
   {
	   if (FindSection(CropName) || FindSection("Crop"))
      {
			GetParameter("HeightA",&HeightA);
			GetParameter("HeightB",&HeightB);
      }
   }
	CloseInputFile();
}

/****************************************************************************\
\****************************************************************************/
void cropWwheat::Sow(double SeedDensDryMatt,double aRelativeDensity,double NitrogenInSeed)
{
   crop::Sow(SeedDensDryMatt,aRelativeDensity,NitrogenInSeed);
   Phenology->Sow();
}

/****************************************************************************\
\****************************************************************************/
void cropWwheat::Harvest(decomposable * Storage,decomposable* Straw)
{
   crop::Harvest(Storage,Straw);
}

/****************************************************************************\
\****************************************************************************/
double cropWwheat::fNitrogen()
{
   if (Phenology->Ripe())
   	return fNitrogenAfterRipe; // ensures that no N translocation takes place after ripeness
   else
      return crop::fNitrogen();
}
/****************************************************************************\
\****************************************************************************/
void cropWwheat::UpdateHeight()
{
   double DS_Converted = Phenology->DS/Phenology->DS_Flagligule;
   MaxPlantHeight = 1.0/(1/HeightA-log(2)/(HeightB*2));
   if (Phenology->Emerged())
   {
		PlantHeight = 1.0/(1/HeightA-log(DS_Converted)/(HeightB*DS_Converted));
   }
   PlantHeight = min(PlantHeight,MaxPlantHeight);
}


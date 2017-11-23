/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include <cropsbrl.h>
#include <bstime.h>

cropSbarley::cropSbarley(const char * aName, const int aIndex, const base * aOwner)
            :crop(aName,aIndex,aOwner)
{
	// For nomenclature see base class 'crop'.
	CropName = "SpringBarley";
   PlantItemName = "BARLEY";                 // These names maps to products.dat !
   StrawItemName = "BARLEYSTRAW";           // These names maps to products.dat !
	WinterSeed = false;
   // Parameters
 	k = 0.65;                         //  Estimated by from ecological trial at Foulum (1999)
	MaxRootDepthCrop=1.0;             // Maximal root depth for this crop
												 // Max water deficits:
   WaterDeficitVegGrowth = 0.50;
   WaterDeficitLagPhase  = 0.50;
   WaterDeficitGrainFill = 0.60;

	MinDMtemp  = 2.0;                    // Minimum temperature for dry matter production (4 in Hansen et al)
	MaxDMtemp  = 10;                     // Maximum temperature for dry matter production

	MinN_Store = 0.012;                 // Minimum content of nitrogen i storage organs (Elly's data)
	MaxN_Store = 0.027;                 // Maximum content of nitrogen i storage organs
   NPctMax    = 7.5;              	 	// Following Justes concept

   // LAI and canopystructure

   MaxPlantHeight     = 1.0;         // Max plant height
   LAIDistributionPar = 0.1;         // Estimated by JB from ecological trial at Foulum

												 // Temperature sums:
   // Phenology
   // Spring barley optimised 2002/07 (BMP)
   // Reestimation of base temperatures not possible, especially for spring crops highly confounded with T sums
	Phenology->TS0          = 130;              // Sowing to emergence
   Phenology->TS1          = 401;              // Emergence to anthesis adjusted proportional from MARKVAND (445/(449+113)*(313+193))
   Phenology->TS2          = 310;              // Anthesis to end of grainfilling
   Phenology->TS3          = 175;              // End of grainfilling to ripeness
   Phenology->TB0          = 0;                // Corresponds to TS0
   Phenology->TB1          = 4;                // Corresponds to TS1
   Phenology->TB2          = 6;                // Corresponds to TS2
   Phenology->TB3          = 0;                // Corresponds to TS3
   Phenology->DS_Flagligule= 1.0; //0.60;             // Elongation = 1.36
   Phenology->DS_StartFill = 1.20;

//	TempSumR2=800;                    //See p. 160 in DAISY-description

	FillFactor=0.5;                   // Fraction of net production after anthesis that goes into grain
   Conversfactor=0.2;              // Part of green leaf that is converted to yellow leaf by wilting
   LAINitrogenRatio = 0.6; // 0.4//0.75; //0.45;        // Maximum ratio between LAI and nitrogen in veg top
   MinAllocToRoot=0.1;
   MaxAllocToRoot=0.5;
   RhizoDepositFraction=0.5;
	NitrateUptakeRate  	= 0.000027; // Calibration based on data from Landforsøgene
	FillFactor  	      = 0.57; // Calibration based on data from Landforsøgene
   LAIDMRatio = 0.015;             // Maximum ratio between LAI and DM in veg top
   MaxRadUseEff = 4.4;

   ReducedNInYellow  = 0.55;          //The min and max N% for yellow VegTop is reduced with this parameter compared to green. From CENTS(plowed, foulum)

#ifdef MELCAL
   Phenology->TS1          = 250;              // Emergence to anthesis
   Phenology->TS2          = 600;              // Anthesis to end of grainfilling
   CoeffGreenIdx           = 0.015;          // Coefficient for leaf growth
	FillFactor              = 0.7;                   // Fraction of net production after anthesis that goes into grain
	MinN_Store = 0.02;                 // Minimum content of nitrogen i storage organs (Elly's data)
	MaxN_Store = 0.03;                 // Maximum content of nitrogen i storage organs

   //for IMPACTS
   MaxRadUseEff = 5.0;
   MinAllocToRoot=0.25;

  //---- Parameters fittet on 3 N levels, 4 years (Foulum)-------
   MinN_Store    = 0.02;            // Minimum content of nitrogen i storage organs
	MaxN_Store    = 0.03;               // Maximum content of nitrogen i storage organs
   MaxRadUseEff = 5.2;
   Phenology->TS1          = 250;          // Emergence to anthesis
   Phenology->TS2          = 600;              // Anthesis to end of grainfilling
	MinAllocToRoot=0.15;
	MinN_Root     = 0.005;           	 // Minimum content of nitrogen in root organs
	MaxN_Root     = 0.03;           	 // Maximum content of nitrogen in root organs
   LAINitrogenRatio = 1.1;           // Maximum ratio between LAI and nitrogen in veg top
   RhizoDepositFraction=0.1;
   Conversfactor     = 0.3;             // Part of green leaf that is converted to yellow leaf by wilting
   ReducedNInYellow  = 0.3;          //The min and max N% for yellow VegTop is reduced with this parameter compared to green. From CENTS(plowed, foulum)
   LAIDMRatio = 0.01;             // Maximum ratio between LAI and DM in veg top
   StoreForFilling		 = 0.6;        // Fraction of DM present at initiation of grain filling that is translocated to grain
	NitrateUptakeRate  	= 0.00006; // Calibration based on data from Landforsøgene
#endif
}



/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropSbarley.h"
#include "../../base/bstime.h"

cropSbarley::cropSbarley(const char * aName, const int aIndex, const base * aOwner)
            :crop(aName,aIndex,aOwner)
{
	// For nomenclature see base class 'crop'.
	CropName = "SpringBarley";
   PlantItemName = "BARLEY";                 // These names maps to products.dat !
   StrawItemName = "BARLEYSTRAW";           // These names maps to products.dat !
	WinterSeed = false;

   // Parameters     (new 2010 MEL)
 	k 								= 0.65;             //  Estimated by from ecological trial at Foulum (1999)
	MaxRootDepthCrop			= 1.0;             // Maximal root depth for this crop

	 // Max water deficits:
   WaterDeficitVegGrowth 	= 0.50;
   WaterDeficitLagPhase  	= 0.50;
   WaterDeficitGrainFill 	= 0.60;

   NCurveParam					= -1000;               // Calibration based on data from Landforsøgene

	MaxRadUseEff 				= 3.5;                    // (Fittet on 3 N levels, 4 years (Foulum)- MEL)
	MinDMtemp  					= 2.0;                    // Minimum temperature for dry matter production (4 in Hansen et al)
	MaxDMtemp  					= 10;                     // Maximum temperature for dry matter production

	MinN_Store 					= 0.016;                 // Minimum content of nitrogen i storage organs (80% of value in Fodermiddeltabellen)
	MaxN_Store 					= 0.021;                 // Maximum content of nitrogen i storage organs  (120% of value in Fodermiddeltabellen)
   MinN_Root     				= 0.013;           	 // Minimum content of nitrogen in root organs (Fittet on 3 N levels, 4 years (Foulum)- MEL)
	MaxN_Root     				= 0.02;           	 // Maximum content of nitrogen in root organs   (Fittet on 3 N levels, 4 years (Foulum)- MEL)

   // LAI and canopystructure
   CoeffGreenIdx           = 0.015;          // Coefficient for leaf growth (Fittet on 3 N levels, 4 years (Foulum)- MEL)
   LAIDMRatio 					= 0.01;             // Maximum ratio between LAI and DM in veg top   (Fittet on 3 N levels, 4 years (Foulum)- MEL)
   LAINitrogenRatio 			= 0.6;        // Maximum ratio between LAI and nitrogen in veg top
   Conversfactor     		= 0.2;             // Part of green leaf that is converted to yellow leaf by wilting   (Fittet on 3 N levels, 4 years (Foulum)- MEL)
   MaxPlantHeight     		= 1.0;         // Max plant height
   LAIDistributionPar 		= 0.4;         // Estimated

												 // Temperature sums:
   // Phenology
	Phenology->TS0          = 130;              // Sowing to emergence
   Phenology->TS1          = 250;              // Emergence to anthesis
   Phenology->TS2          = 500;              // Anthesis to end of grainfilling
   Phenology->TS3          = 175;              // End of grainfilling to ripeness
   Phenology->TB0          = 0;                // Corresponds to TS0
   Phenology->TB1          = 4;                // Corresponds to TS1
   Phenology->TB2          = 6;                // Corresponds to TS2
   Phenology->TB3          = 0;                // Corresponds to TS3
   Phenology->DS_Flagligule= 1.0;            	// Elongation = 1.36
   Phenology->DS_StartFill = 1.20;

   FillFactor					= 0.45;                  // Fraction of net production after anthesis that goes into grain
	StoreForFilling			= 0.3;
	MinAllocToRoot				= 0.25;
   MaxAllocToRoot				= 0.6;
   RhizoDepositFraction		= 0.2;             // (Fittet on 3 N levels, 4 years (Foulum)- MEL)

	NitrateUptakeRate  		= 0.00006;		 // Calibration based on data from Landforsøgene
   ReducedNInYellow  		= 0.3;          //The min and max N% for yellow VegTop is reduced with this parameter compared to green. (Fittet on 3 N levels, 4 years (Foulum)- MEL)
}



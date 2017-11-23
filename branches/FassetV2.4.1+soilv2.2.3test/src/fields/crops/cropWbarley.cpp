/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropWbarley.h"

cropWbarley::cropWbarley(const char * aName, const int aIndex, const base * aOwner)
            :cropSbarley(aName,aIndex,aOwner)
{
   CropName = "WinterBarley";
   PlantItemName = "WNBRL";                 // These names maps to products.dat !
   StrawItemName = "BARLEYSTRAW";           // These names maps to products.dat !
   WinterSeed = true;
   //---- Parameters (largely based on winter wheat and spring barley - no calibration performed)
   WaterDeficitVegGrowth 	= 0.60;
   WaterDeficitLagPhase  	= 0.50;
   WaterDeficitGrainFill 	= 0.60;

	MaxRootDepthCrop      	= 1.5;              // Maximal root depth for this crop
	NitrateUptakeRate			= 0.00006;

   MaxRadUseEff 				= 4.0;
	MinDMtemp					= 2.0;              // Minimum temperature for dry matter production (4 in Hansen et al)
	MaxDMtemp					= 10;               // Maximum temperature for dry matter production

	MinN_Store					= 0.016;            // Minimum content of nitrogen i storage organs (Elly's data)
	MaxN_Store					= 0.021;            // Maximum content of nitrogen i storage organs
   NPctMax       				= 7.0;                  // Following Justes concept
	NCurveParam					= -1000;

   // LAI and canopystructure
   MaxPlantHeight 			= 0.9;              // Max plant height
   CoeffGreenIdx     		= 0.02;          // Coefficient for leaf growth
   LAIDistributionPar 		= 0.1;         	  // Estimated by JB from ecological trial at Foulum
	LAIDMRatio					= 0.017;
	LAINitrogenRatio			= 0.7;

   // Phenology
   // Spring barley optimised 2002/07 (BMP)
   // Reestimation of base temperatures not possible, especially for spring crops highly confounded with T sums
	Phenology->TS0          = 110;              // Sowing to emergence  (from winter wheat)
   Phenology->TS1          = 346;              // Emergence to anthesis (estimate 2002/07)
   Phenology->TS2          = 400;              // Anthesis to end of grainfilling (estimate 2002/07)
   Phenology->TS3          = 157;              // End of grainfilling to ripeness
   Phenology->TB0          = 0;                // Corresponds to TS0
   Phenology->TB1          = 4;                // Corresponds to TS1
   Phenology->TB2          = 6;                // Corresponds to TS2
   Phenology->TB3          = 0;                // Corresponds to TS3
   Phenology->DS_Flagligule= 0.67;             // Taken from winter wheat
   Phenology->DS_StartFill = 1.13;             // Estimated 2002/07

   MinAllocToRoot				= 0.25;
   MaxAllocToRoot          = 0.5;
   RhizoDepositFraction		= 0.2;
	StoreForFilling			= 0.3;
}


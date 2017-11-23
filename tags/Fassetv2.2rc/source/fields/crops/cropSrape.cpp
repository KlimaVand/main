/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropSrape.h"

cropSrape::cropSrape(const char * aName, const int aIndex, const base * aOwner)
          :crop(aName,aIndex,aOwner)
{
	// Parameters (Estimated from winter rape)
   CropName   = "SpringRape";
   PlantItemName = "SPRAPE";
   StrawItemName = "RAPESTRAW";
   WinterSeed = false;

  	WaterDeficitVegGrowth = 0.65;
   WaterDeficitLagPhase  = 0.50;
   WaterDeficitGrainFill = 0.65;

   // Root
   MaxRootDepthCrop = 1.8;            // Value from DAISY (spring rape)
	RootPentrRate    = 0.0025;         // new estimate from KTK (18_3_3)
	RootDistrParm        	= 1.0;          // Root density distribution parameter

   // DM production and translocation
   MaxRadUseEff=5.0;                 // Maximum radiation use efficiency (g/MJ)
	ConversionCoefficient=0.5;        // Coefficent of DM conversion from vegetative to storage. Low because of high energy content in oily seed
   GerminationConversion=1.2;        // Conversion factor seed DM -> plant DM
   StoreForFilling=0.50;             // Fraction of DM present at initiation of grain filling that is translocated to grain
   MinAllocToRoot=0.25;               // Minimal fraction of dry matter production that is allocated to the root (Value from DAISY (spring rape))
	MaxAllocToRoot=0.6;                // Maximal fraction of dry matter production that is allocated to the root (Value from DAISY (spring rape))
   MinDMtemp=2.0; //0.0;                    // Minimum temperature for dry matter production
	MaxDMtemp=8.0;                    // Maximum temperature for dry matter production

   // N
   PowDM      = -0.25;                // Power of dry matter in nitrogen status calculation
   NPctMax    = 6.0;                  // Following Justes concept adjusted to max as: 8.3*4.48/5.35
	MinN_Store = 0.025;                // Minimum content of nitrogen i storage organs (Elly's data)
	MaxN_Store = 0.037;                // Maximum content of nitrogen i storage organs

   // LAI and canopy structure
   k                = 0.62;           // Value taken from beet
	MaxPlantHeight   = 1.6;            // Max plant height (Value from DAISY)
   Conversfactor    = 0.4;
	CoeffGreenIdx    = 0.01;          // Coefficient for leaf growth (Optimized)
   LAINitrogenRatio = 0.5;           // Maximum ratio between LAI and nitrogen in veg top (Estimated from graphs)
   LAIDMRatio       = 0.01;         // Maximum ratio between LAI and DM in veg top (Estimated from graphs)
   InitialCAI       = 0.5;            // Guestimate of initial green leaf area index after 200 degree days

	// Phenology
   Phenology->TS0          = 125;              // Sowing to emergence  (guestimate)
   Phenology->TS1          = 480;              // Emergence to anthesis Optimised 2002/07 OBS - from beginning of flowering !!! (JB modified by 125 to match descrease in leaf area)
   Phenology->TS2          = 435;              // Anthesis to end of grainfilling Optimised 2002/07 OBS - to beginning of flowering !!! (JB modified by 125*0.55 due to different base temperatures)
   Phenology->TS3          = 248;              // End of grainfilling to ripeness From MARKVAND (Sf4)
   Phenology->TB0          = 0;                // Corresponds to TS0
   Phenology->TB1          = 0;                // Corresponds to TS1
   Phenology->TB2          = 6;                // Corresponds to TS2
   Phenology->TB3          = 0;                // Corresponds to TS3
   Phenology->DS_Flagligule= 10000;            // Undefined!
   Phenology->DS_StartFill = 1.20;
   Phenology->DB           = 8.0;              // From winter rape

   ReducedNInYellow  = 0.28;          //The min and max N% for yellow VegTop is reduced with this parameter compared to green. From CENTS(plowed, foulum)
}


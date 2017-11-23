/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "cropwr.h"

cropWrape::cropWrape(const char * aName, const int aIndex, const base * aOwner)
          :crop(aName,aIndex,aOwner)
{
	// Parameters
   CropName   = "WinterRape";
   PlantItemName = "WNRPE";                   // These names maps to products.dat !
   StrawItemName = "RAPESTRAW";              // These names maps to products.dat !
	WinterSeed = true;

   // Root
   MaxRootDepthCrop = 2.5;            // new estimate from KTK (18_3_3)
	RootPentrRate    = 0.0025;         // new estimate from KTK (18_3_3)


   // DM production and translocation
   MaxRadUseEff=2.9;                 // Maximum radiation use efficiency (g/MJ)
	ConversionCoefficient=0.5;        // Coefficent of DM conversion from vegetative to storage. Low because of high energy content in oily seed
   GerminationConversion=1.2;        // Conversion factor seed DM -> plant DM
   StoreForFilling = 0.20;           // Guestimate - Fraction of DM present at initiation of grain filling that is translocated to grain
	FillFactor = 0.50;                // Guestimate - Fraction of net production after anthesis that goes into grain

	MinAllocToRoot=0.1; //0.15;              // Minimal fraction of dry matter production that is allocated to the root (Value from DAISY (spring rape))
	MaxAllocToRoot=0.5; //0.6;               // Maximal fraction of dry matter production that is allocated to the root (Value from DAISY (spring rape))
   MinDMtemp=2.0; //0.0;                    // Minimum temperature for dry matter production
	MaxDMtemp=8.0;                    // Maximum temperature for dry matter production

   // N
   PowDM      = -0.25;                // Power of dry matter in nitrogen status calculation
   NPctMax    = 7.0;                  // Following Justes concept adjusted to max as: 8.3*4.48/5.35
	MinN_Store = 0.025;                // Minimum content of nitrogen i storage organs (Elly's data)
	MaxN_Store = 0.037;                // Maximum content of nitrogen i storage organs
	//MinN_Straw = 0.003;                // Minimum content of nitrogen i storage organs (Elly's data)
	//MaxN_Straw = 0.010;                // Maximum content of nitrogen i storage organs


   // LAI and canopy structure
   k                = 0.62;           // Value taken from beet
	MaxPlantHeight   = 1.6;            // Max plant height (Value from DAISY)
   Conversfactor    = 0.4;
	CoeffGreenIdx    = 0.025;          // Coefficient for leaf growth (Optimized)
   LAINitrogenRatio = 0.28;           // Maximum ratio between LAI and nitrogen in veg top (Estimated from graphs)
   LAIDMRatio       = 0.0086;         // Maximum ratio between LAI and DM in veg top (Estimated from graphs)
   InitialCAI       = 0.5;            // Guestimate of initial green leaf area index after 200 degree days

	// Phenology
	// Spring barley optimised 2002/07 (BMP)
   // Reestimation of base temperatures
   Phenology->TS0          = 125;              // Sowing to emergence
   Phenology->TS1          = 540+125;          // Emergence to anthesis Optimised 2002/07 OBS - from beginning of flowering !!! (JB modified by 125 to match descrease in leaf area)
   Phenology->TS2          = 400-0.55*125;     // Anthesis to end of grainfilling Optimised 2002/07 OBS - to beginning of flowering !!! (JB modified by 125*0.55 due to different base temperatures)
   Phenology->TS3          = 232;              // End of grainfilling to ripeness From MARKVAND (Sf4)
   Phenology->TB0          = 0;                // Corresponds to TS0
   Phenology->TB1          = 0;                // Corresponds to TS1
   Phenology->TB2          = 6;                // Corresponds to TS2
   Phenology->TB3          = 0;                // Corresponds to TS3
   Phenology->DS_Flagligule= 1.0;              // Undefined!
   Phenology->DS_StartFill = 1.20;
   Phenology->DB           = 8.0;

  	WaterDeficitVegGrowth = 0.65;
   WaterDeficitLagPhase  = 0.50;
   WaterDeficitGrainFill = 0.65;
//   TempSumR2             = 900;       // See p. 160+172 in DAISY-description

#ifdef MELCAL
   Phenology->TS1          = 540;          // Emergence to anthesis
   Phenology->TS2          = 500;     // Anthesis to end of grainfilling
   Phenology->TS3          = 200;    // End of grainfilling to ripeness
   NPctMin    = 0.3;
	MinN_Store = 0.029;                // Minimum content of nitrogen i storage organs
	MaxN_Store = 0.037;                // Maximum content of nitrogen i storage organs
	MinAllocToRoot=0.3;              // Minimal fraction of dry matter production that is allocated to the root (Value from DAISY (spring rape))
	MaxAllocToRoot=0.6;               // Maximal fraction of dry matter production that is allocated to the root (Value from DAISY (spring rape))
#endif
}


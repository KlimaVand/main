/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropWrape.h"

cropWrape::cropWrape(const char * aName, const int aIndex, const base * aOwner, string cropName)
          :crop(aName,aIndex,aOwner, cropName)
{
	// Parameters

   PlantItemName = "WNRPE";                   // These names maps to products.dat !
   StrawItemName = "RAPESTRAW";              // These names maps to products.dat !
	WinterSeed = true;

   //Parameters (MEL 2009)
 	WaterDeficitVegGrowth 	= 0.65;
   WaterDeficitLagPhase  	= 0.50;
   WaterDeficitGrainFill 	= 0.65;

   // Root
   MaxRootDepthCrop 			= 2.5;            // new estimate from KTK (18_3_3)
	RootPentrRate    			= 0.0015;         // new estimate from KTK (18_3_3)
	RootDistrParm        	= 1.0;          // Root density distribution parameter


   // DM production and translocation
   MaxRadUseEff				= 3.9;                 // Maximum radiation use efficiency (g/MJ)  (Justes et al. 2000)
	ConversionCoefficient	= 0.5;        // Coefficent of DM conversion from vegetative to storage. Low because of high energy content in oily seed
   GerminationConversion	= 1.2;        // Conversion factor seed DM -> plant DM
	FillFactor 					= 0.6;                // Guestimate - Fraction of net production after anthesis that goes into grain
   StoreForFilling 			= 0.4;          // Guestimate - Fraction of DM present at initiation of grain filling that is translocated to grain

	MinAllocToRoot				= 0.4;              // Minimal fraction of dry matter production that is allocated to the root
	MaxAllocToRoot				= 0.7;               // Maximal fraction of dry matter production that is allocated to the root
   MinDMtemp					= 0.0;                    // Minimum temperature for dry matter production
	MaxDMtemp					= 5.0;                    // Maximum temperature for dry matter production
	RelativeDensity         = 1.0;
   // N
	NitrateUptakeRate  		= 0.00006; 			//
   PowDM      					= -0.21;                // Power of dry matter in nitrogen status calculation (Colnelle et al 1998)
   NPctMax    					= 6.18;                  // Following Justes concept adjusted (Colnelle et al 1998)
   NPctMin   					= 2.07;                  // Following Justes concept adjusted (Colnelle et al 1998)
	MinN_Store 					= 0.03;                // Minimum content of nitrogen i storage organs
	MaxN_Store 					= 0.037;                // Maximum content of nitrogen i storage organs
	MinN_Root     				= 0.01;           	 // Minimum content of nitrogen in root organs
	MaxN_Root     				= 0.02;           	 // Maximum content of nitrogen in root organs

   // LAI and canopy structure
   k                			= 0.62;           // Value taken from beet
	MaxPlantHeight   			= 1.6;            // Max plant height (Value from DAISY)
   Conversfactor    			= 0.4;
	CoeffGreenIdx    			= 0.01;          // Coefficient for leaf growth (Optimized)
   LAINitrogenRatio 			= 0.5;           // Maximum ratio between LAI and nitrogen in veg top
   LAIDMRatio       			= 0.01;         // Maximum ratio between LAI and DM in veg top
   InitialCAI       			= 0.5;            // Guestimate of initial green leaf area index after 200 degree days

	// Phenology
   Phenology->TS0          = 125;
   Phenology->TS1          = 700;
   Phenology->TS2          = 400;
   Phenology->TS3          = 100;              // End of grainfilling to ripeness
   Phenology->TB0          = 0;                // Corresponds to TS0
   Phenology->TB1          = 0;                // Corresponds to TS1
   Phenology->TB2          = 6;                // Corresponds to TS2
   Phenology->TB3          = 0;                // Corresponds to TS3
   Phenology->DS_Flagligule= 1.0;              // Undefined!
   Phenology->DS_StartFill = 1.10;
   Phenology->DB           = 8.0;
}


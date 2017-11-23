/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropMaize.h"
#include "cropMaize2.h"
#include "../../products/products.h"

cropMaize::cropMaize(const char * aName, const int aIndex, const base * aOwner)
          :cropMaize2(aName,aIndex,aOwner)
{
//	// Parameters
//   CropName   = "Maize";
//   PlantItemName = "MAIZE";           // These names map to products.dat !
//   StrawItemName = "MAIZESTRAW";      // These names map to products.dat !
//	WinterSeed = false;
//   C4Photosynthesis = true;
//
//   // Root
//   MaxRootDepthCrop 			= 2.0;            // (From Bleken et al. 2009 (swedish data))
//	RootDistrParm    			= 3.0;            // (as cereals)
//   RootPentrRate     		= 0.004;        // Root penetration rate (m d-1 deg C-1) (From Bleken et al. 2009 (swedish data))
//
//   // DM production and translocation
//   MaxRadUseEff				= 3.0;                  //
//   FillFactor 					= 0.7;                  //ESTIMATE
//   StoreForFilling 			= 0.6;             //ESTIMATE
//   Conversfactor     		= 0.4;             // Part of green leaf that is converted to yellow leaf by wilting
//   MinAllocToRoot        	= 0.15;         // Minimal fraction of dry matter production that is allocated to the root  (From Bleken et al. 2009 (swedish data))
//	MaxAllocToRoot        	= 0.3;         // Maximal fraction of dry matter production that is allocated to the root  (From Bleken et al. 2009 (swedish data))
//
//   // N
//   PowDM      					= -0.42;                //Plenet and Lemaire, 2000. Plant and soil
//   NPctMax    					= 6.3;                  // Following Justes concept.
//   NPctMin 	  					= 2.05;
//	MinN_Store 					= 0.012;                // Minimum content of nitrogen i storage organs  (fodermiddeltabellen)
//	MaxN_Store 					= 0.018;                // Maximum content of nitrogen i storage organs  (fodermiddeltabellen)
//	MinN_Root     				= 0.01;           	 // Minimum content of nitrogen i root organs
//	MaxN_Root     				= 0.03;           	 // Maximum content of nitrogen i root organs
//   ReducedNInYellow  		= 0.39;          //The min and max N% for yellow VegTop is reduced with this parameter compared to green. From CENTS(plowed, foulum)
//   NitrateUptakeRate  		= 0.0006;     // Uptake rate for nitrate-N g/m/d (m root length)
//
//
//   // LAI and canopy structure
// 	k                			= 0.65;            // Extinction coefficient for photosynthetic active radiation (From Bleken et al. 2009 (swedish data))
//	MaxPlantHeight   			= 2.3;            // Beretning nr S 2004 - 1989
//   LAINitrogenRatio 			= 0.4;       	// Maximum ratio between LAI and nitrogen in veg top (estimated from Lindquist & Mortensen(1999) Weed Res. 39, 271)
//   LAIDMRatio       			= 0.0325;           // Maximum ratio between LAI and DM in veg top (Amanullah et al 2007)
//   InitialCAI       			= 0.5;            // Guestimate of initial green leaf area index after 200 degree days
//   CoeffGreenIdx    			= 0.03;          // Coefficient for leaf growth
//   MinimumSenescense 		= 0.3;            // Used to calculate minimum senescense - value as calibrated for winter wheat
//
//	// Phenology
//   Phenology->TS0          = 70;               // Sowing to emergence  (From Bleken et al. 2009 (swedish data))
//   Phenology->TS1          = 650;		        // Emergence to anthesis
//   Phenology->TS2          = 300;              // Anthesis to end of grainfilling  (From Bleken et al. 2009 (swedish data))
//   Phenology->TS3          = 200;              // Anthesis to end of grainfilling  (From Bleken et al. 2009 (swedish data))
//   Phenology->TB0          = 7;               // Corresponds to TS0   (From Bleken et al. 2009 (swedish data))
//   Phenology->TB1          = 6;                // Corresponds to TS1       (From Bleken et al. 2009 (swedish data))
//   Phenology->TB2          = 7;                // Corresponds to TS2       (From Bleken et al. 2009 (swedish data))
//   Phenology->TB3          = 7;                // Corresponds to TS2       (From Bleken et al. 2009 (swedish data))
//   Phenology->DS_Flagligule= 1.0;              // Undefined!
//   Phenology->DS_StartFill = 1.20;
//   Phenology->DB           = 8.0;

}

/****************************************************************************\
\****************************************************************************/
/*double cropMaize::fTW()
{
   double res = 1.0-0.0025*(temp-26.0)*(temp-26.0);    // Response to DM production from CERES.
   res = max(0.0,min(1.0,res));
   return res;
} */


/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropSunflower.h"
#include "../../products/products.h"

cropSunflower::cropSunflower(const char * aName, const int aIndex, const base * aOwner)
          :cropMaize(aName,aIndex,aOwner)
{
	// Parameters
   CropName   = "Sunflower";
   PlantItemName = "SUNFLOWER";           // These names map to products.dat !
   StrawItemName = "SUNFLOWERSTRAW";      // These names map to products.dat !

	// Parameters
   // Root
   MaxRootDepthCrop 			= 0.7;            // (From Todorovic et al. 2009)
	RootDistrParm    			= 3.0;            // (as cereals)
   RootPentrRate     		= 0.004;        // Root penetration rate (m d-1 deg C-1) (From maize)

   // DM production and translocation
   MaxRadUseEff				= 7.0;                  //
   FillFactor 					= 0.9;                  //ESTIMATE
   StoreForFilling 			= 0.3;             //ESTIMATE
   Conversfactor     		= 0.05;             // Part of green leaf that is converted to yellow leaf by wilting
   MinAllocToRoot        	= 0.1;         // Minimal fraction of dry matter production that is allocated to the root  (From Todorovic et al. 2009)
	MaxAllocToRoot        	= 0.5;         // Maximal fraction of dry matter production that is allocated to the root  (From Todorovic et al. 2009)

   // N
   PowDM      					= -0.51;                //From (Lemaire et al. 2007)
   NPctMax    					= 6.0;                  //From (Lemaire et al. 2007)
   NPctMin 	  					= 1.2;                  //From maize
	MinN_Store 					= 0.026;                // Minimum content of nitrogen i storage organs  (fodermiddeltabellen)
	MaxN_Store 					= 0.03;                // Maximum content of nitrogen i storage organs  (fodermiddeltabellen)
	MinN_Root     				= 0.01;           	 // Minimum content of nitrogen i root organs
	MaxN_Root     				= 0.03;           	 // Maximum content of nitrogen i root organs
   ReducedNInYellow  		= 0.39;          //The min and max N% for yellow VegTop is reduced with this parameter compared to green. From CENTS(plowed, foulum)
   NitrateUptakeRate  		= 0.0006;     // Uptake rate for nitrate-N g/m/d (m root length)


   // LAI and canopy structure
 	k                			= 0.7;            // Extinction coefficient for photosynthetic active radiation (From Todorovic et al. 2009)
	MaxPlantHeight   			= 2.3;            // Beretning nr S 2004 - 1989
   LAINitrogenRatio 			= 0.3;       	   // Maximum ratio between LAI and nitrogen in veg top (estimate)
   LAIDMRatio       			= 0.02;           // Maximum ratio between LAI and DM in veg top (From Todorovic et al. 2009)
   InitialCAI       			= 0.01;           // Guestimate of initial green leaf area index after 200 degree days
   CoeffGreenIdx    			= 0.0294;           // Coefficient for leaf growth  (From Todorovic et al. 2009)

	// Phenology
   Phenology->TS0          = 80;             // Sowing to emergence  (From Todorovic et al. 2009)
   Phenology->TS1          = 780;		      // Emergence to anthesis (From Todorovic et al. 2009)
   Phenology->TS2          = 159;            // Anthesis to end of grainfilling (From Todorovic et al. 2009)
   Phenology->TS3          = 622;            // End of grainfilling to ripe(From Todorovic et al. 2009)
   Phenology->TB0          = 6;              // Corresponds to TS0   (From Todorovic et al. 2009)
   Phenology->TB1          = 6;              // Corresponds to TS1   (From Todorovic et al. 2009)
   Phenology->TB2          = 6;              // Corresponds to TS2   (From Todorovic et al. 2009)
   Phenology->TB3          = 6;              // Corresponds to TS2   (From Todorovic et al. 2009)
   Phenology->DS_Flagligule= 1.0;            // Undefined!
   Phenology->DS_StartFill = 1.0;
   Phenology->DB           = 8.0;
}


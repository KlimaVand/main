/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "cropmaize.h"

cropMaize::cropMaize(const char * aName, const int aIndex, const base * aOwner)
          :crop(aName,aIndex,aOwner)
{
	// Parameters
   CropName   = "Maize";
   PlantItemName = "MAIZE";           // These names map to products.dat !
   StrawItemName = "MAIZESTRAW";      // These names map to products.dat !
	WinterSeed = false;

   // Root
   MaxRootDepthCrop = 2.0;            // estimate from www needs confirmation (http://amanzi.beeh.unp.ac.za/ageng/users/lynch/atlas_214t.htm)
	RootDistrParm    = 3.0;            // -II-
	//RootPentrRate    = 0.0025;         // new estimate from KTK (18_3_3)


   // DM production and translocation   (THESE ARE JUST GUESSTIMATES!!!!!!!!!!!!!!!!!!!!!!!!!!!)
   MaxRadUseEff=3.0;                  // Value from Sinclair & Muchow
//   StoreForFilling=0.5;             // Fraction of DM present at initiation of grain filling that is translocated to grain
//   FillFactor=0.7;                  // Fraction of net production after anthesis that goes into grain

   // N
   PowDM      = -0.42;                // Justes pp. 96
   NPctMax    = 6.3;                  // Following Justes concept.
	MinN_Store = 0.025;                // Minimum content of nitrogen i storage organs (Elly's data)
	MaxN_Store = 0.037;                // Maximum content of nitrogen i storage organs
	//MinN_Straw = 0.003;                // Minimum content of nitrogen i storage organs (Elly's data)
	//MaxN_Straw = 0.010;                // Maximum content of nitrogen i storage organs


   // LAI and canopy structure
   k                = 0.5;            // Value estimated from Lindquist & Mortensen(1999) Weed Res. 39, 271
	MaxPlantHeight   = 1.6;            // Beretning nr S 2004 - 1989
   CoeffGreenIdx    = 0.018;          // Coefficient for leaf growth (Lindquist & Mortensen(1999) Weed Res. 39, 271)
   LAINitrogenRatio = 5.0*0.14;       // Maximum ratio between LAI and nitrogen in veg top (estimated from Lindquist & Mortensen(1999) Weed Res. 39, 271)
   LAIDMRatio       = 0.02;           // Maximum ratio between LAI and DM in veg top (Estimated from graphs)
   InitialCAI       = 0.2;            // Guestimate of initial green leaf area index after 200 degree days

	// Phenology
	// Reestimation of base temperatures
   Phenology->TS0          = 50;               // Sowing to emergence
   Phenology->TS1          = 600;//350;        // Emergence to anthesis   NEED TO CHECK THIS JB!!!!!!!!!!!!
   Phenology->TS2          = 700;              // Anthesis to end of grainfilling
   Phenology->TS3          = 200;              // End of grainfilling to ripeness From MARKVAND (Sf4)
   Phenology->TB0          = 8;                // Corresponds to TS0
   Phenology->TB1          = 8;                // Corresponds to TS1
   Phenology->TB2          = 8;                // Corresponds to TS2
   Phenology->TB3          = 8;                // Corresponds to TS3
   Phenology->DS_Flagligule= 1.0;              // Undefined!
   Phenology->DS_StartFill = 1.20;
   Phenology->DB           = 8.0;

#ifdef MELCAL
   MaxRadUseEff = 4;
	MinN_Store = 0.012;                // Minimum content of nitrogen i storage organs
	MaxN_Store = 0.025;                // Maximum content of nitrogen i storage organs
	//From Bleken et al. 2009
   Phenology->TS0          = 70;               // Sowing to emergence
   Phenology->TS1          = 600;		        // Emergence to anthesis
   Phenology->TS2          = 500;              // Anthesis to end of grainfilling
   Phenology->TB0          = 6; //9;               // Corresponds to TS0
   Phenology->TB1          = 6;                // Corresponds to TS1
   Phenology->TB2          = 7;                // Corresponds to TS2
	k                 		= 0.65;            // Extinction coefficient for photosynthetic active radiation
   //From Plenet and Lemaire, 2000. Plant and soil
   PowDM      = -0.42;                // Justes pp. 96
   NPctMax    = 4.3; //6.3;                  // Following Justes concept.
   NPctMin 	  = 2.05;
#endif
}
/****************************************************************************\
\****************************************************************************/
double cropMaize::fTW()
{
   double res = 1.0-0.0025*(temp-26.0)*(temp-26.0);    // Response to DM production from CERES.
   res = max(0.0,min(1.0,res));
   return res;
}


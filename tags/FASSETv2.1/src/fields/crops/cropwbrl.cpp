/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "cropwbrl.h"

cropWbarley::cropWbarley(const char * aName, const int aIndex, const base * aOwner)
            :cropSbarley(aName,aIndex,aOwner)
{
   CropName = "WinterBarley";
   PlantItemName = "WNBRL";                 // These names maps to products.dat !
   StrawItemName = "BARLEYSTRAW";           // These names maps to products.dat !
   WinterSeed = true;
   //---- Parameters (largely based on winter wheat and spring barley - no calibration performed)

	MaxRootDepthCrop      = 1.5;             // Maximal root depth for this crop
   WaterDeficitVegGrowth = 0.60;
   WaterDeficitLagPhase  = 0.50;
   WaterDeficitGrainFill = 0.60;

	MinDMtemp=2.0;                    // Minimum temperature for dry matter production (4 in Hansen et al)
	MaxDMtemp=10;                     // Maximum temperature for dry matter production

	MinN_Store=0.012;                 // Minimum content of nitrogen i storage organs (Elly's data)
	MaxN_Store=0.027;                 // Maximum content of nitrogen i storage organs
	//MinN_Straw=0.004;                 // Minimum content of nitrogen i storage organs
	//MaxN_Straw=0.013;                 // Maximum content of nitrogen i storage organs

   // LAI and canopystructure

   MaxPlantHeight = 0.9;             // Max plant height
   LAIDistributionPar = 0.1;         // Estimated by JB from ecological trial at Foulum
												 // Temperature sums:
   // Phenology
   // Spring barley optimised 2002/07 (BMP)
   // Reestimation of base temperatures not possible, especially for spring crops highly confounded with T sums
	Phenology->TS0          = 125;              // Sowing to emergence  (from winter wheat)
   Phenology->TS1          = 346;              // Emergence to anthesis (estimate 2002/07)
   Phenology->TS2          = 315;              // Anthesis to end of grainfilling (estimate 2002/07)
   Phenology->TS3          = 157;              // End of grainfilling to ripeness
   Phenology->TB0          = 0;                // Corresponds to TS0
   Phenology->TB1          = 4;                // Corresponds to TS1
   Phenology->TB2          = 6;                // Corresponds to TS2
   Phenology->TB3          = 0;                // Corresponds to TS3
   Phenology->DS_Flagligule= 0.67;             // Taken from winter wheat
   Phenology->DS_StartFill = 1.13;             // Estimated 2002/07

//   TempSumR2=700;                    // See p. 160 in DAISY-description
	MinAllocToRoot=0.1;
   MaxAllocToRoot=0.5; //25;
   RhizoDepositFraction=0.5;
	MaxRadUseEff=2.8;            // Calibration based on data from Landforsøgene

#ifdef MELCAL
   MinN_Store    = 0.015;               // Minimum content of nitrogen i storage organs
	MaxN_Store    = 0.03;               // Maximum content of nitrogen i storage organs
   MaxRadUseEff = 4.0;
   Phenology->TS1          = 250;          // Emergence to anthesis
   Phenology->TS2          = 600;              // Anthesis to end of grainfilling
#endif
}


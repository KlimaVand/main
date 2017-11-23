/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include <cropoat.h>
#include <bstime.h>

/****************************************************************************\
\****************************************************************************/
cropOat::cropOat(const char * aName, const int aIndex, const base * aOwner)
        :crop(aName,aIndex,aOwner)
{
	// For nomenclature see base class 'crop'.
	CropName = "Oat";
   PlantItemName = "OAT";                 // These names maps to products.dat !
   StrawItemName = "OATSTRAW";            // These names maps to products.dat !
	WinterSeed = false;
   // Parameters
 	k = 0.65;                         // Value taken from spring barley
	MaxRootDepthCrop=1.0;             // Maximal root depth for this crop. Value taken from spring barley
												 // Max water deficits: (Values taken from spring barley)
   WaterDeficitVegGrowth = 0.50;
   WaterDeficitLagPhase  = 0.50;
   WaterDeficitGrainFill = 0.60;

	MinDMtemp  = 2.0;                   // Minimum temperature for dry matter production. Value taken from spring barley
	MaxDMtemp  = 10;                    // Maximum temperature for dry matter production. Value taken from spring barley

	MinN_Store = 0.013;                 // Minimum content of nitrogen in storage organs
	MaxN_Store = 0.029;                 // Maximum content of nitrogen in storage organs
	//MinN_Straw = 3.7/3.3*0.003;         // Minimum content of nitrogen in storage organs. Adjusted according to Fodermiddeltabel
	//MaxN_Straw = 3.7/3.3*0.01;          // Maximum content of nitrogen in storage organs. Adjusted according to Fodermiddeltabel
   NPctMax    = 7.5;              	 	// Following Justes concept

   // LAI and canopystructure

   MaxPlantHeight     = 1.0;         // Max plant height
   LAIDistributionPar = 0.1;         // Estimated by JB from ecological trial at Foulum

												 // Temperature sums:
   // Phenology
	Phenology->TS0          = 130;              // Sowing to emergence
   Phenology->TS1          = 560;              // Emergence to anthesis (401 sp. barl.)
   Phenology->TS2          = 310;              // Anthesis to end of grainfilling
   Phenology->TS3          = 175;              // End of grainfilling to ripeness
   Phenology->TB0          = 0;                // Corresponds to TS0
   Phenology->TB1          = 4;                // Corresponds to TS1
   Phenology->TB2          = 6;                // Corresponds to TS2
   Phenology->TB3          = 0;                // Corresponds to TS3
   Phenology->DS_Flagligule= 1.0; //0.60;      // Elongation = 1.36
   Phenology->DS_StartFill = 1.20;

//	TempSumR2=800;                    //See p. 160 in DAISY-description

	FillFactor=0.5;                   // Fraction of net production after anthesis that goes into grain
   Conversfactor=0.2;              // Part of green leaf that is converted to yellow leaf by wilting
	MaxRadUseEff = 3.3;       
   LAINitrogenRatio = 0.6; // 0.4//0.75; //0.45;        // Maximum ratio between LAI and nitrogen in veg top
   LAIDMRatio = 1.0; //0.015;             // Maximum ratio between LAI and DM in veg top
   MinAllocToRoot=0.1; //0.05;
   MaxAllocToRoot=0.5; //25;
   RhizoDepositFraction=0.5;

	MaxRadUseEff = 4.4;               // 3+22% from roots
   NitrateUptakeRate  	= 0.000024;
}

/****************************************************************************\
\****************************************************************************/
double cropOat::Update(double ActivePar)
{
	return crop::Update(ActivePar);
}


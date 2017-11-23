/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "cropbeet.h"

/****************************************************************************\
\****************************************************************************/
cropBeet::cropBeet(const char * aName, const int aIndex, const base * aOwner)
         :crop(aName,aIndex,aOwner)
{
   // Parameters
   CropName   = "Beets";
   PlantItemName = "BEET";                 // These names maps to products.dat !
   StrawItemName = "BEETTOP";              // These names maps to products.dat !
	WinterSeed = false;                     // Is the present crop a wintercrop.

   // Root
	RootPentrRate    = 0.001;        // new estimate from KTK (18_3_3)
	MaxRootDepthCrop = 2.0;          // new estimate from KTK (18_3_3)
   SpecificRootLength = 300;
   NitrateUptakeRate  	= 2.0*0.00006; //0.4*0.00006; // NEW value from DAISY (JB 2002-8) 0.000052;     // (0.31) Old value 0.000052. Uptake rate for nitrate-N g/m/d (m root length)

   // DM production and translocation
   MaxRadUseEff      = 4.0; //3.8;          // Maximum radiation use efficiency (g/MJ)
	MinDMtemp         = 4;            // Minimum temperature for dry matter production
	MaxDMtemp         = 10;           // Maximum temperature for dry matter production
	MinAllocToRoot    = 0.3; //0.1;          // Minimal fraction of dry matter production that is allocated to the root - Value from DAISY
   MaxAllocToRoot    = 0.3; //0.5; //0.6;          // Maximal fraction of dry matter production that is allocated to the root - Value from DAISY
   MinAllocToStorage = 0.0;          // Min. allocation to storage organs - from DAISY
	MaxAllocToStorage = 0.70;         // Max. allocation to storage organs - DAISY: 0.6

   // N
   MinN_Store    = 0.009;            // Minimum content of nitrogen i storage organs
	MaxN_Store    = 0.012; //0.013;            // Maximum content of nitrogen i storage organs
   //MinN_Straw    = 0.01;         	 // Minimum content of nitrogen i drymatter organs
	//MaxN_Straw    = 0.03;         	 // Maximum content of nitrogen i drymatter organs
   PowDM         = -0.56;          	 // Power of dry matter (taken from potato?!?!?!)
   NPctMax       = 8.0;           	 // Following Justes concept (taken from potato)

   // LAI and canopy structure
   MaxPlantHeight   = 1;             // Max plant height - Value from DAISY
	Conversfactor    = 0.4;           // Part of green leaf that is converted to yellow leaf by wilting - same value as winter wheat
   InitialCAI       = 0.05;          // Initial green leaf area index after 200 degree days
   CoeffGreenIdx    = 0.05;          // Coefficient for leaf growth (Optimized)
   LAINitrogenRatio = 0.75; //0.55;          // Maximum ratio between LAI and nitrogen in veg top (Estimated from graphs)
   LAIDMRatio       = 0.015;         // Maximum ratio between LAI and DM in veg top (Estimated from graphs)
	k=0.62;                           // Extinction coefficient for photosynthetic active radiation
                                     // According to: 1) Rover, A. 1994. Lichabsorbtion und Ertrag in Abhangigkeit vom Blattflachindex
                                     //                  bei Zuckerruben. Zuckerindustrie. 119:8, 664-670. Value = 0.61
                                     //               2) Szeicz, G. 1974. Solar radiation in crop canopies.
                                     //                  J. Appl. Ecol., 11:3, 1117-1156. Value = 0.63

   // Phenology
	Phenology->TS0          = 200;              // Sowing to emergence
   Phenology->TS1          = 235;              // Emergence to anthesis
   Phenology->TS2          = 3650;             // Anthesis to end of grainfilling
   Phenology->TS3          = 0;                // End of grainfilling to ripeness
   Phenology->TB0          = 0;                // Corresponds to TS0
   Phenology->TB1          = 0;                // Corresponds to TS1
   Phenology->TB2          = 0;                // Corresponds to TS2
   Phenology->TB3          = 0;                // Corresponds to TS3
   Phenology->DS_Flagligule= 0.8;              //
   Phenology->DS_StartFill = 1+650/3650;

   WaterDeficitVegGrowth = 0.65;

//	TempSumR2       = 900;            // See p. 160 in DAISY-description
   TempSumStorage1 = 400;            // Temperature sum for min allocation to storage - DAISY: 0
   TempSumStorage2 = 900;            // Temperature sum for max allocation to storage - from DAISY

	ReducedNInYellow =Conversfactor;  // The min and max N for yellow VegTop is reduced with this parameter
#ifdef MELCAL
   MaxRadUseEff      = 4.0; //3.8;          // Maximum radiation use efficiency (g/MJ)
   MinN_Store    = 0.0072;            // Minimum content of nitrogen i storage organs
	MaxN_Store    = 0.010; //0.013;            // Maximum content of nitrogen i storage organs
	MinAllocToRoot    = 0.1; //0.1;          // Minimal fraction of dry matter production that is allocated to the root - Value from DAISY
   MaxAllocToRoot    = 0.3; //0.5; //0.6;          // Maximal fraction of dry matter production that is allocated to the root - Value from DAISY
   CoeffGreenIdx    = 0.5;          // Coefficient for leaf growth (Optimized)
	Phenology->TS0          = 250;              // Sowing to emergence
   Phenology->TS1          = 800;              // Emergence to anthesis
	Phenology->TS2          = 3000;             // Anthesis to end of grainfilling
   Phenology->TS3          = 1000;                // End of grainfilling to ripeness
#endif
}

/****************************************************************************\
\****************************************************************************/
cropBeet::cropBeet(const cropBeet& acrop)
    : crop(acrop)
{
  
  TempSumStorage1=acrop.TempSumStorage1;
  TempSumStorage2=acrop.TempSumStorage2;
  MinAllocToStorage=acrop.MinAllocToStorage;
  MaxAllocToStorage=acrop.MaxAllocToStorage;
}

/****************************************************************************\
\****************************************************************************/
void cropBeet::Add(crop* aCrop, double fraction)
{
   crop::Add(aCrop,fraction);
   cropBeet * c = (cropBeet*)aCrop;

  	TempSumStorage1=(1-fraction)*TempSumStorage1 + fraction*c->TempSumStorage1;
   TempSumStorage2=(1-fraction)*TempSumStorage2 + fraction*c->TempSumStorage2;
   MinAllocToStorage=(1-fraction)*MinAllocToStorage + fraction*c->MinAllocToStorage;
   MaxAllocToStorage=(1-fraction)*MaxAllocToStorage + fraction*c->MaxAllocToStorage;
}

/****************************************************************************\
\****************************************************************************/
void cropBeet::ReadParameters(char* filename)
{
   crop::ReadParameters(filename);
   if (OpenInputFile(filename))
   {
	   if (FindSection(CropName))
   	{
			GetParameter("TempSumStorage1",&TempSumStorage1);
			GetParameter("TempSumStorage2",&TempSumStorage2);
			GetParameter("MinAllocToStorage",&MinAllocToStorage);
			GetParameter("MaxAllocToStorage",&MaxAllocToStorage);
		}
	}
   CloseInputFile();
}

/****************************************************************************\
\****************************************************************************/
void cropBeet::TransferDryMatterToStorage(double * dDryMatt)
{
   double deltaStorage = 0.0;
   if (TempSumRoot<=TempSumStorage1)
		deltaStorage=*dDryMatt*MinAllocToStorage;
	if ((TempSumRoot>TempSumStorage1) && (TempSumRoot<TempSumStorage2))
		deltaStorage=*dDryMatt*(MinAllocToStorage+(MaxAllocToStorage-MinAllocToStorage)
					 *((TempSumRoot-TempSumStorage1)/(TempSumStorage2-TempSumStorage1)));
	if (TempSumRoot>=TempSumStorage2)
		deltaStorage=*dDryMatt*MaxAllocToStorage;
	*dDryMatt-=deltaStorage;
	DryMatterStorage+=deltaStorage;
}


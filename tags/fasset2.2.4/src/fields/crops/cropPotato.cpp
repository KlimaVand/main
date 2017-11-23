/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "../../data/commonData.h"
#include "cropPotato.h"

/****************************************************************************\
\****************************************************************************/
cropPotato::cropPotato(const char * aName, const int aIndex, const base * aOwner)
           :crop(aName,aIndex,aOwner)
{
   // Potato tubers approx. 24% DM ("Fodermiddeltabellen", 2000)
   // Planted approx 2 t/ha wet weight = 0.24 t DM/ha
   // Harvest amounts intermediate early: approx. 50 - 70 t wet weight; late: approx 60 - 80 t wet weight

   // Parameters
   CropName   = "Potato";
   PlantItemName = "POTATO";                 // These names maps to products.dat !
   StrawItemName = "POTATOTOP";              // These names maps to products.dat !
	WinterSeed = false;                       // Is the present crop a wintercrop.

   // Root
	RootPentrRate    = 0.001;        // Taken from beet
	MaxRootDepthCrop = 1.5;          // Guess

   // DM production and translocation
   MaxRadUseEff      = 4.0;          // Maximum radiation use efficiency (g/MJ)
	MinDMtemp         = 4;            // Minimum temperature for dry matter production
	MaxDMtemp         = 10;           // Maximum temperature for dry matter production
	MinAllocToRoot    = 0.1;          // Minimal fraction of dry matter production that is allocated to the root
   MaxAllocToRoot    = 0.3;          // Maximal fraction of dry matter production that is allocated to the root 
   MinAllocToStorage = 0.4;          // Min. allocation to storage organs
	MaxAllocToStorage = 0.75;         // Max. allocation to storage organs

   // N
   // Tubers have approx. 1.5 % N content ("Fodermiddeltabellen", 2000)
   MinN_Store    = 0.011;            // Minimum content of nitrogen i storage organs
	MaxN_Store    = 0.018;            // Maximum content of nitrogen i storage organs
   PowDM         = -0.42;          	 // Power of dry matter (From Belanger et al., 2001)
   NPctMax       = 6.0;           	 // Following Justes concept (From Belanger et al., 2001)

   // LAI and canopy structure
   MaxPlantHeight   = 1;             // Max plant height - Value from beet
	Conversfactor    = 0.4;           // Part of green leaf that is converted to yellow leaf by wilting - same value as winter wheat
   InitialCAI       = 0.6;           // Initial green leaf area index after 200 degree days - guess
   CoeffGreenIdx    = 0.05;          // Coefficient for leaf growth (Optimized)
   LAINitrogenRatio = 0.75;          // 0.75 Maximum ratio between LAI and nitrogen in veg top
   LAIDMRatio       = 0.015;         // 0.015 Maximum ratio between LAI and DM in veg top
	k=0.62;                           // Extinction coefficient for photosynthetic active radiation

   // Phenology
	Phenology->TS0          = 285;              // Planting to emergence - ca 25 dage - l�gges sidst i april
   Phenology->TS1          = 300;              // Emergence to anthesis
   Phenology->TS2          = 1000;             // Anthesis to end of "grain"-filling
   Phenology->TS3          = 0;                // End of grainfilling to ripeness
   Phenology->TB0          = 0;                // Corresponds to TS0
   Phenology->TB1          = 0;                // Corresponds to TS1
   Phenology->TB2          = 0;                // Corresponds to TS2
   Phenology->TB3          = 0;                // Corresponds to TS3
   Phenology->DS_Flagligule= 0.8;              //
   Phenology->DS_StartFill = 1+650/3650;

   WaterDeficitVegGrowth = 0.35;    //from Markvand
  	WaterDeficitLagPhase  	= 0.35;
   WaterDeficitGrainFill 	= 0.45;

   TempSumStorage1 = 400;            // Temperature sum for min allocation to storage
   TempSumStorage2 = 900;            // Temperature sum for max allocation to storage
	NitrateUptakeRate  	= 0.00006;
}

/****************************************************************************\
\****************************************************************************/
cropPotato::cropPotato(const cropPotato& acrop)
    : crop(acrop)
{

  TempSumStorage1=acrop.TempSumStorage1;
  TempSumStorage2=acrop.TempSumStorage2;
  MinAllocToStorage=acrop.MinAllocToStorage;
  MaxAllocToStorage=acrop.MaxAllocToStorage;
}

/****************************************************************************\
\****************************************************************************/
void cropPotato::Add(crop* aCrop, double fraction)
{
   crop::Add(aCrop,fraction);
   cropPotato * c = (cropPotato*)aCrop;

  	TempSumStorage1=(1-fraction)*TempSumStorage1 + fraction*c->TempSumStorage1;
   TempSumStorage2=(1-fraction)*TempSumStorage2 + fraction*c->TempSumStorage2;
   MinAllocToStorage=(1-fraction)*MinAllocToStorage + fraction*c->MinAllocToStorage;
   MaxAllocToStorage=(1-fraction)*MaxAllocToStorage + fraction*c->MaxAllocToStorage;
}

/****************************************************************************\
\****************************************************************************/
void cropPotato::ReadParameters(char* filename)
{	commonData data;
   if (data.readFileWithoutExit("CropParameters.dat"))
   {
	   int sectionName=data.FindSection(CropName);
		if (sectionName!=-1)
      {
			data.FindItem("TempSumStorage1",sectionName,TempSumStorage1);
			data.FindItem("TempSumStorage2",sectionName,TempSumStorage2);
			data.FindItem("MinAllocToStorage",sectionName,MinAllocToStorage);
			data.FindItem("MaxAllocToStorage",sectionName,MaxAllocToStorage);
		}
	}

}

/****************************************************************************\
\****************************************************************************/
void cropPotato::TransferDryMatterToStorage(double * dDryMatt)
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


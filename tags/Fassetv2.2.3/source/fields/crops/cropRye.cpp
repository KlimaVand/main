/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "../../data/commonData.h"
#include "cropRye.h"
#include "../../base/climate.h"
#include "../../base/controlParameters.h"
#include "../../base/bstime.h"

/****************************************************************************\
\****************************************************************************/
cropRye::cropRye(const char * aName, const int aIndex, const base * aOwner)
        :crop(aName,aIndex,aOwner)
{
   CropName   = "Rye";
   PlantItemName = "RYE";                   // These names maps to products.dat !
   StrawItemName = "RYESTRAW";              // These names maps to products.dat !

	WinterSeed = true;
   //---- Parameters ---------------------------------------------
	MaxRootDepthCrop = 1.5;                 // Taken from winter wheat

	// Water deficits from MARKVAND
   WaterDeficitVegGrowth = 0.70;
   WaterDeficitLagPhase  = 0.55;
   WaterDeficitGrainFill = 0.70;

	// Drymatter production
	MinDMtemp=0.0;                    // winter wheat: 2
	MaxDMtemp=8.0;                    // Maximum temperature for dry matter production. Winter wheat: 10
	MinAllocToRoot=0.3;               // Minimal fraction of dry matter production that is allocated to the root. Taken from winter wheat
	MaxAllocToRoot=0.6;               // Maximal fraction of dry matter production that is allocated to the root. Taken from winter wheat
   StoreForFilling=0.39;             // Fraction of DM present at initiation of grain filling that is translocated to grain. Taken from winter wheat
   FillFactor=0.57;                  // Fraction of net production after anthesis that goes into grain. Taken from winter wheat
   MaxPlantHeight=1.0;               // Max plant height. Taken from winter wheat.
	Conversfactor=0.4;                // Part of green leaf that is converted to yellow leaf by wilting. Taken from winter wheat.
   TopFraction=0.5;                  // Fraction of seed that goes into top. Taken from winter wheat.

   MinN_Store    = 0.011;            // Minimum content of nitrogen in storage organs
	MaxN_Store    = 0.022;            // Maximum content of nitrogen in storage organs

   // Phenology
   Phenology->TS0          = 125;               // Sowing to emergence. Taken from winter wheat
   Phenology->TS1          = 250+120;// 308+153 // Emergence to anthesis
   Phenology->TS2          = 330;    // 420     // Anthesis to end of grainfilling
   Phenology->TS3          = 160;               // End of grainfilling to ripeness. Taken from MARKVAND
   Phenology->TB0          = 0;                 // Corresponds to TS0
   Phenology->TB1          = 4;                 // Corresponds to TS1
   Phenology->TB2          = 6;                 // Corresponds to TS2
   Phenology->TB3          = 0;                 // Corresponds to TS3
   Phenology->DS_Flagligule= 250.0/Phenology->TS1; // 308
   Phenology->DS_StartFill = 1.0+60.0/Phenology->TS2;

//	TempSumR2=700;                    // See p. 160 in DAISY-description. Value taken from winter wheat

   // LAI and canopy structure
	CoeffGreenIdx = 0.0092;           // Coefficient for leaf growth. Value taken from winter wheat
   LAINitrogenRatio = 0.4;           // Maximum ratio between LAI and nitrogen in veg top. Value taken from winter wheat
   LAIDMRatio = 0.011;               // Maximum ratio between LAI and DM in veg top. Value taken from winter wheat
   InitialCAI = 0.65; // 0.46        // Initial green leaf area index after 200 degree days.
   HeightA = 0.61;                   // See Olesen, 2002 for description. Value taken from winter wheat
   HeightB = 0.65;                   // Value taken from winter wheat
   MinimumSenescense=0.35;           // Used to calculate extra senescense caused by dryness. Value taken from winter wheat
	MinAllocToRoot=0.4;
   MaxAllocToRoot=0.7;
   RhizoDepositFraction=0.5;
   FillFactor=0.45;                   // Fraction of net production after anthesis that goes into grain
   MaxRadUseEff = 4.4;
   ReducedNInYellow  = 0.39;          //The min and max N% for yellow VegTop is reduced with this parameter compared to green. From CENTS(plowed, foulum) - estimated from WinterWheat and WinterBarley
}

/****************************************************************************\
\****************************************************************************/
void cropRye::ReadParameters(char* filename)
{
  commonData data;
   if (data.readFileWithoutExit("CropParameters.dat"))
   {
	   int sectionName=data.FindSection(CropName);
		if (sectionName!=-1)
      {
			data.FindItem("HeightA",sectionName, HeightA);
			data.FindItem("HeightB",sectionName,HeightB);
      }
   }

}

/****************************************************************************\
\****************************************************************************/
void cropRye::Sow(double SeedDensDryMatt,double aRelativeDensity,double NitrogenInSeed)
{
   crop::Sow(SeedDensDryMatt,aRelativeDensity,NitrogenInSeed);
   Phenology->Sow();
}

/****************************************************************************\
\****************************************************************************/
double cropRye::fNitrogen()
{
   if (Phenology->Ripe())
   	return fNitrogenAfterRipe; // Ensures that no N translocation takes place after ripeness
   else
      return crop::fNitrogen();
}

/****************************************************************************\
\****************************************************************************/
void cropRye::UpdateHeight()
{
   double DS_Converted = Phenology->DS/Phenology->DS_Flagligule;
   MaxPlantHeight = 1.0/(1.0/HeightA-log(2.0)/(HeightB*2.0));
   if (Phenology->Emerged())
		PlantHeight = 1.0/(1.0/HeightA-log(DS_Converted)/(HeightB*DS_Converted));
   PlantHeight = min(PlantHeight,MaxPlantHeight);
}


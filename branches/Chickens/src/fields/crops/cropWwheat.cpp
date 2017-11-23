/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropWwheat.h"
#include "../../base/climate.h"

#include "../../base/bstime.h"

/****************************************************************************\
\****************************************************************************/
cropWwheat::cropWwheat(const char * aName, const int aIndex, const base * aOwner)
           :crop(aName,aIndex,aOwner)
{
   CropName   = "WinterWheat";
   PlantItemName = "WNWHT";                   // These names maps to products.dat !
   StrawItemName = "WHEATSTRAW";              // These names maps to products.dat !

	WinterSeed = true;
   //---- Parameters ---------------------------------------------

   //Root growth
	MaxRootDepthCrop 			= 1.5;             // new estimate from KTK (18_3_3)
	NitrateUptakeRate			= 0.00006;

	// Water deficits from MARKVAND
   WaterDeficitVegGrowth 	= 0.65;
   WaterDeficitLagPhase  	= 0.45;
   WaterDeficitGrainFill 	= 0.60;

	// Drymatter production
  	MaxRadUseEff 				= 3.6;              //fittet on 3 N levels, 4 years (Foulum)
	MinDMtemp					= 2.0;              // From DAISY
	MaxDMtemp					= 7.0;              // Maximum temperature for dry matter production
	NCurveParam					= -1000;

	//Allocation
	MinAllocToRoot				= 0.3;              // Minimal fraction of dry matter production that is allocated to the root.
	MaxAllocToRoot 			= 0.6;              // Maximal fraction of dry matter production that is allocated to the root
   StoreForFilling			= 0.25;             // Fraction of DM present at initiation of grain filling that is translocated to grain
   FillFactor					= 0.55;             // Fraction of net production after anthesis that goes into grain
   MaxPlantHeight				= 1.0;              // Max plant height (Value from DAISY)
   Conversfactor     		= 0.3;              // Part of green leaf that is converted to yellow leaf by wilting, fittet on 3 N levels, 4 years (Foulum)
   TopFraction					= 0.5;              // Fraction of seed that goes into top
   MinimumSenescense			= 0.35;             // Used to calculate extra senescense caused by dryness
   RhizoDepositFraction 	= 0.2;              // fittet on 3 N levels, 4 years (Foulum)

   // Phenology as in phenology.cpp

   // LAI and canopy structure
	CoeffGreenIdx 				= 0.0092;           // Coefficient for leaf growth, fittet on 3 N levels, 4 years (Foulum)
   LAINitrogenRatio 			= 0.4;              // Maximum ratio between LAI and nitrogen in veg top, fittet on 3 N levels, 4 years (Foulum)
   LAIDMRatio 					= 0.011;            // Maximum ratio between LAI and DM in veg top, fittet on 3 N levels, 4 years (Foulum)
   InitialCAI 					= 0.46;             // Initial green leaf area index after 200 degree days
   HeightA 						= 0.61;             // See Olesen, 2002 for description
   HeightB 						= 0.65;

	//N
   MinN_Store    				= 0.017;            // Minimum content of nitrogen i storage organs, fittet on 3 N levels, 4 years (Foulum)
	MaxN_Store    				= 0.023;            // Maximum content of nitrogen i storage organs, fittet on 3 N levels, 4 years (Foulum)
	MinN_Root     				= 0.01;             // Minimum content of nitrogen in root organs, fittet on 3 N levels, 4 years (Foulum)
	MaxN_Root     				= 0.02;             // Maximum content of nitrogen in root organs, fittet on 3 N levels, 4 years (Foulum)
   ReducedNInYellow  		= 0.3;              //The min and max N% for yellow VegTop is reduced with this parameter compared to green. , fittet on 3 N levels, 4 years (Foulum)
}

/****************************************************************************\
\****************************************************************************/
cropWwheat::~cropWwheat()
{
}

/****************************************************************************\
\****************************************************************************/
cropWwheat::cropWwheat(const cropWwheat& acrop)
  : crop( acrop )
{
   TransferableStorage = acrop.TransferableStorage;
   StoreForFilling = acrop.StoreForFilling;
}

/****************************************************************************\
Note that boolean state variables of course not are interpolated
\****************************************************************************/
void cropWwheat::Add(crop* aCrop, double fraction)
{
   crop::Add(aCrop,fraction);
}

/****************************************************************************\
\****************************************************************************/
void cropWwheat::ReadParameters(char* filename)
{
   crop::ReadParameters(filename);
   UnsetCritical();
   if (OpenInputFile(filename))
   {
	   if (FindSection(CropName) || FindSection("Crop"))
      {
			GetParameter("HeightA",&HeightA);
			GetParameter("HeightB",&HeightB);
      }
   }
	CloseInputFile();
}

/****************************************************************************\
\****************************************************************************/
void cropWwheat::Sow(double SeedDensDryMatt,double aRelativeDensity,double NitrogenInSeed)
{
   crop::Sow(SeedDensDryMatt,aRelativeDensity,NitrogenInSeed);
   Phenology->Sow();
}

/****************************************************************************\
\****************************************************************************/
void cropWwheat::Harvest(decomposable * Storage,decomposable* Straw)
{
   crop::Harvest(Storage,Straw);
}

/****************************************************************************\
\****************************************************************************/
double cropWwheat::fNitrogen()
{
   if (Phenology->Ripe())
   	return fNitrogenAfterRipe; // ensures that no N translocation takes place after ripeness
   else
      return crop::fNitrogen();
}
/****************************************************************************\
\****************************************************************************/
void cropWwheat::UpdateHeight()
{
   double DS_Converted = Phenology->DS/Phenology->DS_Flagligule;
   MaxPlantHeight = 1.0/(1/HeightA-log(2.0)/(HeightB*2));
   if (Phenology->Emerged())
   {
		PlantHeight = 1.0/(1/HeightA-log(DS_Converted)/(HeightB*DS_Converted));
   }
   PlantHeight = min(PlantHeight,MaxPlantHeight);
}


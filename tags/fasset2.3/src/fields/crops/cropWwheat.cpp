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
cropWwheat::cropWwheat(const char * aName, const int aIndex, const base * aOwner, string cropName)
           :crop(aName,aIndex,aOwner, cropName)
{

   PlantItemName = "WNWHT";                   // These names maps to products.dat !
   StrawItemName = "WHEATSTRAW";              // These names maps to products.dat !

	WinterSeed = true;
   HeightA 						= theCropData->getHeightA();            // See Olesen, 2002 for description
   HeightB 						= theCropData->getHeightB();


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


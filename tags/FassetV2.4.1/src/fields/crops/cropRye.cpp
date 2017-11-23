/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "../../base/commonData.h"
#include "cropRye.h"
#include "../../base/climate.h"
#include "../../base/controlParameters.h"
#include "../../base/bstime.h"

/****************************************************************************\
\****************************************************************************/
cropRye::cropRye(const char * aName, const int aIndex, const base * aOwner, string cropName)
        :crop(aName,aIndex,aOwner, cropName)
{

   PlantItemName = "RYE";                   // These names maps to products.dat !
   StrawItemName = "RYESTRAW";              // These names maps to products.dat !

	WinterSeed = true;


   commonData * data=globalSettings->CropInformation;
      data->FindItem("HeightA",&HeightA);
      data->FindItem("HeightB",&HeightB);
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

